
#include "markerwidget.h"
#include <QLabel>
#include <QVBoxLayout>

MarkerWidget::MarkerWidget(const NominatimResult &result, QWidget *parent) : QWidget(parent), m_result(result)
{
    auto display = m_result.display_name;
    auto *layout = new QVBoxLayout();
    setLayout(layout);

    auto label = new QLabel(this);
    label->setText(display);
    layout->addWidget(label);
}
