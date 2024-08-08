#ifndef DEL_ACTION_H
#define DEL_ACTION_H

#include <QDialog>
#include <QTableView>
#include <QSqlTableModel>
#include "mainwindow.h"


namespace Ui {
class Del_action;
}

class Del_action : public QDialog
{
    Q_OBJECT

public:
    explicit Del_action(QWidget *parent = nullptr);
    void draw_table();
    ~Del_action();

private slots:
    void on_pushButton_delete_clicked();


private:
    Ui::Del_action *ui;
    QSqlTableModel *model;
};

#endif // DEL_ACTION_H
