#ifndef ADD_ACTION_H
#define ADD_ACTION_H

#include <QDialog>
#include <QDebug>
#include <QDate>
#include "include/autorization.h"


namespace Ui
{
class add_action;
}

class add_action : public QDialog
{
    Q_OBJECT

public:
    explicit add_action(QWidget *parent = nullptr);
    bool connect_info();
    ~add_action();

signals:
    void calc();
    void on_pushButton_add_clicked();
    void on_comboBox_action_currentIndexChanged(int index);

private slots:
    void add_info();
    void filling_comboBox(int index);

private:
    Ui::add_action *ui;
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // ADD_ACTION_H
