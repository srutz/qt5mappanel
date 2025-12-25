
#include "markerwidget.h"
#include "util.h"
#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

MarkerWidget::MarkerWidget(const QVector<NominatimResult> &results, QWidget *parent) : QWidget(parent), m_results(results)
{
    // For clusters, we'll draw custom graphics in paintEvent
    // Set a fixed size for the marker
    if (m_results.size() > 1) {
        // Cluster marker size
        setFixedSize(50, 50);
    } else {
        // Single marker size
        setFixedSize(30, 30);
    }

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground);

    // Build comprehensive tooltip for all results
    QString tooltipText;

    if (m_results.size() == 1) {
        const NominatimResult &result = m_results.first();
        if (!result.name.isEmpty()) {
            tooltipText += QString("<b>%1</b><br>").arg(result.name);
        }
        tooltipText += QString("<b>Address:</b> %1<br>").arg(result.display_name);
        tooltipText += QString("<b>Type:</b> %1").arg(result.type);
        if (!result.addresstype.isEmpty() && result.addresstype != result.type) {
            tooltipText += QString(" (%1)").arg(result.addresstype);
        }
        tooltipText += QString("<br><b>Coordinates:</b> %1, %2<br>").arg(result.lat, 0, 'f', 6).arg(result.lon, 0, 'f', 6);
        tooltipText += QString("<b>OSM:</b> %1 #%2<br>").arg(result.osm_type).arg(result.osm_id);
        tooltipText += QString("<b>Place ID:</b> %1").arg(result.place_id);
    } else {
        tooltipText += QString("<b style='font-size:12px;'>%1 Locations</b><hr>").arg(m_results.size());

        for (int i = 0; i < m_results.size(); ++i) {
            const NominatimResult &result = m_results.at(i);

            if (i > 0) {
                tooltipText += "<hr>";
            }

            tooltipText += QString("<b>%1.</b> ").arg(i + 1);

            if (!result.name.isEmpty()) {
                tooltipText += QString("<b>%1</b><br>").arg(result.name);
            }

            tooltipText += QString("&nbsp;&nbsp;<b>Address:</b> %1<br>").arg(result.display_name);
            tooltipText += QString("&nbsp;&nbsp;<b>Type:</b> %1").arg(result.type);

            if (!result.addresstype.isEmpty() && result.addresstype != result.type) {
                tooltipText += QString(" (%1)").arg(result.addresstype);
            }

            tooltipText +=
                QString("<br>&nbsp;&nbsp;<b>Coords:</b> %1, %2<br>").arg(result.lat, 0, 'f', 6).arg(result.lon, 0, 'f', 6);
            tooltipText += QString("&nbsp;&nbsp;<b>OSM:</b> %1 #%2 | <b>Place ID:</b> %3")
                               .arg(result.osm_type)
                               .arg(result.osm_id)
                               .arg(result.place_id);

            if (i < m_results.size() - 1) {
                tooltipText += "<br>";
            }
        }
    }

    setToolTip(tooltipText);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

bool MarkerWidget::event(QEvent *event)
{
    // Accept hover and tooltip events to show tooltips
    if (event->type() == QEvent::ToolTip || event->type() == QEvent::HoverEnter || event->type() == QEvent::HoverMove ||
        event->type() == QEvent::HoverLeave) {
        return QWidget::event(event);
    }

    // For mouse events, accept them to prevent passing through to parent
    // This allows tooltips to work while still showing the marker
    if (event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease ||
        event->type() == QEvent::MouseMove) {
        // Accept the event to stop propagation, but don't do anything with it
        event->accept();
        return true;
    }

    return QWidget::event(event);
}

void MarkerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_results.size() > 1) {
        // Draw cluster marker with 3 shaded circles
        int centerX = width() / 2;
        int centerY = height() / 2;

        QColor color1(41, 128, 185);
        QColor color2(52, 152, 219, 180);
        QColor color3(133, 193, 233, 120);

        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(color3));
        painter.drawEllipse(QPointF(centerX, centerY), 22, 22);

        painter.setBrush(QBrush(color2));
        painter.drawEllipse(QPointF(centerX, centerY), 17, 17);

        painter.setBrush(QBrush(color1));
        painter.drawEllipse(QPointF(centerX, centerY), 12, 12);

        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setPixelSize(12);
        font.setBold(true);
        painter.setFont(font);

        QString countText = QString::number(m_results.size());
        QFontMetrics fm(font);
        int textWidth = fm.horizontalAdvance(countText);
        int textHeight = fm.height();
        painter.drawText(centerX - textWidth / 2, centerY + textHeight / 4, countText);

    } else {
        // Draw single marker - simple pin shape
        int centerX = width() / 2;
        int pinBottom = height() - 2;
        int pinTop = 8;

        // Create pin path
        QPainterPath path;
        path.addEllipse(QPointF(centerX, pinTop + 6), 8, 8);

        // Add pin point
        QPainterPath triangle;
        triangle.moveTo(centerX, pinBottom);
        triangle.lineTo(centerX - 4, pinTop + 12);
        triangle.lineTo(centerX + 4, pinTop + 12);
        triangle.closeSubpath();
        path.addPath(triangle);

        // Draw pin with shadow
        painter.setPen(QPen(QColor(100, 100, 100, 100), 1));
        painter.setBrush(QBrush(QColor(220, 53, 69)));
        painter.drawPath(path);

        // Draw inner circle
        painter.setPen(Qt::NoPen);
        painter.setBrush(QBrush(Qt::white));
        painter.drawEllipse(QPointF(centerX, pinTop + 6), 4, 4);
    }
}
