
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
