#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "add_action.h"
#include "qcustomplot.h"
#include "del_action.h"

class add_action;
class Del_action;


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
    void on_pushButton_3_clicked();

    void on_pushButton_add_clicked();

    void draw_graph(QVector<double> money);

private slots:
    void on_pushButton_del_clicked();

private:
    Ui::MainWindow *ui;
    QStackedWidget *stackedWidget;
    add_action *window_add;
    Del_action *window_del;

};
#endif // MAINWINDOW_H
