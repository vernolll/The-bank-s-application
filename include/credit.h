#ifndef CREDIT_H
#define CREDIT_H

#include <QObject>
#include <QWidget>
#include <QMessageBox>
#include "ui_mainwindow.h"
#include "include/add_rate.h"
#include <QSqlTableModel>


class Add_rate;


class CustomDelegate : public QStyledItemDelegate
{
public:
    CustomDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QString displayText(const QVariant &value, const QLocale &locale) const override
    {
        if (value.type() == QVariant::Int || value.type() == QVariant::Double)
        {
            // Format the number with two decimal places and without grouping
            return locale.toString(value.toDouble(), 'f', 2);
        }
        return QStyledItemDelegate::displayText(value, locale);
    }
};


namespace Ui
{
class MainWindow;
}

class Credit : public QObject
{
    Q_OBJECT
public:
    explicit Credit(Ui::MainWindow *ui, QObject *parent = nullptr);
    void to_report();
    ~Credit();

private slots:
    void open_calculator();
    void back();
    void get_info();
    void back_to_credit();

signals:
    void credit();

private:
    Ui::MainWindow *ui;
    Add_rate *add_rate;
    QSqlTableModel *model;
};


#endif // CREDIT_H
