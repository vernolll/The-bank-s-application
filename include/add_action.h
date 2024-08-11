#ifndef ADD_ACTION_H
#define ADD_ACTION_H

#include <QDialog>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDate>


namespace Ui {
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
    void on_comboBox_action_currentIndexChanged(const QString &arg1);

private slots:
    void add_info();
    void filling_comboBox(const QString &arg1);

private:
    Ui::add_action *ui;
    QSqlDatabase db;
};

#endif // ADD_ACTION_H
