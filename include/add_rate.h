#ifndef ADD_RATE_H
#define ADD_RATE_H

#include <QDialog>
#include <QStackedWidget>

namespace Ui {
class Add_rate;
}

class Add_rate : public QDialog
{
    Q_OBJECT

public:
    explicit Add_rate(QWidget *parent = nullptr);
    void fixed();
    void changed();
    ~Add_rate();

private:
    Ui::Add_rate *ui;
};

#endif // ADD_RATE_H
