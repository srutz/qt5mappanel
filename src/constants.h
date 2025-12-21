#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QString>

const auto TILE_SIZE = 256;

struct TileServer {
    QString baseUrl;
    int maxZoom = 18;
};

const TileServer TILE_SERVERS[] = {
    {.baseUrl = "https://tile.openstreetmap.org", .maxZoom = 19},
    {.baseUrl = "https://tile.opentopomap.org", .maxZoom = 18},
};

#endif // CONSTANTS_H
