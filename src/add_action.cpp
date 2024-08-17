#include "include/add_action.h"
#include "ui_add_action.h"

add_action::add_action(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_action)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");

    connect(this, SIGNAL(on_pushButton_add_clicked()), this, SLOT(add_info()));
    connect(this, SIGNAL(on_comboBox_action_currentIndexChanged(const QString &)), this, SLOT(filling_comboBox(const QString &)));
}


add_action::~add_action()
{
    delete ui;
}


bool add_action::connect_info()
{
    QSqlQuery query(db);
    query.prepare("SELECT Action, Category FROM Categories WHERE action = 'Доходы'");
    query.exec();
    while (query.next())
    {
        QString type = query.value(1).toString();
        ui->comboBox_category->addItem(type);
    }
    return true;
}


void add_action::filling_comboBox(const QString &arg1)
{
    QSqlQuery query(db);

    ui->comboBox_category->clear();

    query.prepare("SELECT Action, Category FROM Categories WHERE action = :act");
    query.bindValue(":act", arg1);
    query.exec();

    while (query.next())
    {
        QString type = query.value(1).toString();
        ui->comboBox_category->addItem(type);
    }
}


void add_action::add_info()
{
    QDate date = ui->dateEdit->date();
    double money = ui->lineEdit_money->text().toDouble();
    QString action = ui->comboBox_action->currentText();
    QString category = ui->comboBox_category->currentText();

    QSqlQuery query(db);

    if (money != 0 && !action.isEmpty() && !category.isEmpty())
    {
        query.prepare("INSERT INTO Actions (Action, Category, Money, Date) VALUES (:act, :cat, :mon, :day)");
        query.bindValue(":act", action);
        query.bindValue(":cat", category);
        query.bindValue(":mon", money);
        query.bindValue(":day", date);

        if (query.exec())
        {
            qDebug() << "Insertion successful!";
            emit calc();
            this->close();
        }
        else
        {
            qDebug() << "Insertion failed:" << query.lastError().text();
        }
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Заполните поля правильно.");
    }
}
