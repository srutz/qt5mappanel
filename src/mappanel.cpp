
#include "mappanel.h"
#include "datafetcher.h"
#include "maputil.h"
#include "markerwidget.h"
#include <QPainter>
#include <QWheelEvent>
#include <cmath>
#include <utility>

using namespace std;

MapPanel::MapPanel(TileServer server, QWidget *parent) : QWidget(parent), m_tileServer(std::move(server))
{
    this->setContentsMargins(0, 0, 0, 0);
    // Enable double buffering to reduce flicker
    setAttribute(Qt::WA_OpaquePaintEvent);
    setAttribute(Qt::WA_NoSystemBackground);
    setMouseTracking(true);

    // Create overlay widget for markers
    m_overlayWidget = new QWidget(this);
    m_overlayWidget->setGeometry(0, 0, width(), height());
    m_overlayWidget->show();
}

const TileServer &MapPanel::tileServer() const { return m_tileServer; }

void MapPanel::setTileServer(const TileServer &server)
{
    if (server.baseUrl == m_tileServer.baseUrl) {
        return;
    }
    m_tileServer = server;
    m_tileCache.clear();
    update();
}

QVector<NominatimResult> MapPanel::markers() const { return m_markers; }

void MapPanel::setMarkers(const QVector<NominatimResult> &markers)
{
    m_markers = markers;
    recreateMarkerWidgets();
    updateMarkerPositions();
    update();
}

const QPoint &MapPanel::mousePosition() const { return m_mousePosition; }

void MapPanel::setMousePosition(const QPoint &position)
{
    if (position == m_mousePosition) {
        return;
    }
    m_mousePosition = position;
    emit mousePositionChanged(m_mousePosition);
}

const QPoint &MapPanel::mapPosition() const { return m_mapPosition; }

void MapPanel::setMapPosition(QPoint p)
{
    if (p == m_mapPosition) {
        return;
    }
    m_mapPosition = p;
    emit mapPositionChanged(p);
    updateMarkerPositions();
    update();
}

void MapPanel::setMapPositionCentered(QPoint p)
{
    int w = this->width();
    int h = this->height();
    setMapPosition(QPoint(p.x() - w / 2, p.y() - h / 2));
}

int MapPanel::zoom() const { return m_zoom; }

void MapPanel::setZoom(int zoom, bool keepCenter)
{
    if (zoom == this->m_zoom) {
        return;
    }
    int oldZoom = this->m_zoom;
    if (keepCenter) {
        QPoint center = m_mapPosition + QPoint(this->width() / 2, this->height() / 2);
        double scale = pow(2.0, zoom - this->m_zoom);
        QPoint newCenter(static_cast<int>(center.x() * scale), static_cast<int>(center.y() * scale));
        m_mapPosition = newCenter - QPoint(this->width() / 2, this->height() / 2);
    }
    this->m_zoom = min(m_tileServer.maxZoom, zoom);
    recreateMarkerWidgets();
    updateMarkerPositions();
    emit zoomChanged(oldZoom, this->m_zoom);
}

void MapPanel::zoomInOut(QPoint pivot, int delta)
{
    if (delta == 0) {
        return;
    }
    QPoint p = mapPosition();
    int dx = pivot.x();
    int dy = pivot.y();
    int newZoom = m_zoom + (delta > 0 ? 1 : -1);
    if (newZoom < 1 || newZoom > m_tileServer.maxZoom) {
        return;
    }
    setZoom(newZoom);
    if (delta > 0) {
        setMapPosition(QPoint(p.x() * 2 + dx, p.y() * 2 + dy));
    } else if (delta < 0) {
        setMapPosition(QPoint((p.x() - dx) / 2, (p.y() - dy) / 2));
    }
}

