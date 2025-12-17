#ifndef MAPPANEL_H
#define MAPPANEL_H

#include "tilecache.h"
#include <QDebug>
#include <QPoint>
#include <QString>
#include <QWidget>

const int TILE_SIZE = 256;

struct TileServer {
    QString baseUrl;
    int maxZoom = 16;
};

class MapPanel : public QWidget
{
    Q_OBJECT

  private:
    TileServer tileServer;
    TileCache tileCache;
    QPoint mapPosition = {0, 0};
    int zoom = 8;

    void paintTile(QPainter &painter, int dx, int dy, int x, int y);

  public:
    MapPanel(const TileServer &server, QWidget *parent = nullptr);
    ~MapPanel();
    int getZoom() const;
    void setZoom(int zoom);

  protected:
    void paintEvent(QPaintEvent *event) override;

  signals:
    void zoomChanged(int oldZoom, int zoom);
};

#endif // MAPPANEL_H
