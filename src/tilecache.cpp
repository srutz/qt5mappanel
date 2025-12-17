
#include "tilecache.h"
#include <QDebug>

TileCache::TileCache() {}

TileCache::~TileCache() {}

optional<CacheEntry> TileCache::getTile(const TileKey &tileKey)
{
    if (cache.contains(tileKey)) {
        return cache.value(tileKey);
    }
    return std::nullopt;
}

void TileCache::putTile(const TileKey &tileKey, const optional<QImage> &image)
{
    CacheEntry entry{.image = image, .timestamp = QDateTime::currentDateTime()};
    auto oldSize = cache.size();
    cache.insert(tileKey, entry);
    auto newSize = cache.size();
    if (newSize != oldSize) {
        // qDebug() << "TileCache: inserted tile (" << tileKey.x << "," << tileKey.y << "," << tileKey.zoom << "), cache size:" <<
        // cache.size();
        evictOldEntries();
    }
}

void TileCache::evictOldEntries()
{
    const auto maxEntries = 1000;
    // Simple eviction policy: remove oldest entries until size is under maxEntries
    while (cache.size() > maxEntries) {
        auto oldestKey = cache.begin().key();
        for (auto it = cache.begin(); it != cache.end(); ++it) {
            if (it.value().timestamp < cache.value(oldestKey).timestamp) {
                oldestKey = it.key();
            }
        }
        cache.remove(oldestKey);
    }
}