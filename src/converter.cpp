#include "include/converter.h"

Converter::Converter(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui)
{
    curr = new Currencies(ui, this);
}


Converter::~Converter()
{
    delete curr;
}


void Converter::open_converter()
{
    ui->stackedWidget->setCurrentWidget(ui->page_converter);
    curr->open_currencies();
    load_info();
}


void Converter::back_to_main()
{
    ui->stackedWidget->setCurrentWidget(ui->page_main);
}


void Converter::load_info()
{
    QSqlQuery query(db);

    ui->comboBox->clear();
    ui->comboBox_2->clear();

    query.prepare("SELECT letcode FROM exchangerate");
    query.exec();
    while (query.next())
    {
        QString letcode = query.value(0).toString();
        ui->comboBox->addItem(letcode);
        ui->comboBox_2->addItem(letcode);
    }
}


void Converter::get_info()
{
    QString curr1 = ui->comboBox->currentText();
    QString curr2 = ui->comboBox_2->currentText();
    double course1, unit1, course2, unit2;


    QSqlQuery query(db);
    query.prepare("SELECT letcode, unit, course FROM exchangerate WHERE letcode = :cur1");
    query.bindValue(":cur1", curr1);
    query.exec();
    while (query.next())
    {
        course1 = query.value(1).toDouble();
        unit1 = query.value(2).toDouble();
    }

    QSqlQuery query1(db);
    query1.prepare("SELECT letcode, unit, course FROM exchangerate WHERE letcode = :cur2");
    query1.bindValue(":cur2", curr2);
    query1.exec();
    while (query1.next())
    {
        course2= query1.value(1).toDouble();
        unit2 = query1.value(2).toDouble();
    }

    double money = ui->lineEdit->text().toDouble();

    counting(money, course1, unit1, course2, unit2);
}


void Converter::counting(double money, double course1, double unit1, double course2, double unit2)
{
    double to_rub1 = course1 / unit1;
    double to_rub2 = course2 / unit2;

    double result = money * to_rub2 / to_rub1;
    QString res = QString::number(result);

    ui->lineEdit_2->setText(res);
}
