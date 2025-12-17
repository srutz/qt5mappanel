
#include "maputil.h"

int MapUtil::lon2position(double lon, int z)
{
    double xmax = TILE_SIZE * (1 << z);
    return static_cast<int>(floor((lon + 180) / 360 * xmax));
}

int MapUtil::lat2position(double lat, int z)
{
    double ymax = TILE_SIZE * (1 << z);
    return static_cast<int>(floor((1 - log(tan(toRadians(lat)) + 1 / cos(toRadians(lat))) / M_PI) / 2 * ymax));
}

QPoint MapUtil::latLonToTileXY(double lat, double lon, int zoom)
{
    // -0.11, 51.51
    auto x = lon2position(lon, zoom);
    auto y = lat2position(lat, zoom);
    return QPoint(x, y);
}
