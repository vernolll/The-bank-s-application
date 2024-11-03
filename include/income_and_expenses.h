#ifndef INCOME_AND_EXPENSES_H
#define INCOME_AND_EXPENSES_H

#include <QObject>
#include <QWidget>
#include <QDate>
#include <QtCharts>
#include <QChartView>
#include <algorithm>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

#include "ui_mainwindow.h"
#include "include/add_action.h"
#include "include/del_action.h"
#include "include/autorization.h"


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
    explicit income_and_expenses(Ui::MainWindow *ui, QSqlDatabase database, QObject *parent = nullptr);
    ~income_and_expenses();

public slots:
    void open_inc_exp();
    void back();
    void open_add_action();
    void open_del_action();
    void calculations();

private:
    Ui::MainWindow *ui;
    add_action *window_add;
    Del_action *window_del;
    QSqlDatabase db;

    void draw_diagrams(QVector<QString> categories_inc, QVector<double> money_inc, QVector<QString> categories_exp, QVector<double> money_exp);
    void draw_graph(QVector<QPair<QDate, double>> wallet);

};

#endif // INCOME_AND_EXPENSES_H
