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
    explicit Converter(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~Converter();

private slots:
    void open_converter();
    void back_to_main();
    void get_info();


private:
    Ui::MainWindow *ui;
    Currencies *curr;
    QSqlDatabase db;

    void load_info();
    void counting(double money, double course1, double unit1, double course2, double unit2);

};

#endif // CONVERTER_H
