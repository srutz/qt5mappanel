#ifndef TILECACHE_H
#define TILECACHE_H

#include <QString>
#include <QHash>
#include <QDateTime>
#include <QImage>
#include <optional>

using std::optional;

struct TileKey
{
    int x;
    int y;
    int zoom;

    bool operator==(const TileKey &other) const
    {
        return x == other.x && y == other.y && zoom == other.zoom;
    }
};

struct CacheEntry
{
    optional<QImage> image;
    QDateTime timestamp;
};
inline uint qHash(const TileKey &key, uint seed = 0)
{
    return qHash(QString("%1_%2_%3").arg(key.x).arg(key.y).arg(key.zoom), seed);
}

class TileCache
{
public:
    TileCache();
    ~TileCache();

    optional<CacheEntry> getTile(const TileKey &tileKey);
    void putTile(const TileKey &tileKey, const optional<QImage> &image);

private:
    QHash<TileKey, CacheEntry> cache;
    void evictOldEntries();
};

#endif // TILECACHE_H