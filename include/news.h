#ifndef NEWS_H
#define NEWS_H


#include <QObject>
#include <QWidget>
#include "ui_mainwindow.h"
#include <curl/curl.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>


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

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;

    void draw_table();

};

#endif // NEWS_H
