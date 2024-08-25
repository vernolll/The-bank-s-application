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
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS rates");
    query.exec("DROP TABLE IF EXISTS credit");

    double money = ui->lineEdit_money_credit->text().toDouble();
    int time = ui->lineEdit_date->text().toInt();
    QDate day = ui->dateEdit_date->date();
    QString rate = ui->comboBox_rate->currentText();
    QString payment = ui->comboBox_payments->currentText();
    QString y_or_m = ui->comboBox_month->currentText();

    if(money != NULL && time != NULL && !day.isNull() && rate != NULL && payment != NULL && y_or_m != NULL)
    {
        query.exec("CREATE TABLE credit (money REAL, time INT, year_or_month TEXT, date DATE, payment TEXT, type TEXT)");
        query.prepare("INSERT INTO credit (money, time, year_or_month, date, payment, type) VALUES (:money, :time, :year_or_month, :date, :payment, :type)");
        query.bindValue(":money", money);
        query.bindValue(":time", time);
        query.bindValue(":year_or_month", y_or_m);
        query.bindValue(":date", day);
        query.bindValue(":payment", payment);
        query.bindValue(":type", rate);

        if(!query.exec())
        {
            qDebug() << "error inserting!";
        }

        add_rate->open();

        if (rate == "Фиксированная")
        {
            add_rate->open_fixed();
        }
        else
        {
            add_rate->open_changed();
        }
    }
    else
    {
        QMessageBox::warning(nullptr, "Ошибка", "Заполните все поля правильно.");
    }
}
