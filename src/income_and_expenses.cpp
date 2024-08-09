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


void income_and_expenses::open_del_action()
{
    window_del->show();
    window_del->draw_table();
}


void income_and_expenses::calculations()
{
    QVector<QDate> transactionDates;
    QVector<double> moneyValues;
    query = new QSqlQuery(db);

    if (db.open())
    {
        if(query->exec("SELECT Money, Action, Date FROM Actions"))
        {
            while (query->next())
            {
                double money = query->value(0).toDouble();
                QString action = query->value(1).toString();
                QDate date = query->value(2).toDate();

                if (action == "Расходы")
                {
                    money *= -1; // Multiply money by -1 for expenses
                }
                moneyValues.push_back(money);
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
}
