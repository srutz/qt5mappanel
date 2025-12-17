#ifndef DATAFETCHER_H
#define DATAFETCHER_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

/*
 * Generic Data Fetcher for loading web resources
 */

class DataFetcher : public QObject
{
    Q_OBJECT

  public:
    /* options for the fetch */
    struct FetchOptions {
        QString url;
        QString method = "GET"; // GET, POST, PUT, DELETE
        QMap<QString, QString> headers;
        std::optional<QByteArray> data; // POST / PUT / PATCH upload-data
    };

    explicit DataFetcher(QObject *parent = nullptr);
    void fetch(const FetchOptions &options);

  signals:
    void responseReceived(const QByteArray &document);
    void error(const QString &message);

  private slots:
    void handleNetworkResponse(QNetworkReply *reply);

  private:
    QNetworkAccessManager *manager;
};

#endif // DATAFETCHER_H
