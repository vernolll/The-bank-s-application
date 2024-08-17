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
#include <curl/curl.h>

class MainWindow;

namespace Ui
{
class MainWindow;
}


class Currencies : public QObject
{
    Q_OBJECT
public:
    Currencies(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~Currencies();

private slots:
    void open_currencies();
    void back();
    void fetchData();
    void parseData(const QString &content);
    void draw_table();

private:
    Ui::MainWindow *ui;
    CURL *curl;
    QSqlDatabase db;
    QSqlQuery query;
    QSqlTableModel *model;
};

#endif // CURRENCIES_H
