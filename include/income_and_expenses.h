#ifndef INCOME_AND_EXPENSES_H
#define INCOME_AND_EXPENSES_H

#include <QObject>
#include <QWidget>
#include "ui_mainwindow.h"
#include "include/add_action.h"
#include "include/qcustomplot.h"
#include "include/del_action.h"


class add_action;
class Del_action;


namespace Ui
{
class MainWindow;
}


class income_and_expenses : public QObject
{
    Q_OBJECT
public:
    explicit income_and_expenses(Ui::MainWindow *ui, QObject *parent = nullptr);
    bool add_Database();
    ~income_and_expenses();

public slots:
    void on_pushButton_3_clicked();
    void on_pushButton_add_clicked();
    void draw_graph(QVector<double> money);
    void on_pushButton_del_clicked();
    void calculations();


private:
    Ui::MainWindow *ui;
    add_action *window_add;
    Del_action *window_del;
    QSqlQuery *query;
    QSqlDatabase db;
};

#endif // INCOME_AND_EXPENSES_H
