#include "datafetcher.h"

#include "raiiguard.h"

DataFetcher::DataFetcher(const QString &m_info, QObject *parent)
    : QObject(parent), m_manager(new QNetworkAccessManager(this)), m_info(m_info)
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &DataFetcher::handleNetworkResponse);
}

DataFetcher::~DataFetcher() {}

void DataFetcher::fetch(const FetchOptions &options)
{
    QUrl url(options.url);
    // qDebug() << "fetch" << url.toString();
    if (!url.isValid()) {
        emit error("Invalid URL");
        return;
    }
    QNetworkRequest request(url);
    for (auto it = options.headers.constBegin(); it != options.headers.constEnd(); ++it) {
        request.setRawHeader(it.key().toUtf8(), it.value().toUtf8());
    }

    // make us appear like firefox
    request.setRawHeader(
        "User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:104.0) "
                      "Gecko/20100101 Firefox/104.0");
    // accept header for pngs and images
    request.setRawHeader("Accept", "image/avif,image/webp,*/*");
    // cache options like a brwoser would
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);

    if (options.method == "POST" || options.method == "PATCH") {
        m_manager->post(request, options.data.value_or(QByteArray()));
    } else if (options.method == "PUT") {
        m_manager->put(request, options.data.value_or(QByteArray()));
    } else {
        m_manager->get(request);
    }
}

void DataFetcher::handleNetworkResponse(QNetworkReply *reply)
{
    m_completed = true;
    /* make sure reply is deleted under all circumstances */
    RAIIGuard guard([reply] { reply->deleteLater(); });
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network error for URL:" << reply->url().toString() << "Error code:" << reply->error()
                 << "Error message:" << reply->errorString();
        emit error(reply->errorString());
        return;
    }
    auto data = reply->readAll();
    /*
    qDebug() << "response:"
             << "url" << reply->url().toString() << "http-status"
             << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << "content-type"
             << reply->header(QNetworkRequest::ContentTypeHeader).toString() << "content-length"
             << reply->header(QNetworkRequest::ContentLengthHeader).toLongLong() << "size" << data.size() << "bytes";
    */
    emit responseReceived(data);
}
