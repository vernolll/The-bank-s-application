#include "include/add_rate.h"
#include "ui_add_rate.h"

Add_rate::Add_rate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Add_rate)
{
    ui->setupUi(this);
}

Add_rate::~Add_rate()
{
    delete ui;
}


void Add_rate::fixed()
{
    ui->stackedWidget->setCurrentWidget(ui->page_fix);
}

void Add_rate::changed()
{
    ui->stackedWidget->setCurrentWidget(ui->page_change);
}
