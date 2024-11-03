#include "include/currencies.h"


Currencies::Currencies(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui),
    model(nullptr)
{
    networkManager = new QNetworkAccessManager(this);
    searchModel = new QSqlQueryModel();
}


Currencies::~Currencies()
{
    if (model)
    {
        delete model;
    }
    if (searchModel)
    {
        delete searchModel;
    }
}


void Currencies::open()
{
    ui->stackedWidget->setCurrentWidget(ui->page_currencies);
    open_currencies();
}


void Currencies::open_currencies()
{
    QUrl url("https://cbr.ru/currency_base/daily/");
    QNetworkRequest request(url);

    // Настройка SSL, если это необходимо
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);  // Отключает проверку SSL-сертификата
    request.setSslConfiguration(sslConfig);

    networkManager->get(request);  // Отправляем GET-запрос
}


void Currencies::back()
{
    ui->stackedWidget->setCurrentWidget(ui->page_main);
}


size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    QByteArray *data = reinterpret_cast<QByteArray*>(userp);
    *data += QByteArray::fromRawData(reinterpret_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}


void Currencies::fetchData(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << "Error fetching data:" << reply->errorString();
    }
    else
    {
        // Чтение и обработка данных
        QByteArray response = reply->readAll();
        QString responseString = QString::fromUtf8(response);
        parseData(responseString);  // Передача данных для парсинга
    }

    reply->deleteLater();
}


void Currencies::parseData(const QString &content)
{
    QSqlQuery query(db);

    query.exec("DROP TABLE IF EXISTS exchangerate");
    query.exec("CREATE TABLE exchangerate (intcode INTEGER, letcode TEXT, unit INTEGER, currency TEXT, course REAL)");

    QRegularExpression re(R"(<tr>\s*<td>(\d+)</td>\s*<td>(\w+)</td>\s*<td>(.*?)</td>\s*<td>(.*?)</td>\s*<td>(.*?)</td>)");
    QRegularExpressionMatchIterator i = re.globalMatch(content);

    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        int unit = match.captured(1).toInt();
        QString currencyCode = match.captured(2);
        int currencyName = match.captured(3).toInt();
        QString exchangeRate = match.captured(4);
        QString course = match.captured(5);
        QStringList parts = course.split(',');
        QString formattedString = parts.join('.');
        double coursee = formattedString.toDouble();

        query.prepare("INSERT INTO ExchangeRate (intcode, letcode, unit, currency, course) VALUES (:intc, :letc, :uni, :curr, :cour)");
        query.bindValue(":intc", unit);
        query.bindValue(":letc", currencyCode);
        query.bindValue(":uni", currencyName);
        query.bindValue(":curr", exchangeRate);
        query.bindValue(":cour", coursee);
        if (!query.exec())
        {
            qDebug() << "Insertion failed.";
        }
    }
    draw_table();
}


void Currencies::draw_table()
{
    model = new QSqlTableModel();
    model->setTable("exchangerate");

    model->setHeaderData(0, Qt::Horizontal, "Цифр.Код");
    model->setHeaderData(1, Qt::Horizontal, "Букв.Код");
    model->setHeaderData(2, Qt::Horizontal, "Единица");
    model->setHeaderData(3, Qt::Horizontal, "Наименование");
    model->setHeaderData(4, Qt::Horizontal, "Курс");
    model->select();

    ui->tableView_currencies->setModel(model);
    ui->tableView_currencies->resizeColumnsToContents();

    ui->tableView_currencies->setColumnWidth(3, 320);
    ui->tableView_currencies->setColumnWidth(4, 148);
}


void Currencies::searching()
{
    QString data = ui->lineEdit_search->text();
    QSqlQuery query(db);

    query.prepare("SELECT * FROM exchangerate WHERE currency LIKE :info");
    query.bindValue(":info", "%" + data + "%");

    if (query.exec())
    {
        if (searchModel)
        {
            delete searchModel;
        }

        searchModel = new QSqlQueryModel();
        searchModel->setQuery(query);

        searchModel->setHeaderData(0, Qt::Horizontal, "Цифр.Код");
        searchModel->setHeaderData(1, Qt::Horizontal, "Букв.Код");
        searchModel->setHeaderData(2, Qt::Horizontal, "Единица");
        searchModel->setHeaderData(3, Qt::Horizontal, "Наименование");
        searchModel->setHeaderData(4, Qt::Horizontal, "Курс");

        ui->tableView_currencies->setModel(searchModel);
        ui->tableView_currencies->resizeColumnsToContents();

        ui->tableView_currencies->setColumnWidth(3, 320);
        ui->tableView_currencies->setColumnWidth(4, 148);
    }
    else
    {
        qDebug() << "Search failed:" << query.lastError().text();
    }
}
