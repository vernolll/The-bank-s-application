#ifndef CURRENCIES_H
#define CURRENCIES_H

#include <QObject>
#include <QWidget>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDebug>
#include <QSslSocket>
#include "ui_mainwindow.h"
#include "include/mainwindow.h"
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
    ~Currencies();

private slots:
    void open_currencies();
    void fetchData();
    void parseData(const QString &content);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    CURL *curl;
};

#endif // CURRENCIES_H
