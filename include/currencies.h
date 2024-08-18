#ifndef CURRENCIES_H
#define CURRENCIES_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include "ui_mainwindow.h"
#include "include/mainwindow.h"
#include <QString>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <curl/curl.h>


namespace Ui
{
class MainWindow;
}


class Currencies : public QObject
{
    Q_OBJECT
public:
    Currencies(Ui::MainWindow *ui, QObject *parent = nullptr);
    void open_currencies();
    ~Currencies();

private slots:
    void open();
    void back();
    void fetchData();
    void parseData(const QString &content);
    void draw_table();
    void searching();

private:
    Ui::MainWindow *ui;
    CURL *curl;
    QSqlDatabase db;
    QSqlTableModel *model;
};

#endif // CURRENCIES_H
