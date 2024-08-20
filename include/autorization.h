#ifndef AUTORIZATION_H
#define AUTORIZATION_H


#include <QObject>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include "ui_mainwindow.h"


extern QString user;


namespace Ui
{
class MainWindow;
}


class Autorization : public QObject
{
    Q_OBJECT
public:
    explicit Autorization(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~Autorization();

private slots:
    void registration();
    void autorizat();
    void new_person();


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    bool isPasswordStrong(const QString &password);
    QString sha256(const QString& str);
};

#endif // AUTORIZATION_H
