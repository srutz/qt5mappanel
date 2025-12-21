#ifndef NOMINATIM_H
#define NOMINATIM_H

#include <QJsonObject>
#include <QString>
#include <QVector>

struct BoundingBox {
    double latMin = 0.0;
    double latMax = 0.0;
    double lonMin = 0.0;
    double lonMax = 0.0;
};

struct NominatimResult {
    qint64 place_id = 0;
    QString licence;
    QString osm_type;
    qint64 osm_id = 0;
    double lat = 0.0;
    double lon = 0.0;
    QString classType;
    QString type;
    int place_rank = 0;
    double importance = 0.0;
    QString addresstype;
    QString name;
    QString display_name;
    BoundingBox boundingbox;

    // parse
    static NominatimResult fromJson(const QJsonObject &json);
    static QVector<NominatimResult> fromJsonString(const QString &s);
};

#endif // NOMINATIM_H