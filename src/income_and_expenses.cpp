#include "include/mainwindow.h"
#include "ui_mainwindow.h"

income_and_expenses::income_and_expenses(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui)
{
    window_add = new add_action();
    window_del = new Del_action();

    connect(window_add, SIGNAL(calc()), this, SLOT(calculations()));
    connect(window_del, SIGNAL(calc()), this, SLOT(calculations()));
}


income_and_expenses::~income_and_expenses()
{
    //delete ui;
    delete window_add;
    delete window_del;
}


void income_and_expenses::open_inc_exp()
{
    add_Database();
    ui->stackedWidget->setCurrentWidget(ui->page_expenses_income);
    window_add->connect_info();
    calculations();
}


bool income_and_expenses::add_Database()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("finance");
    db.setUserName("postgres");
    db.setPassword("2k0a0r6AS");

    if (!db.open())
    {
        qDebug() << "Error: Unable to open database" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}


void income_and_expenses::open_add_action()
{
    window_add->show();

}


void income_and_expenses::draw_graph(QVector<double> money, QVector<QDate> date)
{
    ui->Chart->clearGraphs();
    ui->Chart->addGraph();
    QVector<double> dateIndices;

    for (int i = 0; i < date.size(); ++i)
    {
        dateIndices.append(i);
    }

    ui->Chart->graph(0)->setData(dateIndices, money);

    // Set the tick labels to display dates
    QVector<QString> labels;
    for (const QDate &d : date)
    {
        labels.append(d.toString("dd/MM/yyyy"));
    }
    ui->Chart->xAxis->setAutoTicks(false);
    ui->Chart->xAxis->setAutoTickLabels(false);
    ui->Chart->xAxis->setTickVector(dateIndices);
    ui->Chart->xAxis->setTickVectorLabels(labels);

    // Adjust X-axis range
    ui->Chart->xAxis->setRange(0, dateIndices.size()-1);

    // Adjust Y-axis range
    ui->Chart->yAxis->setRange(*std::min_element(money.begin(), money.end()), *std::max_element(money.begin(), money.end()));

    // Redraw the plot
    ui->Chart->replot();
}


void income_and_expenses::draw_diagrams(QVector<QString> categories_inc, QVector<double> money_inc, QVector<QString> categories_exp, QVector<double> money_exp)
{
    QPieSeries *series_inc = new QPieSeries();
    QPieSeries *series_exp = new QPieSeries();

    // Combine data for income categories
    QHash<QString, double> incomeData;
    for (int i = 0; i < categories_inc.size(); i++)
    {
        QString category = categories_inc[i];
        if (incomeData.contains(category))
        {
            incomeData[category] += money_inc[i];
        }
        else
        {
            incomeData.insert(category, money_inc[i]);
        }
    }

    // Add income categories to the income series
    for (auto it = incomeData.begin(); it != incomeData.end(); ++it)
    {
        QPieSlice *slice_inc = series_inc->append(it.key(), it.value());
        slice_inc->setLabel(it.key());
    }

    // Combine data for expense categories
    QHash<QString, double> expenseData;
    for (int i = 0; i < categories_exp.size(); i++)
    {
        QString category = categories_exp[i];
        if (expenseData.contains(category))
        {
            expenseData[category] += money_exp[i];
        }
        else
        {
            expenseData.insert(category, money_exp[i]);
        }
    }

    // Add expense categories to the expense series
    for (auto it = expenseData.begin(); it != expenseData.end(); ++it)
    {
        QPieSlice *slice_exp = series_exp->append(it.key(), it.value());
        slice_exp->setLabel(it.key());
    }

    // Create and display the income chart
    QChart *chart_inc = new QChart();
    chart_inc->addSeries(series_inc);
    chart_inc->setTitle("Доходы");
    chart_inc->legend()->setVisible(true);
    chart_inc->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView_inc = new QChartView(chart_inc);
    chartView_inc->setRenderHint(QPainter::Antialiasing);

    if(ui->diagram_incomes)
    {
        QVBoxLayout *layout_inc = new QVBoxLayout(ui->diagram_incomes);
        layout_inc->addWidget(chartView_inc);
        ui->diagram_incomes->setLayout(layout_inc);
    }

    ui->diagram_incomes->update();

    // Create and display the expense chart
    QChart *chart_exp = new QChart();
    chart_exp->addSeries(series_exp);
    chart_exp->setTitle("Расходы");
    chart_exp->legend()->setVisible(true);
    chart_exp->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView_exp = new QChartView(chart_exp);
    chartView_exp->setRenderHint(QPainter::Antialiasing);

    if(ui->diagram_expenses)
    {
        QVBoxLayout *layout_exp = new QVBoxLayout(ui->diagram_expenses);
        layout_exp->addWidget(chartView_exp);
        ui->diagram_expenses->setLayout(layout_exp);
    }

    ui->diagram_expenses->update();
}


void income_and_expenses::open_del_action()
{
    window_del->show();
    window_del->draw_table();
}


void income_and_expenses::calculations()
{
    QVector<QDate> transactionDates;
    QVector<double> moneyValues, money_inc, money_exp;
    QVector<QString> categories_inc, categories_exp;
    query = new QSqlQuery(db);
    QVector<double> wallet;

    if (db.open())
    {
        if(query->exec("SELECT Money, Action, Date, Category FROM Actions"))
        {
            while (query->next())
            {
                double money = query->value(0).toDouble();
                QString action = query->value(1).toString();
                QDate date = query->value(2).toDate();
                QString categor = query->value(3).toString();

                if (action == "Расходы")
                {
                    money_exp.push_back(money);
                    money *= -1; // Multiply money by -1 for expenses
                    categories_exp.push_back(categor);
                }
                else
                {
                    categories_inc.push_back(categor);
                    money_inc.push_back(money);
                }
                moneyValues.push_back(money);
                transactionDates.push_back(date);

            }
        }
        double balance = 0;
        bool is_warning = false;
        for (int i = 0; i < moneyValues.size(); i++)
        {
            balance += moneyValues[i];
            wallet += balance;
            if (balance <= 1000) // warning if there is not enough money in the account
            {
                is_warning = true;
            }
        }

        if(is_warning)
        {
            ui->label_advice->setText("ВНИМАНИЕ: Вы были близки к кризису. Будьте осторожнее.");
        }
        else
        {
            ui->label_advice->setText("Советы: хорошо живете.");
        }

        ui->label_balance->setNum(balance);
    }
    else
    {
        db.close();
        calculations();
    }

    qDebug() << categories_inc << money_inc << categories_exp << money_exp;
    draw_graph(wallet, transactionDates);
    draw_diagrams(categories_inc, money_inc, categories_exp, money_exp);
}
