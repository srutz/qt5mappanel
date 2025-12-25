#ifndef MAPPANEL_H
#define MAPPANEL_H

#include "constants.h"
#include "nominatim.h"
#include "tilecache.h"
#include <QHash>
#include <QPoint>
#include <QString>
#include <QVector>
#include <QWidget>
#include <optional>

using std::optional;

class MarkerWidget;

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
    optional<QPoint> m_selectionStart;
    optional<QPoint> m_selectionEnd;
    QVector<NominatimResult> m_markers;
    QWidget *m_overlayWidget;
    QHash<QString, MarkerWidget *> m_markerWidgets;

    void paintTile(QPainter &painter, int dx, int dy, int x, int y);
    void updateMarkerPositions();
    void recreateMarkerWidgets();
    void zoomToRectangle(const QPoint &p1, const QPoint &p2);
    QVector<QVector<NominatimResult>> clusterMarkers() const;

  public:
    MapPanel(TileServer server, QWidget *parent = nullptr);
    ~MapPanel() = default;

    const TileServer &tileServer() const;
    void setTileServer(const TileServer &server);

    QVector<NominatimResult> markers() const;
    void setMarkers(const QVector<NominatimResult> &markers);

    const QPoint &mousePosition() const;
    void setMousePosition(const QPoint &position);

    const QPoint &mapPosition() const;
    void setMapPosition(QPoint p);
    void setMapPositionCentered(QPoint p);

    int zoom() const;
    void setZoom(int zoom, bool keepCenter = false);
    void zoomInOut(QPoint pivot, int delta);

    bool debug() const;
    void setDebug(bool debug);

    int tileCacheSize() const;

  protected:
    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

  signals:
    void mousePositionChanged(const QPoint &position);
    void mapPositionChanged(const QPoint &position);
    void zoomChanged(int oldZoom, int zoom);
    void tileCacheSizeChanged(int newSize);
};

#endif // MAPPANEL_H
