#ifndef MAPUTIL_H
#define MAPUTIL_H

#include "constants.h"
#include <QPoint>
#include <cmath>

using namespace std;

struct MapUtil {

    static inline double toRadians(double deg) { return deg * M_PI / 180.0; }

    static int lon2position(double lon, int z);
    static int lat2position(double lat, int z);
    static QPoint latLonToTileXY(double lat, double lon, int zoom);
};

#endif // MAPUTIL_H
