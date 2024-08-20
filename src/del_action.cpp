#include "include/del_action.h"
#include "ui_del_action.h"


Del_action::Del_action(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Del_action),
    model(nullptr)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");

    connect(this, SIGNAL(on_pushButton_delete_clicked()), this, SLOT(delete_row()));
    connect(this, SIGNAL( on_tableView_clicked(const QModelIndex &)), this, SLOT(get_index(const QModelIndex &)));
}


Del_action::~Del_action()
{
    delete ui;
    delete model;
}


void Del_action::draw_table()
{
    model = new QSqlTableModel();
    model->setTable("actions");
    model->setFilter("username = '" + user +"'");

    model->setHeaderData(0, Qt::Horizontal, "Дейтсвие");
    model->setHeaderData(1, Qt::Horizontal, "Категория");
    model->setHeaderData(2, Qt::Horizontal, "Деньги");
    model->setHeaderData(3, Qt::Horizontal, "Дата");

    model->select();

    ui->tableView->setModel(model);
    ui->tableView->setColumnWidth(3, 163);
    ui->tableView->hideColumn(4);
    ui->tableView->show();
}


void Del_action::delete_row()
{
    model->removeRow(row);
    emit calc();
    this->close();
}


void Del_action::get_index(const QModelIndex &index)
{
    row = index.row();
}
