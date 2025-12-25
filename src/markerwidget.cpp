
#include "markerwidget.h"
#include "util.h"
#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

MarkerWidget::MarkerWidget(const QVector<NominatimResult> &results, QWidget *parent) : QWidget(parent), m_results(results)
{
    auto *layout = new QHBoxLayout();
    layout->setContentsMargins(8, 4, 8, 4);
    setLayout(layout);

    auto icon = new QLabel(this);
    icon->setStyleSheet("font-family: 'Lucide'; color: #000000;");
    Util::setLucideIcon(icon, QString::fromUtf8("\uea34"), 18);
    layout->addWidget(icon, 0, Qt::AlignVCenter);

    // Show count if multiple results
    if (m_results.size() > 1) {
        auto countLabel = new QLabel(QString::number(m_results.size()), this);
        countLabel->setStyleSheet("font-weight: bold; color: #d40000; font-size: 10px;");
        layout->addWidget(countLabel, 0, Qt::AlignVCenter);
    }

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
