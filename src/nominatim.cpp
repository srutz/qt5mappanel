
#include "nominatim.h"
#include <QJsonArray>
#include <QJsonDocument>

// Parse a single result from a JSON object
NominatimResult NominatimResult::fromJson(const QJsonObject &json)
{
    NominatimResult result;
    result.place_id = json["place_id"].toVariant().toLongLong();
    result.licence = json["licence"].toString();
    result.osm_type = json["osm_type"].toString();
    result.osm_id = json["osm_id"].toVariant().toLongLong();
    result.lat = json["lat"].toString().toDouble();
    result.lon = json["lon"].toString().toDouble();
    result.classType = json["class"].toString();
    result.type = json["type"].toString();
    result.place_rank = json["place_rank"].toInt();
    result.importance = json["importance"].toDouble();
    result.addresstype = json["addresstype"].toString();
    result.name = json["name"].toString();
    result.display_name = json["display_name"].toString();

    auto bbox = json["boundingbox"].toArray();
    if (bbox.size() >= 4) {
        result.boundingbox.latMin = bbox[0].toString().toDouble();
        result.boundingbox.latMax = bbox[1].toString().toDouble();
        result.boundingbox.lonMin = bbox[2].toString().toDouble();
        result.boundingbox.lonMax = bbox[3].toString().toDouble();
    }
    return result;
}

// Parse multiple results from a JSON string
QVector<NominatimResult> NominatimResult::fromJsonString(const QString &s)
{
    QVector<NominatimResult> results;
    auto doc = QJsonDocument::fromJson(s.toUtf8());
    if (!doc.isArray()) {
        return results;
    }
    auto array = doc.array();
    for (const QJsonValue &val : array) {
        if (val.isObject()) {
            results.append(fromJson(val.toObject()));
        }
    }
    return results;
}
