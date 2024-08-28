#include "include/add_rate.h"
#include "ui_add_rate.h"

Add_rate::Add_rate(QWidget *parent, Credit *credit) :
    QDialog(parent),
    ui(new Ui::Add_rate),
    credit(credit)
{
    ui->setupUi(this);
    setWindowTitle("Мои финансы");

    model = new QSqlTableModel();

    connect(this, SIGNAL(on_pushButton_confirm_clicked()), this, SLOT(culc_fixed()));
    connect(this, SIGNAL(on_pushButton_add_clicked()), this, SLOT(add_changed()));
    connect(this, SIGNAL(on_pushButton_confirm_2_clicked()), this, SLOT(culc_changed()));
}

Add_rate::~Add_rate()
{
    delete ui;
    delete model;
    delete credit;
}


void Add_rate::open_fixed()
{
    ui->stackedWidget->setCurrentWidget(ui->page_fix);
}


void Add_rate::open_changed()
{
    ui->stackedWidget->setCurrentWidget(ui->page_change);
    draw_table();
}


void Add_rate::draw_table()
{
    model->setTable("rates");

    model->setHeaderData(1, Qt::Horizontal, "Дата");
    model->setHeaderData(2, Qt::Horizontal, "Процент");
    model->setHeaderData(3, Qt::Horizontal, "Срок");

    model->select();

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(0);
    ui->tableView->setColumnWidth(1, 120);
    ui->tableView->setColumnWidth(2, 120);
    ui->tableView->setColumnWidth(3, 120);
    ui->tableView->show();
}


void Add_rate::culc_fixed()
{
    double rate = ui->lineEdit_rate->text().toDouble();
    QString percent = ui->comboBox_persents_2->currentText();

    if(!percent.isEmpty() && rate != 0.0)
    {
        QSqlQuery query;
        query.exec("CREATE TABLE IF NOT EXISTS rates (type TEXT, date DATE, rate REAL, percent TEXT)");
        query.prepare("INSERT INTO rates (type, rate, percent) VALUES (:type, :rate, :percent)");
        query.bindValue(":type", "Фиксированная");
        query.bindValue(":rate", rate);
        query.bindValue(":percent", percent);
        query.exec();
        this->close();
        calculation();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля правильно.");
    }
}


void Add_rate::add_changed()
{
    QDate date = ui->dateEdit_date->date();
    double rate = ui->lineEdit_rate_2->text().toDouble();
    QString percent = ui->comboBox_persents->currentText();

    QSqlQuery query;

    QDate start_date, finish_date;
    QString info;
    int time;

    query.exec("SELECT time, year_or_month, date FROM credit");
    while(query.next())
    {
        time = query.value(0).toInt() - 1;
        info = query.value(1).toString();
        start_date = query.value(2).toDate();
    }

    if(info == "лет")
    {
        finish_date = start_date.addYears(time);
    }
    else
    {
        finish_date = start_date.addMonths(time);
    }

    if(date < start_date || date > finish_date)
    {
        QMessageBox::warning(this, "Ошибка", "Дата указана неверно.");
        return;
    }

    if(!date.isNull() && rate != 0.0 && !percent.isEmpty())
    {
        query.exec("CREATE TABLE IF NOT EXISTS rates (type TEXT, date DATE, rate REAL, percent TEXT)");
        query.prepare("INSERT INTO rates (type, date, rate, percent) VALUES (:type, :date, :rate, :percent)");
        query.bindValue(":type", "Изменяемая");
        query.bindValue(":date", date);
        query.bindValue(":rate", rate);
        query.bindValue(":percent", percent);
        query.exec();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля правильно.");
    }

    draw_table();
}

void Add_rate::culc_changed()
{
    if(ui->tableView->model()->rowCount() > 0)
    {
        this->close();
        calculation();
    }
    else
    {
        QMessageBox::warning(this, "Ошибка", "Заполните все поля правильно.");
    }
}


