#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");

    stackedWidget = ui->stackedWidget;

    ui->stackedWidget->setCurrentWidget(ui->page);

    window_add = new add_action();
    window_del = new Del_action();

    connect(window_add, SIGNAL(calc()), this, SLOT(calculations()));
    connect(window_del, SIGNAL(calc()), this, SLOT(calculations()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete window_add;
    delete stackedWidget;
    delete window_del;
}


void MainWindow::on_pushButton_3_clicked()
{
    add_Database();
    ui->stackedWidget->setCurrentWidget(ui->page_expenses_income);
    window_add->connect_info();
    calculations();
}

bool MainWindow::add_Database()
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


void MainWindow::on_pushButton_add_clicked()
{
    window_add->show();

}


void MainWindow::draw_graph(QVector<double> money)
{
    QVector<double> actions;
    for(int i = 0; i < money.size(); i++)
    {
        actions.push_back(i);
    }
    ui->Chart->clearGraphs();
    ui->Chart->addGraph();
    ui->Chart->graph(0)->setData(actions, money);

    qDebug() << money << actions;

    ui->Chart->xAxis->setLabel("Действия");
    ui->Chart->yAxis->setLabel("Деньги");
    ui->Chart->xAxis->setRange(0, money.size() - 1); // Adjust X-axis range
    ui->Chart->yAxis->setRange(*std::min_element(money.begin(), money.end()), *std::max_element(money.begin(), money.end())); // Adjust Y-axis range
    ui->Chart->replot();
}


void MainWindow::on_pushButton_del_clicked()
{
    window_del->show();
    window_del->draw_table();
}

void MainWindow::calculations()
{
    QVector<double> moneyValues;
    query = new QSqlQuery(db);

    if (db.open())
    {
        if(query->exec("SELECT Money, Action FROM Actions"))
        {
            while (query->next())
            {
                double money = query->value(0).toDouble();
                QString action = query->value(1).toString(); // Assuming action is stored in column 1

                if (action == "Расходы")
                {
                    money *= -1; // Multiply money by -1 for expenses
                }
                moneyValues.push_back(money);
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
    for (int i = 0; i < moneyValues.size(); i++)
    {
        balance += moneyValues[i];
        wallet += balance;
    }

    ui->label_balance->setNum(balance);

    draw_graph(wallet);
}
