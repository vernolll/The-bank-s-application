#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "income_and_expenses.h"


class income_and_expenses;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:



signals:
    void on_pushButton_del_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_add_clicked();

    void draw_graph(QVector<double> money);

    void calculations();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    income_and_expenses *inc_exp;

};
#endif // MAINWINDOW_H
