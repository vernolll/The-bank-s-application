#ifndef CREDIT_H
#define CREDIT_H

#include <QObject>
#include <QWidget>
#include "ui_mainwindow.h"
#include "include/add_rate.h"


class Add_rate;


namespace Ui
{
class MainWindow;
}

class Credit : public QObject
{
    Q_OBJECT
public:
    explicit Credit(Ui::MainWindow *ui, QObject *parent = nullptr);
    ~Credit();

private slots:
    void open_calculator();
    void back();
    void get_info();

private:
    Ui::MainWindow *ui;
    Add_rate *add_rate;
};

#endif // CREDIT_H
