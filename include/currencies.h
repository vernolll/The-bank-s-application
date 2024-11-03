#ifndef CURRENCIES_H
#define CURRENCIES_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include <QString>
#include <QTableView>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>

#include "ui_mainwindow.h"


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
    void searching();

private:
    Ui::MainWindow *ui;
    //CURL *curl;
    QNetworkAccessManager *networkManager;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlQueryModel *searchModel;

    void fetchData(QNetworkReply *reply);
    void parseData(const QString &content);
    void draw_table();
};

#endif // CURRENCIES_H
