
#include <QPainter>
#include <cmath>
#include "mappanel.h"

using namespace std;

MapPanel::MapPanel(const TileServer &server, QWidget *parent)
    : QWidget(parent), tileServer(server)
{
    qDebug() << "MapPanel created with TileServer URL:" << tileServer.baseUrl;
}

MapPanel::~MapPanel()
{
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
    for (int y = y0; y < y1; ++y)
    {
        int dx = x0 * TILE_SIZE - mapPosition.x();
        for (int x = x0; x < x1; ++x)
        {
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
    if (drawImage)
    {
    }
    if (DEBUG)
    {
        painter.setPen(Qt::gray);
        painter.drawRect(dx, dy, TILE_SIZE, TILE_SIZE);
        painter.setPen(Qt::black);
        painter.fillRect(dx + 4, dy + 4, TILE_SIZE - 8, TILE_SIZE - 8, Qt::blue);
        QString s = QString("T %1, %2%3").arg(x).arg(y).arg(!tileInBounds ? " #" : "");
        painter.setPen(Qt::gray);
        painter.drawText(dx + 4 + 8, dy + 4 + 12, s);
    }
}
