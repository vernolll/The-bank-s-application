#include "del_action.h"
#include "ui_del_action.h"

Del_action::Del_action(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Del_action)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");
}


Del_action::~Del_action()
{
    delete ui;
}

void Del_action::draw_table()
{
    model = new QSqlTableModel();
    model->setTable("actions");
    model->select();

    ui->tableView->setModel(model);
    ui->tableView->show();
}


void Del_action::on_pushButton_delete_clicked()
{

}
