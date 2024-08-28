#ifndef ADD_RATE_H
#define ADD_RATE_H

#include <QDialog>
#include <QStackedWidget>
#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>
#include <cmath>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include "include/credit.h"

class Credit;

namespace Ui
{
class Add_rate;
}

class Add_rate : public QDialog
{
    Q_OBJECT

public:
    explicit Add_rate(QWidget *parent = nullptr, Credit *credit = nullptr);
    void open_fixed();
    void open_changed();
    ~Add_rate();

signals:
    void on_pushButton_confirm_clicked();
    void on_pushButton_add_clicked();
    void on_pushButton_confirm_2_clicked();

private slots:
    void culc_fixed();
    void add_changed();
    void culc_changed();

private:
    Ui::Add_rate *ui;
    QSqlTableModel *model;
    Credit *credit;
    QSqlDatabase db;

    void draw_table();
    void calculation();
};

#endif // ADD_RATE_H
