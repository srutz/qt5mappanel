#ifndef SEARCHPANEL_H
#define SEARCHPANEL_H

#include "mappanel.h"
#include "nominatim.h"
#include <QVector>
#include <QWidget>

class SearchPanel : public QWidget
{
    Q_OBJECT
    MapPanel *m_mapPanel;
    QVector<NominatimResult> m_results;

  public:
    explicit SearchPanel(MapPanel *mapPanel, QWidget *parent = nullptr);
    ~SearchPanel();

    QVector<NominatimResult> results() const;
    void setResults(const QVector<NominatimResult> &results);
};

#endif // SEARCHPANEL_H
