
#ifndef MARKERWIDGET_H
#define MARKERWIDGET_H

#include "nominatim.h"
#include <QWidget>

class MarkerWidget : public QWidget
{
    Q_OBJECT

  public:
    MarkerWidget(const NominatimResult &result, QWidget *parent);
    ~MarkerWidget() = default;

    const NominatimResult &result() const { return m_result; }

  private:
    NominatimResult m_result;
};

#endif // MARKERWIDGET_H