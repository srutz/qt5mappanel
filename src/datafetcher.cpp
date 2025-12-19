#include "datafetcher.h"

#include "raiiguard.h"

DataFetcher::DataFetcher(QObject *parent) : QObject(parent), manager(new QNetworkAccessManager(this))
{
    connect(manager, &QNetworkAccessManager::finished, this, &DataFetcher::handleNetworkResponse);
}

void DataFetcher::fetch(const FetchOptions &options)
{
    QUrl url(options.url);
    qDebug() << "Fetching URL:" << url.toString();
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
        manager->post(request, options.data.value_or(QByteArray()));
    } else if (options.method == "PUT") {
        manager->put(request, options.data.value_or(QByteArray()));
    } else {
        manager->get(request);
    }
}

void DataFetcher::handleNetworkResponse(QNetworkReply *reply)
{
    /* make sure reply is deleted under all circumstances */
    RAIIGuard guard([reply] { reply->deleteLater(); });
    if (reply->error() != QNetworkReply::NoError) {
        emit error(reply->errorString());
        return;
    }
    auto data = reply->readAll();
    qDebug() << "response:"
             << "url" << reply->url().toString() << "http-status"
             << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << "content-type"
             << reply->header(QNetworkRequest::ContentTypeHeader).toString() << "content-length"
             << reply->header(QNetworkRequest::ContentLengthHeader).toLongLong() << "size" << data.size() << "bytes";
    // QString::fromUtf8(data);
    emit responseReceived(data);
}
