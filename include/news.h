#ifndef NEWS_H
#define NEWS_H


#include <QObject>
#include <QWidget>
#include <QSqlQuery>
#include <QSqlError>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QRegularExpression>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QDesktopServices>

#include "ui_mainwindow.h"


namespace Ui
{
class MainWindow;
}



class News : public QObject
{
    Q_OBJECT
public:
    explicit News(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~News();

private slots:
    void open_news();
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb);
    void back();
    void open_site(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    QNetworkAccessManager *networkManager;

    void draw_table();

};

#endif // NEWS_H
