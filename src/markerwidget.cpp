
#include "markerwidget.h"
#include "util.h"
#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

MarkerWidget::MarkerWidget(const NominatimResult &result, QWidget *parent) : QWidget(parent), m_result(result)
{
    auto display = m_result.display_name;
    auto *layout = new QHBoxLayout();
    layout->setContentsMargins(8, 4, 8, 4);
    setLayout(layout);

    auto icon = new QLabel(this);
    icon->setStyleSheet("font-family: 'Lucide'; color: #000000;");
    Util::setLucideIcon(icon, QString::fromUtf8("\uea34"), 18);
    layout->addWidget(icon, 0, Qt::AlignVCenter);

    QString tooltipText;
    if (!m_result.name.isEmpty()) {
        tooltipText += QString("<b>%1</b><br>").arg(m_result.name);
    }
    tooltipText += QString("<b>Address:</b> %1<br>").arg(m_result.display_name);
    tooltipText += QString("<b>Type:</b> %1").arg(m_result.type);
    if (!m_result.addresstype.isEmpty() && m_result.addresstype != m_result.type) {
        tooltipText += QString(" (%1)").arg(m_result.addresstype);
    }
    tooltipText += QString("<br><b>Coordinates:</b> %1, %2<br>").arg(m_result.lat, 0, 'f', 6).arg(m_result.lon, 0, 'f', 6);
    tooltipText += QString("<b>OSM:</b> %1 #%2<br>").arg(m_result.osm_type).arg(m_result.osm_id);
    tooltipText += QString("<b>Place ID:</b> %1").arg(m_result.place_id);

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
