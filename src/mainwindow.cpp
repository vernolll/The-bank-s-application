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

    inc_exp = new income_and_expenses(ui, db, this);
    curs = new Currencies(ui, this);
    conv = new Converter(ui, this);

    connect(this, SIGNAL(on_pushButton_balance_clicked()), inc_exp, SLOT(open_inc_exp()));
    connect(this, SIGNAL(on_pushButton_add_clicked()), inc_exp, SLOT(open_add_action()));
    connect(this, SIGNAL(on_pushButton_del_clicked()), inc_exp, SLOT(open_del_action()));
    connect(this, SIGNAL(on_pushButton_cur_clicked()), curs, SLOT(open()));
    connect(this, SIGNAL(on_pushButton_back_2_clicked()), inc_exp, SLOT(back()));
    connect(this, SIGNAL(on_pushButton_back_clicked()), curs, SLOT(back()));
    connect(this, SIGNAL(on_pushButton_search_clicked()), curs, SLOT(searching()));
    connect(this, SIGNAL(on_pushButton_converter_clicked()), conv, SLOT(open_converter()));
    connect(this, SIGNAL(on_pushButton_back_3_clicked()), conv, SLOT(back_to_main()));
    connect(this, SIGNAL(on_pushButton_count_clicked()), conv, SLOT(get_info()));
}


MainWindow::~MainWindow()
{
    delete ui;
    delete inc_exp;
    delete curs;
    delete conv;
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
