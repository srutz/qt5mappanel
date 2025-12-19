#ifndef TILECACHE_H
#define TILECACHE_H

#include <QDateTime>
#include <QHash>
#include <QImage>
#include <QString>
#include <optional>

using std::optional;

const int MAX_TILE_CACHE_ENTRIES = 1000;

struct TileKey {
    int x;
    int y;
    int zoom;

    bool operator==(const TileKey &other) const { return x == other.x && y == other.y && zoom == other.zoom; }
};

struct CacheEntry {
    optional<QImage> image;
    QDateTime timestamp;
};
inline uint qHash(const TileKey &key, uint seed = 0)
{
    return qHash(key.x, seed) ^ (qHash(key.y, seed) << 1) ^ (qHash(key.zoom, seed) << 2);
}

class TileCache
{
  public:
    TileCache();
    ~TileCache();

    optional<CacheEntry> getTile(const TileKey &tileKey);
    void putTile(const TileKey &tileKey, const optional<QImage> &image);
    void dump();
    int size() const { return m_cache.size(); }

  private:
    QHash<TileKey, CacheEntry> m_cache;
    void evictOldEntries();
};

#endif // TILECACHE_H