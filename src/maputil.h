#ifndef MAPUTIL_H
#define MAPUTIL_H

#include "constants.h"
#include <QPoint>
#include <cmath>

using namespace std;

struct MapUtil {

    struct Coordinate {
        double lat = 0;
        double lon = 0;
    };

    static double toRad(double deg) { return deg * M_PI / 180.0; }
    static double toDeg(double rad) { return rad * 180.0 / M_PI; }

    static int lon2position(double lon, int z);
    static int lat2position(double lat, int z);
    static QPoint latLonToPosition(double lat, double lon, int zoom);
    static Coordinate positionToLatLon(const QPoint &position, int zoom);
};

#endif // MAPUTIL_H
