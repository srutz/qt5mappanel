#ifndef SEARCHPANEL_H
#define SEARCHPANEL_H

#include "mappanel.h"
#include <QTableView>
#include <QWidget>

class SearchPanel : public QWidget
{
    Q_OBJECT
    MapPanel *m_mapPanel;
    QTableView *m_resultsTable;

    void applyResults(const QVector<NominatimResult> &results);

  public:
    explicit SearchPanel(MapPanel *mapPanel, QWidget *parent = nullptr);
    ~SearchPanel();
};

#endif // SEARCHPANEL_H
