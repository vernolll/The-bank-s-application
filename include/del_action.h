#ifndef DEL_ACTION_H
#define DEL_ACTION_H

#include <QDialog>
#include <QTableView>
#include <QSqlTableModel>

namespace Ui
{
class Del_action;
}

class Del_action : public QDialog
{
    Q_OBJECT

public:
    explicit Del_action(QWidget *parent = nullptr);
    void draw_table();
    ~Del_action();

signals:
    void calc();
    void on_pushButton_delete_clicked();
    void on_tableView_clicked(const QModelIndex &index);

private slots:
    void delete_row();
    void get_index(const QModelIndex &index);

private:
    Ui::Del_action *ui;
    QSqlTableModel *model;
    int row;
};

#endif // DEL_ACTION_H
