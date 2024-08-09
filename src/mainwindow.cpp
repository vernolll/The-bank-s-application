#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");

    stackedWidget = ui->stackedWidget;

    ui->stackedWidget->setCurrentWidget(ui->page);

    inc_exp = new income_and_expenses(ui, this);

    connect(this, SIGNAL(on_pushButton_3_clicked()), inc_exp, SLOT(open_inc_exp()));
    connect(this, SIGNAL(on_pushButton_add_clicked()), inc_exp, SLOT(open_add_action()));
    connect(this, SIGNAL(on_pushButton_del_clicked()), inc_exp, SLOT(open_del_action()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete stackedWidget;
}
