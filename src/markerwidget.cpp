
#include "markerwidget.h"
#include "util.h"
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>

MarkerWidget::MarkerWidget(const NominatimResult &result, QWidget *parent) : QWidget(parent), m_result(result)
{
    auto display = m_result.display_name;
    auto *layout = new QHBoxLayout();
    layout->setContentsMargins(8, 4, 8, 4);
    setLayout(layout);

    auto icon = new QLabel(this);
    icon->setStyleSheet("font-family: 'Lucide'; color: #000000;");
    Util::setLucideIcon(icon, QString::fromUtf8("\uea34"), 14);
    layout->addWidget(icon, 0, Qt::AlignVCenter);

    // set tooltip
    icon->setToolTip(display);

    /*

    auto label = new QLabel(this);
    label->setText(display);
    label->setStyleSheet(
        "QLabel { background-color: rgba(255, 255, 255, 220); "
        "border: 2px solid #0078d4; "
        "border-radius: 4px; "
        "padding: 4px 8px; "
        "color: black; }");
    label->setWordWrap(true);
    label->setMaximumWidth(250);
    layout->addWidget(label);

    // Add drop shadow effect
    auto *shadowEffect = new QGraphicsDropShadowEffect(this);
    shadowEffect->setBlurRadius(10);
    shadowEffect->setColor(QColor(0, 0, 0, 160));
    shadowEffect->setOffset(2, 2);
    label->setGraphicsEffect(shadowEffect);
    */
}
