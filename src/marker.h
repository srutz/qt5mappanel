
#ifndef MARKER_H
#define MARKER_H

#include "nominatim.h"
#include <QObject>

class Marker : public QObject
{
    Q_OBJECT

  public:
    Marker(const NominatimResult &result, QObject *parent);
    ~Marker() = default;

    const NominatimResult &result() const { return m_result; }

  private:
    NominatimResult m_result;
};

#endif // MARKER_H