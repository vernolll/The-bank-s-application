#ifndef CREDIT_H
#define CREDIT_H

#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include <QSqlDatabase>
#include "ui_mainwindow.h"
#include "include/add_rate.h"
#include <QSqlTableModel>


class Add_rate;


namespace Ui
{
class MainWindow;
}

class Credit : public QObject
{
    Q_OBJECT
public:
    explicit Credit(Ui::MainWindow *ui, QObject *parent = nullptr);
    void to_report();
    ~Credit();

private slots:
    void open_calculator();
    void back();
    void get_info();
    void back_to_credit();

signals:
    void credit();

private:
    Ui::MainWindow *ui;
    Add_rate *add_rate;
    QSqlTableModel *model;
    QSqlDatabase db;
};


#endif // CREDIT_H
