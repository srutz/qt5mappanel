
#ifndef MARKERWIDGET_H
#define MARKERWIDGET_H

#include "nominatim.h"
#include <QVector>
#include <QWidget>

class MarkerWidget : public QWidget
{
    Q_OBJECT

  public:
    MarkerWidget(const QVector<NominatimResult> &results, QWidget *parent);
    ~MarkerWidget() = default;

    const QVector<NominatimResult> &results() const { return m_results; }
    int count() const { return m_results.size(); }

  protected:
    bool event(QEvent *event) override;

  private:
    QVector<NominatimResult> m_results;
};

#endif // MARKERWIDGET_H