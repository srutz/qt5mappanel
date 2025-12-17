#ifndef MAPPANEL_H
#define MAPPANEL_H

#include "constants.h"
#include "tilecache.h"
#include <QDebug>
#include <QPoint>
#include <QString>
#include <QWidget>

struct TileServer {
    QString baseUrl;
    int maxZoom = 18;
};

class MapPanel : public QWidget
{
    Q_OBJECT

  private:
    TileServer m_tileServer;
    TileCache m_tileCache;
    QPoint m_mapPosition = {0, 0};
    int m_zoom = 8;

    void paintTile(QPainter &painter, int dx, int dy, int x, int y);

  public:
    MapPanel(const TileServer &server, QWidget *parent = nullptr);
    ~MapPanel();
    const QPoint &mapPosition() const;
    void setMapPosition(QPoint p);
    void setMapPositionCentered(QPoint p);

    int zoom() const;
    void setZoom(int zoom);
    void zoomInOut(QPoint pivot, int delta);

  protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

  signals:
    void zoomChanged(int oldZoom, int zoom);
};

#endif // MAPPANEL_H
