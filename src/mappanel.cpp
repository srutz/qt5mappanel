
#include "mappanel.h"
#include "datafetcher.h"
#include <QPainter>
#include <cmath>

using namespace std;

MapPanel::MapPanel(const TileServer &server, QWidget *parent) : QWidget(parent), tileServer(server)
{
    qDebug() << "MapPanel created with TileServer URL:" << tileServer.baseUrl;
}

MapPanel::~MapPanel() {}

int MapPanel::getZoom() const { return zoom; }

void MapPanel::setZoom(int zoom)
{
    if (zoom == this->zoom) {
        return;
    }
    int oldZoom = this->zoom;
    this->zoom = min(tileServer.maxZoom, zoom);
    emit zoomChanged(oldZoom, this->zoom);
}

// see  https://sourceforge.net/p/mappanel/code/HEAD/tree/com.roots.map/src/com/roots/map/MapPanel.java#l585

void MapPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "Painting MapPanel with TileServer URL:" << tileServer.baseUrl;
    int width = this->width();
    int height = this->height();
    QPainter painter(this);
    painter.fillRect(0, 0, width, height, Qt::lightGray);

    int x0 = static_cast<int>(floor(static_cast<double>(mapPosition.x()) / TILE_SIZE));
    int y0 = static_cast<int>(floor(static_cast<double>(mapPosition.y()) / TILE_SIZE));
    int x1 = static_cast<int>(ceil((static_cast<double>(mapPosition.x()) + width) / TILE_SIZE));
    int y1 = static_cast<int>(ceil((static_cast<double>(mapPosition.y()) + height) / TILE_SIZE));

    int dy = y0 * TILE_SIZE - mapPosition.y();
    for (int y = y0; y < y1; ++y) {
        int dx = x0 * TILE_SIZE - mapPosition.x();
        for (int x = x0; x < x1; ++x) {
            paintTile(painter, dx, dy, x, y);
            dx += TILE_SIZE;
        }
        dy += TILE_SIZE;
    }
}

void MapPanel::paintTile(QPainter &painter, int dx, int dy, int x, int y)
{
    bool DEBUG = !false;
    bool DRAW_IMAGES = true;
    bool DRAW_OUT_OF_BOUNDS = false;

    int xTileCount = 1 << zoom;
    int yTileCount = 1 << zoom;
    bool tileInBounds = x >= 0 && x < xTileCount && y >= 0 && y < yTileCount;
    bool drawImage = DRAW_IMAGES && tileInBounds;
    if (drawImage) {
        auto cacheEntry = tileCache.getTile(TileKey{x, y, zoom});
        if (cacheEntry.has_value() && cacheEntry->image.has_value()) {
            auto image = cacheEntry->image.value();
            painter.drawImage(QRect(dx, dy, TILE_SIZE, TILE_SIZE), image);
        } else if (!cacheEntry.has_value()) {
            // mark as loading
            tileCache.putTile(TileKey{x, y, zoom}, std::nullopt);
            // initiate fetch
            auto format = tileServer.baseUrl + "/%1/%2/%3.png";
            auto url = format.arg(zoom).arg(x).arg(y);
            auto fetcher = new DataFetcher(this);
            DataFetcher::FetchOptions options{.url = url};
            connect(fetcher, &DataFetcher::responseReceived, this,
                    [this, x, y, fetcher](const QByteArray &data) {
                        QImage image;
                        image.loadFromData(data);
                        tileCache.putTile(TileKey{x, y, zoom}, image);
                        this->update();
                        fetcher->deleteLater();
                    });
            connect(fetcher, &DataFetcher::error, this, [fetcher](const QString &message) {
                qDebug() << "Error fetching tile:" << message;
                fetcher->deleteLater();
            });
            fetcher->fetch(options);
        }
        if (DEBUG) {
            painter.setPen(Qt::gray);
            painter.drawRect(dx, dy, TILE_SIZE, TILE_SIZE);
            painter.setPen(Qt::black);
            QString s = QString("T %1, %2%3").arg(x).arg(y).arg(!tileInBounds ? " #" : "");
            painter.setPen(Qt::gray);
            painter.drawText(dx + 4 + 8, dy + 4 + 12, s);
        }
    }
}
