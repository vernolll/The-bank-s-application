#include "include/news.h"

News::News(Ui::MainWindow *ui, QObject *parent) :
    QObject(parent),
    ui(ui)
{
    model = new QStandardItemModel();
}


News::~News()
{
    delete model;
}


void News::open_news()
{
    ui->stackedWidget->setCurrentWidget(ui->page_news);

    // Очистка таблицы новостей и создание новой таблицы
    QSqlQuery query;
    query.exec("DROP TABLE IF EXISTS news");
    query.exec("CREATE TABLE news (news TEXT, url TEXT)");

    // Настройка запроса
    QUrl url("https://www.banki.ru/news/lenta/");
    QNetworkRequest request(url);

    // Опциональная настройка SSL
    QSslConfiguration sslConfig = request.sslConfiguration();
    sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);  // Отключает проверку SSL-сертификата
    request.setSslConfiguration(sslConfig);

    // Выполнение GET-запроса
    networkManager->get(request);
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
        articleTitle.remove(QRegularExpression("[\n\t]"));

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
