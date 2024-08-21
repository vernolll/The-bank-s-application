#include "include/news.h"

News::News(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui)
{
}


News::~News()
{
}


void News::open_news()
{
    ui->stackedWidget->setCurrentWidget(ui->page_news);

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();

    if(curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.banki.ru/news/lenta/");
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_setopt(curl, CURLOPT_CAINFO, "/etc/ssl/certs/ca-certificates.crt");

        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);

        res = curl_easy_perform(curl);

        if(res != CURLE_OK)
        {
            qDebug() << "Error fetching news headlines: " << curl_easy_strerror(res);
        }

        curl_easy_cleanup(curl);
    }
    else
    {
        qDebug() << "Failed to initialize CURL";
    }
}


size_t News::WriteCallback(void *contents, size_t size, size_t nmemb)
{
    QString htmlContent = QString::fromUtf8(static_cast<char*>(contents), size * nmemb);

    QRegularExpression regex(R"(<a[^>]*>([^<]+)</a>)");
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(htmlContent);

    while (matchIterator.hasNext())
    {
        QRegularExpressionMatch match = matchIterator.next();
        QString articleTitle = match.captured(1);
        articleTitle.remove(QRegExp("[\n\t]"));
        qDebug() << "Article Title: " << articleTitle;
    }


    return size * nmemb;
}


void News::back()
{
    ui->stackedWidget->setCurrentWidget(ui->page_main);
}
