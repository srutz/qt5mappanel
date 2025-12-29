
#include "maputil.h"
#include <cmath>

using namespace std;

int MapUtil::lon2position(double lon, int z)
{
    double xmax = TILE_SIZE * (1 << z);
    return static_cast<int>(floor((lon + 180) / 360 * xmax));
}

int MapUtil::lat2position(double lat, int z)
{
    double ymax = TILE_SIZE * (1 << z);
    return static_cast<int>(floor((1 - log(tan(toRad(lat)) + 1 / cos(toRad(lat))) / M_PI) / 2 * ymax));
}

QPoint MapUtil::latLonToPosition(double lat, double lon, int zoom)
{
    auto x = lon2position(lon, zoom);
    auto y = lat2position(lat, zoom);
    return QPoint(x, y);
}

MapUtil::Coordinate MapUtil::positionToLatLon(const QPoint &position, int zoom)
{
    double xmax = TILE_SIZE * (1 << zoom);
    double lon = position.x() / xmax * 360.0 - 180;
    double ymax = TILE_SIZE * (1 << zoom);
    double lat = toDeg(atan(sinh(M_PI - (2.0 * M_PI * position.y()) / ymax)));
    return Coordinate{.lat = lat, .lon = lon};
}

// Haversine formula to calculate distance between two lat/lon points in meters
double MapUtil::distanceInMeters(double lat1, double lon1, double lat2, double lon2)
{
    const double EARTH_RADIUS = 6371000.0; // Earth's radius in meters

    double dLat = toRad(lat2 - lat1);
    double dLon = toRad(lon2 - lon1);

    double a = sin(dLat / 2) * sin(dLat / 2) + cos(toRad(lat1)) * cos(toRad(lat2)) * sin(dLon / 2) * sin(dLon / 2);

    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return EARTH_RADIUS * c;
}