void MapPanel::zoomToFitMarkers(double paddingFactor)
{
    if (m_markers.isEmpty()) {
        return;
    }

    // Find bounds of all markers at max zoom level for precise calculation
    int maxZoom = m_tileServer.maxZoom;
    double minLat = m_markers[0].lat;
    double maxLat = m_markers[0].lat;
    double minLon = m_markers[0].lon;
    double maxLon = m_markers[0].lon;

    for (const auto &marker : m_markers) {
        minLat = qMin(minLat, marker.lat);
        maxLat = qMax(maxLat, marker.lat);
        minLon = qMin(minLon, marker.lon);
        maxLon = qMax(maxLon, marker.lon);
    }

    // Convert bounds to positions at max zoom
    QPoint topLeft = MapUtil::latLonToPosition(maxLat, minLon, maxZoom);
    QPoint bottomRight = MapUtil::latLonToPosition(minLat, maxLon, maxZoom);

    int boundsWidth = bottomRight.x() - topLeft.x();
    int boundsHeight = bottomRight.y() - topLeft.y();

    // Add padding
    int paddingX = static_cast<int>(boundsWidth * paddingFactor);
    int paddingY = static_cast<int>(boundsHeight * paddingFactor);
    boundsWidth += 2 * paddingX;
    boundsHeight += 2 * paddingY;

    // Find zoom level that fits the bounds in the viewport
    int viewWidth = width();
    int viewHeight = height();
    int targetZoom = maxZoom;

    // Start from max zoom and zoom out until bounds fit
    for (int z = maxZoom; z >= 1; z--) {
        double scale = pow(2.0, z - maxZoom);
        int scaledWidth = static_cast<int>(boundsWidth * scale);
        int scaledHeight = static_cast<int>(boundsHeight * scale);

        if (scaledWidth <= viewWidth && scaledHeight <= viewHeight) {
            targetZoom = z;
            break;
        }
    }

    // Calculate center point
    double centerLat = (minLat + maxLat) / 2.0;
    double centerLon = (minLon + maxLon) / 2.0;
    QPoint centerPos = MapUtil::latLonToPosition(centerLat, centerLon, targetZoom);

    // Apply zoom and center
    setZoom(targetZoom, false);
    setMapPositionCentered(centerPos);
}

bool MapPanel::debug() const { return m_Debug; }

void MapPanel::setDebug(bool debug)
{
    m_Debug = debug;
    update();
    if (m_Debug) {
        m_tileCache.dump();
    }
}

int MapPanel::tileCacheSize() const { return m_tileCache.size(); }

void MapPanel::wheelEvent(QWheelEvent *event)
{
    QPoint p = event->position().toPoint();
    int delta = event->angleDelta().y();
    if (delta > 0) {
        zoomInOut(p, 1);
    } else if (delta < 0) {
        zoomInOut(p, -1);
    }
    event->accept();
}

