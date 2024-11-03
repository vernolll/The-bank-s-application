#include "include/autorization.h"

QString user = NULL;

Autorization::Autorization(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui)
{

}


Autorization::~Autorization()
{

}


void Autorization::registration()
{
    ui->stackedWidget->setCurrentWidget(ui->page_registration);
}


bool Autorization::isPasswordStrong(const QString &password)
{
    if (password.length() < 8)
    {
        return false; // Minimum length
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (const QChar &c : password)
    {
        if (c.isUpper())
        {
            hasUpper = true;
        }
        else if (c.isLower())
        {
            hasLower = true;
        }
        else if (c.isDigit())
        {
            hasDigit = true;
        }
        else if (!c.isSpace() && !c.isLetterOrNumber())
        {
            hasSpecial = true;
        }
    }

    return hasUpper && hasLower && hasDigit && hasSpecial;
}


QString Autorization::sha256(const QString& str)
{
    QByteArray data = str.toUtf8();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    return hash.toHex();
}


void Autorization::autorizat()
{
    QSqlQuery query(db);
    query.prepare("CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)");

    QString username = ui->lineEdit_login->text();
    QString password = ui->lineEdit_password->text();

    QString pass = sha256(password);

    query.prepare("SELECT * FROM users WHERE username = :username AND password = :password");
    query.bindValue(":username", username);
    query.bindValue(":password", pass);
    query.exec();


    if (query.next())
    {
        qDebug() << "Authorization successful";
        user = username;
        ui->stackedWidget->setCurrentWidget(ui->page_main);
    }
    else
    {
        qDebug() << "Authorization failed";
        QMessageBox::critical(nullptr, "Ошибка!", "Неверный логин и/или пароль.");
    }
}


bool Autorization::check_if_exist(QString user, QString password)
{
    QSqlQuery query(db);
    query.prepare("CREATE TABLE IF NOT EXISTS users (username TEXT PRIMARY KEY, password TEXT)");
    query.prepare("SELECT * FROM users WHERE username = :user");
    query.bindValue(":user", user);
    if(query.exec() && query.next())
    {
        QMessageBox::warning(nullptr, "Ошибка", "Такой логин уже используется");
        return true;
    }

    query.prepare("SELECT * FROM users WHERE password = :password");
    query.bindValue(":password", password);
    if(query.exec() && query.next())
    {
        QMessageBox::warning(nullptr, "Ошибка", "Пароль занят.");
        return true;
    }
    return false;
}


void Autorization::new_person()
{
    QSqlQuery query(db);
    if(!ui->lineEdit_login_2->text().isEmpty() && !ui->lineEdit_password_2->text().isEmpty())
    {
        QString login = ui->lineEdit_login_2->text();
        QString password = ui->lineEdit_password_2->text();

        if (!isPasswordStrong(password))
        {
            QMessageBox::warning(nullptr, "Ненадежный пароль", "Пароль должен содержать не менее 8 символов и состоять из прописных и строчных букв, цифр и специальных символов.", QMessageBox::Ok);
            qDebug() << "Ненадежный пароль";
            return;
        }

        if(login.length() < 8)
        {
            QMessageBox::warning(nullptr, "Короткий логин", "Логин должен содержать не менее 8 символов.");
            return;
        }

        if(!check_if_exist(login, sha256(password)))
        {
            query.prepare("INSERT INTO Users (username, password) VALUES (:log, :pas)");
            query.bindValue(":log", login);
            query.bindValue(":pas", sha256(password));

            if (!query.exec())
            {
                qDebug() << "Error executing query:" << query.lastError().text();
            }
            else
            {
                qDebug() << "User registered successfully!";
                ui->stackedWidget->setCurrentWidget(ui->page_autorization);
            }
        }
    }
}
