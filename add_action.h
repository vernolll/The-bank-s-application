#ifndef ADD_ACTION_H
#define ADD_ACTION_H

#include <QDialog>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QMessageBox>


class MainWindow;

namespace Ui {
class add_action;
}

class add_action : public QDialog
{
    Q_OBJECT

public:
    explicit add_action(QWidget *parent = nullptr);
    bool add_Database();
    void calculations();
    ~add_action();

signals:
    void window_add();

    void graph(QVector<double> money);

private slots:
    void on_comboBox_action_currentIndexChanged(const QString &arg1);

    void on_pushButton_add_clicked();


private:
    Ui::add_action *ui;
    QSqlDatabase db;
    QSqlQuery *query;
};

#endif // ADD_ACTION_H
