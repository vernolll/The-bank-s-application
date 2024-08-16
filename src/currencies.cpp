#include "include/currencies.h"


Currencies::Currencies(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui),
    curl(nullptr)  // Initialize curl to nullptr
{
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


Currencies::~Currencies()
{
    if (curl)
    {
        curl_easy_cleanup(curl);  // Clean up curl when the object is destroyed
    }
}


void Currencies::open_currencies()
{
    ui->stackedWidget->setCurrentWidget(ui->page_currencies);
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
    // Regular expressions to capture required data
    QRegularExpression re(R"(<tr>\s*<td>(\d+)</td>\s*<td>(\w+)</td>\s*<td>(.*?)</td>\s*<td>(.*?)</td>\s*<td>(.*?)</td>)");
    QRegularExpressionMatchIterator i = re.globalMatch(content);

    while (i.hasNext())
    {
        QRegularExpressionMatch match = i.next();
        int unit = match.captured(1).toInt();
        QString currencyCode = match.captured(2);
        QString currencyName = match.captured(3);
        QString exchangeRate = match.captured(4);
        QString course = match.captured(5);

        qDebug() << "Цифр. код:" << unit;
        qDebug() << "Букв. код:" << currencyCode;
        qDebug() << "Единиц:" << currencyName;
        qDebug() << "Валюта:" << exchangeRate;
        qDebug() << "Курс:" << course;
        qDebug() << "-------------------";
    }
}