void MapPanel::mousePressEvent(QMouseEvent *event)
{
    // setMapPositionCentered(event->pos() + mapPosition());
    if (event->modifiers() & Qt::ControlModifier) {
        m_selectionStart = event->pos();
        m_selectionEnd = event->pos();
        setCursor(Qt::CrossCursor);
    } else {
        m_downCoords = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

void MapPanel::mouseMoveEvent(QMouseEvent *event)
{
    if (m_selectionStart.has_value()) {
        m_selectionEnd = event->pos();
        update();
    } else if (m_downCoords.has_value()) {
        QPoint delta = event->pos() - m_downCoords.value();
        setMapPosition(m_mapPosition - delta);
        m_downCoords = event->pos();
        update();
    }
    setMousePosition(event->pos());
}

void MapPanel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (m_selectionStart.has_value() && m_selectionEnd.has_value()) {
        zoomToRectangle(m_selectionStart.value(), m_selectionEnd.value());
        m_selectionStart.reset();
        m_selectionEnd.reset();
        update();
    }
    m_downCoords.reset();
    setCursor(Qt::ArrowCursor);
}

void MapPanel::mouseDoubleClickEvent(QMouseEvent *event) { Q_UNUSED(event); }

void MapPanel::zoomToRectangle(const QPoint &p1, const QPoint &p2)
{
    // Calculate rectangle bounds
    int x1 = qMin(p1.x(), p2.x());
    int y1 = qMin(p1.y(), p2.y());
    int x2 = qMax(p1.x(), p2.x());
    int y2 = qMax(p1.y(), p2.y());
    int rectWidth = x2 - x1;
    int rectHeight = y2 - y1;

    // Ignore very small selections (likely accidental clicks)
    if (rectWidth < 10 || rectHeight < 10) {
        return;
    }

    // Calculate the center of the selection in map coordinates
    QPoint rectCenter((x1 + x2) / 2, (y1 + y2) / 2);
    QPoint mapCenter = m_mapPosition + rectCenter;

    // Calculate zoom level needed to fit the rectangle
    int viewWidth = width();
    int viewHeight = height();

    // Find the zoom level where the selection fits best
    int currentZoom = m_zoom;
    int targetZoom = currentZoom;

    // Calculate how much the selected area needs to scale to fit the view
    double scaleX = static_cast<double>(viewWidth) / rectWidth;
    double scaleY = static_cast<double>(viewHeight) / rectHeight;
    double scale = qMin(scaleX, scaleY) * 0.9; // 0.9 for some margin

    // Convert scale to zoom level change
    int zoomDelta = static_cast<int>(round(log2(scale)));
    targetZoom = qMax(1, qMin(m_tileServer.maxZoom, currentZoom + zoomDelta));

    // Apply zoom and center
    if (targetZoom != currentZoom) {
        double zoomScale = pow(2.0, targetZoom - currentZoom);
        QPoint newMapCenter(static_cast<int>(mapCenter.x() * zoomScale), static_cast<int>(mapCenter.y() * zoomScale));
        setZoom(targetZoom, false);
        setMapPositionCentered(newMapCenter);
    } else {
        setMapPositionCentered(mapCenter);
    }

    update();
}

// see
// https://sourceforge.net/p/mappanel/code/HEAD/tree/com.roots.map/src/com/roots/map/MapPanel.java#l585

void MapPanel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    // qDebug() << "Painting MapPanel with TileServer URL:" << tileServer.baseUrl;
    int width = this->width();
    int height = this->height();
    QPainter painter(this);
    painter.fillRect(0, 0, width, height, Qt::lightGray);

    int x0 = static_cast<int>(floor(static_cast<double>(m_mapPosition.x()) / TILE_SIZE));
    int y0 = static_cast<int>(floor(static_cast<double>(m_mapPosition.y()) / TILE_SIZE));
    int x1 = static_cast<int>(ceil((static_cast<double>(m_mapPosition.x()) + width) / TILE_SIZE));
    int y1 = static_cast<int>(ceil((static_cast<double>(m_mapPosition.y()) + height) / TILE_SIZE));

    int dy = y0 * TILE_SIZE - m_mapPosition.y();
    for (int y = y0; y < y1; ++y) {
        int dx = x0 * TILE_SIZE - m_mapPosition.x();
        for (int x = x0; x < x1; ++x) {
            paintTile(painter, dx, dy, x, y);
            dx += TILE_SIZE;
        }
        dy += TILE_SIZE;
    }

    // Draw selection rectangle if active
    if (m_selectionStart.has_value() && m_selectionEnd.has_value()) {
        QPoint p1 = m_selectionStart.value();
        QPoint p2 = m_selectionEnd.value();
        int x = qMin(p1.x(), p2.x());
        int y = qMin(p1.y(), p2.y());
        int w = qAbs(p2.x() - p1.x());
        int h = qAbs(p2.y() - p1.y());

        painter.setPen(QPen(QColor(0, 120, 215), 2));
        painter.setBrush(QBrush(QColor(0, 120, 215, 50)));
        painter.drawRect(x, y, w, h);
    }
}

void MapPanel::paintTile(QPainter &painter, int dx, int dy, int x, int y)
{
    int xTileCount = 1 << m_zoom;
    int yTileCount = 1 << m_zoom;
    bool tileInBounds = x >= 0 && x < xTileCount && y >= 0 && y < yTileCount;
    // qDebug() << "paintTile: zoom=" << m_zoom << "x=" << x << "y=" << y << "inBounds=" << tileInBounds;
    if (tileInBounds) {
        auto zoom = m_zoom;
        auto cacheEntry = m_tileCache.getTile(TileKey{x, y, zoom});
        if (cacheEntry.has_value() && cacheEntry->image.has_value()) {
            auto image = cacheEntry->image.value();
            painter.drawImage(QRect(dx, dy, TILE_SIZE, TILE_SIZE), image);
        } else if (!cacheEntry.has_value()) {
            // mark as loading
            auto oldTileCacheSize = m_tileCache.size();
            m_tileCache.putTile(TileKey{x, y, zoom}, std::nullopt);
            auto newTileCacheSize = m_tileCache.size();
            if (newTileCacheSize != oldTileCacheSize) {
                emit tileCacheSizeChanged(newTileCacheSize);
            }

            // initiate fetch
            auto format = m_tileServer.baseUrl + "/%1/%2/%3.png";
            auto url = format.arg(zoom).arg(x).arg(y);
            auto fetcher = new DataFetcher(url, this);
            DataFetcher::FetchOptions options{.url = url};
            connect(fetcher, &DataFetcher::responseReceived, this, [this, x, y, zoom, fetcher](const QByteArray &data) {
                QImage image;
                image.loadFromData(data);
                m_tileCache.putTile(TileKey{x, y, zoom}, image);
                this->update();
                fetcher->deleteLater();
            });
            // qDebug() << "initload: zoom=" << zoom << "x=" << x << "y=" << y;
            connect(fetcher, &DataFetcher::error, this, [fetcher](const QString &message) {
                qDebug() << "Error fetching tile:" << message;
                fetcher->deleteLater();
            });
            fetcher->fetch(options);
        }
        if (m_Debug) {
            painter.setPen(Qt::gray);
            painter.drawRect(dx, dy, TILE_SIZE, TILE_SIZE);

            // draw transparent overlay
            painter.fillRect(dx + 4, dy, TILE_SIZE - 8, 24, QColor(0, 0, 0, 140));

            auto tileKey = TileKey{x, y, m_zoom};
            auto cacheEntry = m_tileCache.getTile(tileKey);
            QString s = QString("T zoom=%1, x=%2, y=%3").arg(m_zoom).arg(x).arg(y);
            painter.setPen(Qt::white);
            painter.drawText(dx + 4 + 8, dy + 4 + 12, s);
        }
    }
}

