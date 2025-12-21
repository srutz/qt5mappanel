#ifndef MAPPANEL_H
#define MAPPANEL_H

#include "constants.h"
#include "tilecache.h"
#include <QDebug>
#include <QPoint>
#include <QString>
#include <QWidget>
#include <optional>

using std::optional;

struct TileServer {
    QString baseUrl;
    int maxZoom = 18;
};

class MapPanel : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int zoom READ zoom WRITE setZoom NOTIFY zoomChanged)
    Q_PROPERTY(int tileCacheSize READ tileCacheSize NOTIFY tileCacheSizeChanged)

  private:
    TileServer m_tileServer;
    TileCache m_tileCache;
    QPoint m_mapPosition = {0, 0};
    int m_zoom = 8;
    bool m_Debug = false;
    QPoint m_mousePosition;
    optional<QPoint> m_downCoords;

    void paintTile(QPainter &painter, int dx, int dy, int x, int y);

  public:
    MapPanel(const TileServer &server, QWidget *parent = nullptr);
    ~MapPanel();

    const TileServer &tileServer() const;

    const QPoint &mousePosition() const;
    void setMousePosition(const QPoint &position);

    const QPoint &mapPosition() const;
    void setMapPosition(QPoint p);
    void setMapPositionCentered(QPoint p);

    int zoom() const;
    void setZoom(int zoom);
    void zoomInOut(QPoint pivot, int delta);

    bool debug() const;
    void setDebug(bool debug);

    int tileCacheSize() const;

  protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

  signals:
    void mousePositionChanged(const QPoint &position);
    void zoomChanged(int oldZoom, int zoom);
    void tileCacheSizeChanged(int newSize);
};

#endif // MAPPANEL_H
