
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
        evictOldEntries();
    }
}

void TileCache::evictOldEntries()
{
    // Simple eviction policy: remove oldest entries until size is under maxEntries
    while (m_cache.size() > MAX_TILE_CACHE_ENTRIES) {
        auto oldestKey = m_cache.begin().key();
        for (auto it = m_cache.begin(); it != m_cache.end(); ++it) {
            if (it.value().timestamp < m_cache.value(oldestKey).timestamp) {
                oldestKey = it.key();
            }
        }
        m_cache.remove(oldestKey);
    }
}

void TileCache::dump()
{
    qDebug() << "TileCache dump: size =" << m_cache.size();
    for (auto it = m_cache.begin(); it != m_cache.end(); ++it) {
        qDebug() << "TileKey: (" << it.key().x << "," << it.key().y << "," << it.key().zoom << ")"
                 << "Timestamp:" << it.value().timestamp.toString() << "Has Image:" << it.value().image.has_value()
                 << "Image Size:"
                 << (it.value().image.has_value()
                         ? QString("%1x%2").arg(it.value().image->width()).arg(it.value().image->height())
                         : "N/A");
    }
}