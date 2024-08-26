#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSqlDatabase>
#include "include/income_and_expenses.h"
#include "include/currencies.h"
#include "include/converter.h"
#include "include/autorization.h"
#include "include/news.h"
#include "include/credit.h"


class income_and_expenses;
class Currencies;
class Converter;
class Autorization;
class News;
class Credit;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    bool add_Database();
    ~MainWindow();

signals:
    void on_pushButton_del_clicked();
    void on_pushButton_balance_clicked();
    void on_pushButton_add_clicked();
    void on_pushButton_cur_clicked();
    void on_pushButton_back_2_clicked();
    void on_pushButton_back_clicked();
    void on_pushButton_search_clicked();
    void on_pushButton_converter_clicked();
    void on_pushButton_back_3_clicked();
    void on_pushButton_count_clicked();
    void on_pushButton_login_clicked();
    void on_pushButton_regirstr_clicked();
    void on_pushButton_regirstr_2_clicked();
    void on_pushButton_news_clicked();
    void on_pushButton_back_4_clicked();
    void on_tableView_news_clicked(const QModelIndex &index);
    void on_pushButton_credit_clicked();
    void on_pushButton_back_5_clicked();
    void on_pushButton_count_2_clicked();
    void on_pushButton_back_6_clicked();

private:
    Ui::MainWindow *ui;
    income_and_expenses *inc_exp;
    Currencies *curs;
    Converter *conv;
    Autorization *autoriz;
    News *news;
    Credit *credit;
    QSqlDatabase db;
};
#endif // MAINWINDOW_H
