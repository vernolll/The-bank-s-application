#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");

    add_Database();

    ui->stackedWidget->setCurrentWidget(ui->page_main);

    income_and_expenses *inc_exp = new income_and_expenses(ui, db, this);
    Currencies *curs = new Currencies(ui, this);

    connect(this, SIGNAL(on_pushButton_balance_clicked()), inc_exp, SLOT(open_inc_exp()));
    connect(this, SIGNAL(on_pushButton_add_clicked()), inc_exp, SLOT(open_add_action()));
    connect(this, SIGNAL(on_pushButton_del_clicked()), inc_exp, SLOT(open_del_action()));
    connect(this, SIGNAL(on_pushButton_cur_clicked()), curs, SLOT(open_currencies()));
    connect(this, SIGNAL(on_pushButton_back_2_clicked()), inc_exp, SLOT(back()));
    connect(this, SIGNAL(on_pushButton_back_clicked()), curs, SLOT(back()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete inc_exp;
    delete curs;
}


bool MainWindow::add_Database()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("finance");
    db.setUserName("postgres");
    db.setPassword("2k0a0r6AS");

    if (!db.open())
    {
        qDebug() << "Error: Unable to open database" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}