void Add_rate::calculation()
{
    double rate, money, payment;
    int time;
    QString month_or_year, percent, type, payment_type;
    QDate date;
    QVector<QString> rate_v;
    QVector<double> percent_v;
    QVector<QDate> date_v;
    double before, after;

    QSqlQuery query;

    query.exec("DROP TABLE IF EXISTS payment_details");
    query.exec("CREATE TABLE payment_details ("
                "date DATE, "
                "before DECIMAL(20, 7), "
                "after DECIMAL(20, 7), "
                "payment DECIMAL(20, 7),"
                "after_pay DECIMAL(20, 7))");

    query.exec("SELECT money, time, year_or_month, date, payment, type FROM credit");
    while(query.next())
    {
        money = query.value(0).toDouble();
        time = query.value(1).toInt();
        month_or_year = query.value(2).toString();
        date = query.value(3).toDate();
        payment_type = query.value(4).toString();
        type = query.value(5).toString();
    }

    if (type == "Фиксированная")
    {
        query.exec("SELECT rate, percent FROM rates");
        while(query.next())
        {
            rate = query.value(0).toDouble();
            percent = query.value(1).toString();
        }
    }
    else
    {
        query.exec("SELECT date, percent, rate FROM rates");
        while(query.next())
        {
            date_v += query.value(0).toDate();
            rate_v += query.value(1).toString();
            percent_v += query.value(2).toDouble();
        }
    }

    before = money;

    if(payment_type == "Аннуитетные")
    {
        if(type == "Фиксированная")
        {
            if (percent == "% в месяц")
            {
                rate = rate / 100;
            }
            else if (percent == "% в год")
            {
                rate = (rate / 100) / 12;
            }

            if(month_or_year == "лет")
            {
                time = time * 12;
            }

            if (rate > 0)
            {
                payment = (money * rate * pow(1 + rate, time)) / (pow(1 + rate, time) - 1);
            }
            else
            {
                payment = money / time;
            }

            for(int month = 0; month <= time - 1; month++)
            {
                after = before * (rate + 1);
                QSqlQuery insertQuery;
                insertQuery.prepare("INSERT INTO payment_details (date, before, after, payment, after_pay) VALUES (?, ?, ?, ?, ?)");
                insertQuery.addBindValue(date.addMonths(month));
                insertQuery.addBindValue(before);
                insertQuery.addBindValue(after);
                insertQuery.addBindValue(payment);
                before = after - payment;
                insertQuery.addBindValue(before);
                insertQuery.exec();
            }
        }
        else
        {
            double totalPayment = 0.0;

            for (int i = 0; i < percent_v.size(); i++)
            {
                double currentRate = percent_v[i];

                if (rate_v[i] == "% в месяц")
                {
                    currentRate /= 100;
                }
                else if (rate_v[i] == "% в год")
                {
                    currentRate = (currentRate / 100) / 12;
                }

                int duration = 0;
                if (i < percent_v.size() - 1)
                {
                    QDate startDate = date_v[i];
                    QDate endDate = date_v[i + 1];
                    duration = startDate.daysTo(endDate) / 30;
                }
                else
                {
                    QDate lastStartDate = date_v[i - 1];
                    QDate endDate = date_v[i];
                    duration = lastStartDate.daysTo(endDate) / 30;
                }

                double remainingBalance = money;
                double monthlyPayment = 0.0;

                monthlyPayment = (money * currentRate * pow(1 + currentRate, duration)) / (pow(1 + currentRate, duration) - 1);

                totalPayment += monthlyPayment;

                for (int month = 0; month < duration; month++)
                {
                    QSqlQuery insertQuery;
                    insertQuery.prepare("INSERT INTO payment_details (date, before, after, payment, after_pay) VALUES (?, ?, ?, ?, ?)");
                    insertQuery.addBindValue(date_v[i].addMonths(month));
                    insertQuery.addBindValue(remainingBalance);
                    double interestPayment = remainingBalance * currentRate;
                    insertQuery.addBindValue(remainingBalance + interestPayment);
                    insertQuery.addBindValue(monthlyPayment);
                    insertQuery.addBindValue(remainingBalance + interestPayment - monthlyPayment);
                    insertQuery.exec();

                    remainingBalance = remainingBalance + interestPayment - monthlyPayment;
                }
            }
        }
    }
    else if (payment_type == "Дифференцированные")
    {
        if(type == "Фиксированная")
        {
            // перевод в десятичные
            if (percent == "% в месяц")
            {
                rate = rate / 100;
            }
            // перевод в десятичные и в месячную ставку
            else if (percent == "% в год")
            {
                rate = (rate / 100) / 12;
            }

            // перевод срока в месяцы
            if(month_or_year == "лет")
            {
                time = time * 12;
            }

            double principalPayment = money / time;
            double totalPayment = 0.0;

            for (int i = 1; i <= time; ++i)
            {
                double interestPayment = (money - (principalPayment * (i - 1))) * rate;
                payment = principalPayment + interestPayment;
                totalPayment += payment;

                QSqlQuery query;
                query.prepare("INSERT INTO payment_details (date, before, after, payment, after_pay) VALUES (:date, :before, :after, :payment, :after_pay)");
                query.bindValue(":date", date.addMonths(i - 1).toString("yyyy-MM-dd"));
                query.bindValue(":before", money - (principalPayment * (i - 1)));
                query.bindValue(":after", (money - (principalPayment * (i - 1))) * (1 + rate));
                query.bindValue(":payment", payment);
                query.bindValue(":after_pay", (money - (principalPayment * (i - 1))) * (1 + rate) - payment);
                query.exec();

            }
        }
        else
        {
            double segmentPayment = 0.0, result = 0.0;
            int monthsPassed = 0;
            double remainingBalance = money;

            for (int i = 0; i < percent_v.size(); ++i)
            {
                double currentRate = percent_v[i];

                if (rate_v[i] == "% в месяц") {
                    currentRate /= 100;
                }
                else if (rate_v[i] == "% в год") {
                    currentRate = (currentRate / 100) / 12;
                }

                int duration = 0;
                if (i < percent_v.size() - 1)
                {
                    QDate startDate = date_v[i];
                    QDate endDate = date_v[i + 1];
                    int startMonths = startDate.year() * 12 + startDate.month();
                    int endMonths = endDate.year() * 12 + endDate.month();
                    duration = endMonths - startMonths;
                }
                else
                {
                    QDate lastStartDate = date_v[i - 1];
                    QDate endDate = date_v[i];
                    int lastStartMonths = lastStartDate.year() * 12 + lastStartDate.month();
                    int endMonths = endDate.year() * 12 + endDate.month();
                    duration = endMonths - lastStartMonths;
                }

                monthsPassed += duration;
                double principalPayment = money / percent_v.size() / duration;

                for (int j = 0; j < duration; ++j)
                {
                    double interestPayment = remainingBalance * currentRate;
                    segmentPayment = principalPayment + interestPayment;

                    result += segmentPayment;

                    query.prepare("INSERT INTO payment_details (date, before, after, payment, after_pay) VALUES (:date, :before, :after, :payment, :after_pay)");
                    query.bindValue(":date", date_v[i].addMonths(monthsPassed - duration + j).toString("yyyy-MM-dd"));
                    query.bindValue(":before", remainingBalance);
                    remainingBalance *= (1 + currentRate);
                    query.bindValue(":after", remainingBalance);
                    remainingBalance -= segmentPayment;
                    query.bindValue(":payment", segmentPayment);
                    query.bindValue(":after_pay", remainingBalance);
                    query.exec();
                }
            }
        }
    }
    credit->to_report();
}

