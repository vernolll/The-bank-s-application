#include "include/news.h"

News::News(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui)
{
    curl = curl_easy_init();
    model = new QStandardItemModel();
}


News::~News()
{
    delete model;
    curl_easy_cleanup(curl);
}


void News::open_news()
{
    ui->stackedWidget->setCurrentWidget(ui->page_news);

    CURLcode res;

    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS news");
    query.exec("CREATE TABLE news (news TEXT, url TEXT)");

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

        draw_table();
    }
    else
    {
        qDebug() << "Failed to initialize CURL";
    }
}


size_t News::WriteCallback(void *contents, size_t size, size_t nmemb)
{
    QString htmlContent = QString::fromUtf8(static_cast<char*>(contents), size * nmemb);

    QRegularExpression regex("href=\"([^\"]+)\"\\s*>\\s*([^<]+)<\\/a>");
    QRegularExpressionMatchIterator matchIterator = regex.globalMatch(htmlContent);

    QSqlQuery query;

    while (matchIterator.hasNext())
    {
        QRegularExpressionMatch match = matchIterator.next();
        QString link = "https://www.banki.ru" + match.captured(1);
        QString articleTitle = match.captured(2);
        articleTitle.remove(QRegExp("[\n\t]"));

        query.prepare("INSERT INTO news (news, url) VALUES (:news, :url)");
        query.bindValue(":news", articleTitle);
        query.bindValue(":url", link);

        if (!query.exec())
        {
            qDebug() << "Error inserting data into news table: " << query.lastError().text();
        }
    }

    return size * nmemb;
}


void News::back()
{
    ui->stackedWidget->setCurrentWidget(ui->page_main);
}


void News::draw_table()
{

    if (ui->tableView_news->model() != nullptr)
    {
        delete ui->tableView_news->model();
    }

    model->setColumnCount(1);
    model->setHeaderData(0, Qt::Horizontal, "Новости");

    QSqlQuery query;
    query.exec("SELECT news FROM news");

    int row = 0;
    while (query.next())
    {
        QString newsText = query.value(0).toString();
        QStandardItem *item = new QStandardItem(newsText);
        item->setTextAlignment(Qt::AlignCenter);
        model->setItem(row, 0, item);
        row++;
    }

    ui->tableView_news->setModel(model);
    ui->tableView_news->setColumnWidth(0, 750);
    ui->tableView_news->verticalHeader()->hide();
    ui->tableView_news->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableView_news->show();
}


void News::open_site(const QModelIndex &index)
{
    if (index.isValid())
    {
        QSqlQuery query;
        query.prepare("SELECT url FROM news LIMIT 1 OFFSET :rowIndex");
        query.bindValue(":rowIndex", index.row());

        if (query.exec() && query.first())
        {
            QString url = query.value(0).toString();
            QUrl qurl(url);
            QDesktopServices::openUrl(qurl);
        }
    }
}
