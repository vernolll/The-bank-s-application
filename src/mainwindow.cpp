#include "include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");

    add_Database();

    ui->stackedWidget->setCurrentWidget(ui->page_credit);

    inc_exp = new income_and_expenses(ui, db, this);
    curs = new Currencies(ui, this);
    conv = new Converter(ui, this);
    autoriz = new Autorization(ui, this);
    news = new News(ui, this);
    credit = new Credit(ui, this);

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
    connect(this, SIGNAL(on_pushButton_login_clicked()), autoriz, SLOT(autorizat()));
    connect(this, SIGNAL(on_pushButton_regirstr_clicked()), autoriz, SLOT(registration()));
    connect(this, SIGNAL(on_pushButton_regirstr_2_clicked()), autoriz, SLOT(new_person()));
    connect(this, SIGNAL(on_pushButton_news_clicked()), news, SLOT(open_news()));
    connect(this, SIGNAL(on_pushButton_back_4_clicked()), news, SLOT(back()));
    connect(this, SIGNAL(on_tableView_news_clicked(const QModelIndex &)), news, SLOT(open_site(const QModelIndex &)));
    connect(this, SIGNAL(on_pushButton_credit_clicked()), credit, SLOT(open_calculator()));
    connect(this, SIGNAL(on_pushButton_back_5_clicked()), credit, SLOT(back()));
    connect(this, SIGNAL(on_pushButton_count_2_clicked()), credit, SLOT(get_info()));
    connect(this, SIGNAL(on_pushButton_back_6_clicked()), credit, SLOT(back_to_credit()));
}


MainWindow::~MainWindow()
{
    delete ui;
    delete inc_exp;
    delete curs;
    delete conv;
    delete autoriz;
    delete news;
}


bool MainWindow::add_Database()
{
    db = QSqlDatabase::addDatabase("QPSQL");
     db.setHostName("localhost");
    db.setDatabaseName("finance");
    db.setUserName("postgres");
    QString password = qgetenv("PASSWORD.ENV");
    db.setPassword(password);

    if (!db.open())
    {
        qDebug() << "Error: Unable to open database" << db.lastError().text();
        db.setDatabaseName("postgres");
        if (db.open())
        {
            QSqlQuery query(db);
            query.exec("CREATE DATABASE finance");
            qDebug() << "Database 'finance' created successfully.";
        }
        else
        {
            qDebug() << "Error: Unable to connect to 'postgres' database";
            return false;
        }

        db.setDatabaseName("finance");
        if (!db.open())
        {
            qDebug() << "Error: Unable to open database 'finance'" << db.lastError().text();
            return false;
        }
    }

    qDebug() << "Database connected successfully.";
    return true;
}
