#include "add_action.h"
#include "ui_add_action.h"

add_action::add_action(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::add_action)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");
}


add_action::~add_action()
{
    delete ui;
}


bool add_action::add_Database()
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
    query = new QSqlQuery(db);
    query->prepare("SELECT Action, Category FROM Categories WHERE action = 'Доходы'");
    query->exec();
    while (query->next())
    {
        QString type = query->value(1).toString();
        ui->comboBox_category->addItem(type);
    }
    delete query;
    return true;
}

void add_action::on_comboBox_action_currentIndexChanged(const QString &arg1)
{
    query = new QSqlQuery(db);
    ui->comboBox_category->clear();

    query->prepare("SELECT Action, Category FROM Categories WHERE action = :act");
    query->bindValue(":act", arg1);
    query->exec();

    while (query->next())
    {
        QString type = query->value(1).toString();
        ui->comboBox_category->addItem(type);
    }
    delete query;
}

void add_action::on_pushButton_add_clicked()
{
    double money = ui->lineEdit_money->text().toDouble();
    QString action = ui->comboBox_action->currentText();
    QString category = ui->comboBox_category->currentText();

    if (money != 0 && action != NULL && category != NULL)
    {
        query = new QSqlQuery(db);
        query->prepare("INSERT INTO Actions (Action, Category, Money) VALUES (:act, :cat, :mon)");
        query->bindValue(":act", action);
        query->bindValue(":cat", category);
        query->bindValue(":mon", money);

        if (query->exec())
        {
            qDebug() << "Insertion successful!";
            calculations();
            this->close();

        } else
        {
            qDebug() << "Insertion failed:" << query->lastError().text();
        }
    }
    delete query;
}


void add_action::calculations()
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

    QVector<double> wallet;
    double balance = 0;
    for (int i = 0; i < moneyValues.size(); i++)
    {
        balance += moneyValues[i];
        wallet += balance;
    }

    emit graph(wallet);
    delete query;
}
