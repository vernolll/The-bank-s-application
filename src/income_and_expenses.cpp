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
    // Create a map to store the last money value for each date
    QMap<QDate, double> dateMoneyMap;

    for (int i = 0; i < date.size(); ++i)
    {
        dateMoneyMap[date[i]] = money[i];
    }

    // Clear the original vectors and populate them with unique dates and corresponding money
    QVector<QDate> uniqueDates = dateMoneyMap.keys().toVector();
    QVector<double> uniqueMoney;

    for (const QDate &d : uniqueDates)
    {
        uniqueMoney.append(dateMoneyMap[d]);
    }

    ui->Chart->clearGraphs();
    ui->Chart->addGraph();
    QVector<double> dateIndices;

    for (int i = 0; i < uniqueDates.size(); ++i)
    {
        dateIndices.append(i);
    }

    ui->Chart->graph(0)->setData(dateIndices, uniqueMoney);

    // Set the tick labels to display dates
    QVector<QString> labels;
    for (const QDate &d : uniqueDates)
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
    ui->Chart->yAxis->setRange(*std::min_element(uniqueMoney.begin(), uniqueMoney.end()), *std::max_element(uniqueMoney.begin(), uniqueMoney.end()));

    // Redraw the plot
    ui->Chart->replot();
}



void income_and_expenses::draw_diagrams(QVector<QString> categories_inc, QVector<double> money_inc, QVector<QString> categories_exp, QVector<double> money_exp)
{
    // Clear existing layout in diagram_incomes if required
    QLayout* existingLayout_inc = ui->diagram_incomes->layout();
    if (existingLayout_inc)
    {
        QLayoutItem *child;
        while ((child = existingLayout_inc->takeAt(0)) != nullptr)
        {
            delete child->widget();
            delete child;
        }
        delete existingLayout_inc;
    }

    // Clear existing layout in diagram_expenses if required
    QLayout* existingLayout_exp = ui->diagram_expenses->layout();
    if (existingLayout_exp)
    {
        QLayoutItem *child;
        while ((child = existingLayout_exp->takeAt(0)) != nullptr)
        {
            delete child->widget();
            delete child;
        }
        delete existingLayout_exp;
    }

    // Create and display the income chart
    QPieSeries *series_inc = new QPieSeries(ui->diagram_incomes); // Set parent
    QHash<QString, double> incomeData;

    for (int i = 0; i < categories_inc.size(); i++)
    {
        QString category = categories_inc[i];
        incomeData[category] += money_inc[i]; // Simplified insertion
    }

    for (auto it = incomeData.begin(); it != incomeData.end(); ++it)
    {
        QPieSlice *slice_inc = series_inc->append(it.key(), it.value());
        slice_inc->setLabel(it.key());
    }

    QChart *chart_inc = new QChart();
    chart_inc->addSeries(series_inc);
    chart_inc->setTitle("Доходы");
    chart_inc->legend()->setVisible(true);
    chart_inc->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView_inc = new QChartView(chart_inc, ui->diagram_incomes); // Set parent
    chartView_inc->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout* layout_inc = new QVBoxLayout(ui->diagram_incomes);
    layout_inc->addWidget(chartView_inc);
    ui->diagram_incomes->setLayout(layout_inc);

    // Create and display the expense chart
    QPieSeries *series_exp = new QPieSeries(ui->diagram_expenses); // Set parent
    QHash<QString, double> expenseData;

    for (int i = 0; i < categories_exp.size(); i++)
    {
        QString category = categories_exp[i];
        expenseData[category] += money_exp[i]; // Simplified insertion
    }

    for (auto it = expenseData.begin(); it != expenseData.end(); ++it)
    {
        QPieSlice *slice_exp = series_exp->append(it.key(), it.value());
        slice_exp->setLabel(it.key());
    }

    QChart *chart_exp = new QChart(); // No parent here
    chart_exp->addSeries(series_exp);
    chart_exp->setTitle("Расходы");
    chart_exp->legend()->setVisible(true);
    chart_exp->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView_exp = new QChartView(chart_exp, ui->diagram_expenses); // Set parent
    chartView_exp->setRenderHint(QPainter::Antialiasing);

    QVBoxLayout* layout_exp = new QVBoxLayout(ui->diagram_expenses);
    layout_exp->addWidget(chartView_exp);
    ui->diagram_expenses->setLayout(layout_exp);
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

    QSqlQuery query(db);

    if (db.open())
    {
        query.prepare("SELECT Money, Action, Date, Category FROM Actions");
        if(query.exec())
        {
            while (query.next())
            {
                double money = query.value(0).toDouble();
                QString action = query.value(1).toString();
                QDate date = query.value(2).toDate();
                QString categor = query.value(3).toString();

                if (action == "Расходы")
                {
                    money_exp.push_back(money);
                    money *= -1; // Multiply money by -1 for expenses
                    categories_exp.push_back(categor);
                    moneyValues.push_back(money);
                }
                else
                {
                    categories_inc.push_back(categor);
                    money_inc.push_back(money);
                    moneyValues.push_back(money);
                }
                transactionDates.push_back(date);
            }
        }
    }
    else
    {
        db.close();
        calculations();
    }

    QVector<double> wallet;
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
    draw_graph(wallet, transactionDates);
    draw_diagrams(categories_inc, money_inc, categories_exp, money_exp);
}
