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
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.constData(), data.size());
    SHA256_Final(hash, &sha256);

    QString hashed;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        hashed.append(QString("%1").arg((int)hash[i], 0, 16));
    }

    return hashed;
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


void Autorization::new_person()
{
    QSqlQuery query(db);
    if(!ui->lineEdit_login_2->text().isEmpty() && !ui->lineEdit_password_2->text().isEmpty())
    {
        qDebug() << "4";
        QString login = ui->lineEdit_login_2->text();
        QString password = ui->lineEdit_password_2->text();

        if (!isPasswordStrong(password))
        {
            qDebug() << "1";
            QMessageBox::warning(nullptr, "Ненадежный пароль", "Пароль должен содержать не менее 8 символов и состоять из прописных и строчных букв, цифр и специальных символов.", QMessageBox::Ok);
            qDebug() << "Ненадежный пароль";
            return;
        }

        query.prepare("INSERT INTO Users (username, password) VALUES (:log, :pas)");
        query.bindValue(":log", login);
        query.bindValue(":pas", sha256(password));

        if (!query.exec())
        {
            qDebug() << "1";
            qDebug() << "Error executing query:" << query.lastError().text();
        }
        else
        {
            qDebug() << "3";
            qDebug() << "User registered successfully!";

            qDebug() << sha256(password);



            ui->stackedWidget->setCurrentWidget(ui->page_autorization);
        }
    }
}
