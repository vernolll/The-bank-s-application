#include "include/credit.h"



Credit::Credit(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui)
{
    model = new QSqlTableModel();
    add_rate = new Add_rate(nullptr, this);
}


Credit::~Credit()
{
    add_rate = nullptr;
    delete model;
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

    if(money != 0.0 && time != 0 && !day.isNull() && !rate.isEmpty() && !payment.isEmpty() && !y_or_m.isEmpty())
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


void Credit::back_to_credit()
{
    ui->stackedWidget->setCurrentWidget(ui->page_credit);
}


void Credit::to_report()
{
    ui->stackedWidget->setCurrentWidget(ui->page_report);

    model->setTable("payment_details");

    model->setHeaderData(0, Qt::Horizontal, "Дата платежа");
    model->setHeaderData(1, Qt::Horizontal, "До %");
    model->setHeaderData(2, Qt::Horizontal, "После %");
    model->setHeaderData(3, Qt::Horizontal, "Платеж");
    model->setHeaderData(4, Qt::Horizontal, "После платежа");

    model->select();

    ui->tableView_credit->setModel(model);
    ui->tableView_credit->setColumnWidth(4, 200);
    CustomDelegate *delegate = new CustomDelegate();
    ui->tableView_credit->setItemDelegateForColumn(4, delegate);
    ui->tableView_credit->setItemDelegateForColumn(3, delegate);
    ui->tableView_credit->setItemDelegateForColumn(2, delegate);
    ui->tableView_credit->setItemDelegateForColumn(1, delegate);

    ui->tableView_credit->show();

    double average = 0.0, overpay = 0.0, money;
    QVector<double> pay;
    QSqlQuery query;
    query.exec("SELECT payment FROM payment_details");
    while(query.next())
    {
        pay += query.value(0).toDouble();
    }

    for(int i = 0; i < pay.size(); i++)
    {
        average += pay[i];
    }

    query.exec("SELECT money FROM credit");
    while(query.next())
    {
        money = query.value(0).toDouble();
    }
    overpay = average - money;
    average /= pay.size();

    QString averageText = QString::number(average, 'f', 2);
    QString overpayText = QString::number(overpay, 'f', 2);

    ui->label_average->setText(averageText);
    ui->label_overpayment->setText(overpayText);
}
