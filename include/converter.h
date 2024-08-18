#ifndef CONVERTER_H
#define CONVERTER_H

#include <QObject>
#include <QWidget>
#include "ui_mainwindow.h"
#include "include/currencies.h"


class Currencies;


namespace Ui
{
class MainWindow;
}


class Converter : public QObject
{
    Q_OBJECT
public:
    Converter(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~Converter();

private slots:
    void open_converter();
    void back_to_main();
    void load_info();
    void get_info();
    void counting(double money, double course1, double unit1, double course2, double unit2);

private:
    Ui::MainWindow *ui;
    Currencies *curr;
    QSqlDatabase db;

};

#endif // CONVERTER_H
