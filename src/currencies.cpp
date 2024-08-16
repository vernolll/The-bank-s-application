#include "include/currencies.h"

#include <QString>
#include <QRegularExpression>
#include <QDebug>

Currencies::Currencies(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui),
    curl(nullptr)  // Initialize curl to nullptr
{
    // Initialize curl and check if successful
    curl = curl_easy_init();
    if (curl) {
        // Set SSL options in the constructor
        curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/ca-certificates.crt");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        // Fetch data
        fetchData();
    } else {
        qDebug() << "Failed to initialize curl";
    }
}

Currencies::~Currencies()
{
    if (curl) {
        curl_easy_cleanup(curl);  // Clean up curl when the object is destroyed
    }
}

void Currencies::open_currencies()
{
    ui->stackedWidget->setCurrentWidget(ui->page_currencies);
}

// Callback function for curl to store the response
size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
    QString *data = reinterpret_cast<QString*>(userp);
    *data += QString::fromLatin1(reinterpret_cast<char*>(buffer), size * nmemb);
    return size * nmemb;
}

void Currencies::fetchData()
{
    if (curl) {  // Use the same curl instance created in the constructor
        QString response;

        // Set URL
        curl_easy_setopt(curl, CURLOPT_URL, "https://cbr.ru/currency_base/daily/");

        // Set callback for writing data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            qDebug() << "Error fetching data:" << curl_easy_strerror(res);
        } else {
            // Parse the data
            parseData(response);
        }
    } else {
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
        QString unit = match.captured(1);
        QString currencyCode = match.captured(2);
        QString currencyName = match.captured(3);
        QString exchangeRate = match.captured(4);

        qDebug() << "Unit:" << unit;
        qDebug() << "Currency Code:" << currencyCode;
        qDebug() << "Currency Name:" << currencyName;
        qDebug() << "Exchange Rate:" << exchangeRate;
        qDebug() << "-------------------";
    }
}
