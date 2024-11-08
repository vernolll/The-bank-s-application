#include "include/add_action.h"
#include "ui_add_action.h"


add_action::add_action(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_action)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");

    connect(this, SIGNAL(on_pushButton_add_clicked()), this, SLOT(add_info()));
    connect(ui->comboBox_action, &QComboBox::currentIndexChanged, this, &add_action::filling_comboBox);
}


add_action::~add_action()
{
    delete ui;
}


bool add_action::connect_info()
{
    QSqlQuery query(db);

    query.prepare("SELECT name FROM sqlite_master WHERE type='table' AND name='Categories'");
    query.exec();

    if (!query.next())
    {
        QVector<QPair<QString, QString>> categories =
        {
            qMakePair<QString, QString>("Доходы", "Зарплата"),
            qMakePair<QString, QString>("Доходы", "Перевод"),
            qMakePair<QString, QString>("Доходы", "Пенсия"),
            qMakePair<QString, QString>("Доходы", "Гонорар"),
            qMakePair<QString, QString>("Доходы", "Премия"),
            qMakePair<QString, QString>("Доходы", "Пособие"),
            qMakePair<QString, QString>("Расходы", "Продукты"),
            qMakePair<QString, QString>("Расходы", "Развлечения"),
            qMakePair<QString, QString>("Расходы", "Здоровье"),
            qMakePair<QString, QString>("Расходы", "Образование"),
            qMakePair<QString, QString>("Расходы", "Транспорт"),
            qMakePair<QString, QString>("Расходы", "Налоги"),
            qMakePair<QString, QString>("Расходы", "Одежда"),
            qMakePair<QString, QString>("Расходы", "Автомобиль"),
            qMakePair<QString, QString>("Расходы", "Питомцы"),
            qMakePair<QString, QString>("Расходы", "Страхование"),
            qMakePair<QString, QString>("Расходы", "Красота"),
            qMakePair<QString, QString>("Расходы", "Ребенок"),
            qMakePair<QString, QString>("Расходы", "Покупка")
        };

        for (const auto &pair : categories)
        {
            query.exec("CREATE TABLE Categories (action TEXT, category TEXT)");
            query.prepare("INSERT INTO Categories (action, category) VALUES (:action, :category)");
            query.bindValue(":action", pair.first);
            query.bindValue(":category", pair.second);
            query.exec();
        }
    }

    query.prepare("SELECT Action, Category FROM Categories WHERE action = 'Доходы'");
    query.exec();
    while (query.next())
    {
        QString type = query.value(1).toString();
        ui->comboBox_category->addItem(type);
    }
    return true;
}


void add_action::filling_comboBox(int index)
{
    if (!db.isOpen())
    {
        qDebug() << "Database is not open.";
        return;
    }

    QSqlQuery query(db);
    ui->comboBox_category->clear();

    QString action = ui->comboBox_action->itemText(index);

    query.prepare("SELECT Action, Category FROM Categories WHERE Action = :act");
    query.bindValue(":act", action);

    if (!query.exec())
    {
        qDebug() << "Query execution failed:" << query.lastError().text();
        return;
    }

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
        query.prepare("INSERT INTO Actions (Action, Category, Money, Date, username) VALUES (:act, :cat, :mon, :day, :user)");
        query.bindValue(":act", action);
        query.bindValue(":cat", category);
        query.bindValue(":mon", money);
        query.bindValue(":day", date);
        query.bindValue(":user", user);

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
