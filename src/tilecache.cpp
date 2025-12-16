
#include "tilecache.h"

TileCache::TileCache()
{
}

TileCache::~TileCache()
{
}

optional<QImage> TileCache::getTile(const TileKey &tileKey)
{
    if (cache.contains(tileKey))
    {
        return cache.value(tileKey).image;
    }
    return std::nullopt;
}

void TileCache::putTile(const TileKey &tileKey, const QImage &image)
{
    CacheEntry entry{.image = image, .timestamp = QDateTime::currentDateTime()};
    cache.insert(tileKey, entry);
    evictOldEntries();
}

void TileCache::evictOldEntries()
{
    const auto maxEntries = 1000;
    // Simple eviction policy: remove oldest entries until size is under maxEntries
    while (cache.size() > maxEntries)
    {
        auto oldestKey = cache.begin().key();
        for (auto it = cache.begin(); it != cache.end(); ++it)
        {
            if (it.value().timestamp < cache.value(oldestKey).timestamp)
            {
                oldestKey = it.key();
            }
        }
        cache.remove(oldestKey);
    }
}