void MapPanel::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (m_overlayWidget != nullptr) {
        m_overlayWidget->setGeometry(0, 0, width(), height());
        updateMarkerPositions();
    }
}

void MapPanel::updateMarkerPositions()
{
    for (auto it = m_markerWidgets.begin(); it != m_markerWidgets.end(); ++it) {
        auto *markerWidget = it.value();
        // first result for positioning (primary location)
        const auto &result = markerWidget->results().first();
        auto markerPos = MapUtil::latLonToPosition(result.lat, result.lon, m_zoom);
        auto screenPos = markerPos - m_mapPosition;
        auto markerWidth = markerWidget->width();
        auto markerHeight = markerWidget->height();
        markerWidget->move(screenPos.x() - markerWidth / 2, screenPos.y() - markerHeight);
        // viewport check
        auto visible = screenPos.x() >= -markerWidth && screenPos.x() <= width() + markerWidth &&
                       screenPos.y() >= -markerHeight && screenPos.y() <= height();
        markerWidget->setVisible(visible);
    }
}

QVector<QVector<NominatimResult>> MapPanel::clusterMarkers() const
{
    auto clusterRadius = m_zoom >= 15 ? 30 : (m_zoom >= 10 ? 50 : 80);
    QVector<QVector<NominatimResult>> clusters;
    QVector<bool> assigned(m_markers.size(), false);
    for (auto i = 0; i < m_markers.size(); ++i) {
        if (assigned[i])
            continue;

        QVector<NominatimResult> cluster;
        cluster.append(m_markers[i]);
        assigned[i] = true;

        auto pos1 = MapUtil::latLonToPosition(m_markers[i].lat, m_markers[i].lon, m_zoom);

        // Find nearby markers to add to this cluster
        for (int j = i + 1; j < m_markers.size(); ++j) {
            if (assigned[j]) {
                continue;
            }
            auto pos2 = MapUtil::latLonToPosition(m_markers[j].lat, m_markers[j].lon, m_zoom);
            int dx = pos2.x() - pos1.x();
            int dy = pos2.y() - pos1.y();
            auto distance = sqrt(dx * dx + dy * dy);
            if (distance <= clusterRadius) {
                cluster.append(m_markers[j]);
                assigned[j] = true;
            }
        }
        clusters.append(cluster);
    }
    return clusters;
}

void MapPanel::recreateMarkerWidgets()
{
    qDeleteAll(m_markerWidgets);
    m_markerWidgets.clear();

    QVector<QVector<NominatimResult>> clusters = clusterMarkers();
    auto clusterIndex = 0;
    for (const QVector<NominatimResult> &cluster : clusters) {
        auto key = cluster.size() == 1 ? QString::number(cluster.first().place_id) : QString("cluster_%1").arg(clusterIndex++);

        auto *markerWidget = new MarkerWidget(cluster, m_overlayWidget);
        markerWidget->adjustSize();
        markerWidget->show();
        m_markerWidgets[key] = markerWidget;
    }
}
