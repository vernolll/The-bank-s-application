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

    connect(window_add, SIGNAL(graph(QVector<double>)), this, SLOT(draw_graph(QVector<double>)));
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
    ui->stackedWidget->setCurrentWidget(ui->page_expenses_income);
    window_add->add_Database();
    window_add->calculations();
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
