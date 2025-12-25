
#include "marker.h"
#include <QLabel>
#include <QVBoxLayout>

Marker::Marker(const NominatimResult &result, QObject *parent) : QObject(parent), m_result(result)
{
    auto display = m_result.display_name;
    auto *layout = new QVBoxLayout();
    setLayout(layout);

    auto label = new QLabel(this);
    label->setText(display);
    layout->addWidget(label);
}
