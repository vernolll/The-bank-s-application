#include "include/credit.h"

Credit::Credit(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui)
{
    add_rate = new Add_rate();
}


Credit::~Credit()
{
    delete add_rate;
}


void Credit::open_calculator()
{
    ui->stackedWidget->setCurrentWidget(ui->page_credit);
}


void Credit::back()
{
    ui->stackedWidget->setCurrentWidget(ui->page_main);
}


void Credit::get_info()
{
    double money = ui->lineEdit_money_credit->text().toDouble();
    int time = ui->lineEdit_date->text().toInt();
    QDate day = ui->dateEdit_date->date();
    QString rate = ui->comboBox_rate->currentText();
    QString payment = ui->comboBox_payments->currentText();

    add_rate->open();

    if (rate == "Фиксированная")
    {
        add_rate->fixed();
    }
    else
    {
        add_rate->changed();
    }
}
