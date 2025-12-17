
#include "tilecache.h"
#include <QDebug>

TileCache::TileCache() {}

TileCache::~TileCache() {}

optional<CacheEntry> TileCache::getTile(const TileKey &tileKey)
{
    if (m_cache.contains(tileKey)) {
        return m_cache.value(tileKey);
    }
    return std::nullopt;
}

void TileCache::putTile(const TileKey &tileKey, const optional<QImage> &image)
{
    CacheEntry entry{.image = image, .timestamp = QDateTime::currentDateTime()};
    auto oldSize = m_cache.size();
    m_cache.insert(tileKey, entry);
    auto newSize = m_cache.size();
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
    while (m_cache.size() > maxEntries) {
        auto oldestKey = m_cache.begin().key();
        for (auto it = m_cache.begin(); it != m_cache.end(); ++it) {
            if (it.value().timestamp < m_cache.value(oldestKey).timestamp) {
                oldestKey = it.key();
            }
        }
        m_cache.remove(oldestKey);
    }
}