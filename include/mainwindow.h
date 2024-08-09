#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "include/income_and_expenses.h"


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

signals:
    void on_pushButton_del_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_add_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    income_and_expenses *inc_exp;

};
#endif // MAINWINDOW_H
