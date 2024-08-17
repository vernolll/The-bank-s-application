#include "include/currencies.h"


Currencies::Currencies(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui),
    curl(nullptr),
    model(nullptr)
{
}


Currencies::~Currencies()
{
    if (curl)
    {
        curl_easy_cleanup(curl);  // Clean up curl when the object is destroyed
    }
    delete model;
}


void Currencies::open_currencies()
{
    ui->stackedWidget->setCurrentWidget(ui->page_currencies);

    // Initialize curl and check if successful
    curl = curl_easy_init();
    if (curl)
    {
        // Set SSL options in the constructor
        curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/ca-certificates.crt");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        fetchData();
    }
    else
    {
        qDebug() << "Failed to initialize curl";
    }
}


void Currencies::back()
{
    ui->stackedWidget->setCurrentWidget(ui->page_main);
}


// Callback function for curl to store the response
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp)
{
    QByteArray *data = reinterpret_cast<QByteArray*>(userp);
    *data += QByteArray::fromRawData(reinterpret_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}


void Currencies::fetchData()
{
    if (curl)
    {
        QByteArray response; // Use QByteArray

        // Set URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://cbr.ru/currency_base/daily/");

        // Set callback for writing data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            qDebug() << "Error fetching data:" << curl_easy_strerror(res);
        }
        else
        {
            QString responseString = QString::fromUtf8(response);
            parseData(responseString);
        }
    }
    else
    {
        qDebug() << "Curl is not initialized";
    }
}


void Currencies::parseData(const QString &content)
{
    QSqlQuery query(db);

    query.exec("DROP TABLE IF EXISTS ExchangeRate");
    query.exec("CREATE TABLE ExchangeRate (intcode INTEGER, letcode TEXT, unit INTEGER, currency TEXT, course REAL)");

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
        query.exec();
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

    model->setHeaderData(0, Qt::Horizontal, "Unit");
    model->setHeaderData(1, Qt::Horizontal, "Currency Code");
    model->setHeaderData(2, Qt::Horizontal, "Currency Name");
    model->setHeaderData(3, Qt::Horizontal, "Exchange Rate");
    model->setHeaderData(4, Qt::Horizontal, "Course");
    model->select();

    ui->tableView_currencies->setModel(model);
    ui->tableView_currencies->resizeColumnsToContents();
}





