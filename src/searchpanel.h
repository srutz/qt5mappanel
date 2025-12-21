#ifndef SEARCHPANEL_H
#define SEARCHPANEL_H

#include "mappanel.h"
#include <QWidget>

class SearchPanel : public QWidget
{
    Q_OBJECT
    MapPanel *m_mapPanel;

  public:
    explicit SearchPanel(MapPanel *mapPanel, QWidget *parent = nullptr);
    ~SearchPanel();
};

#endif // SEARCHPANEL_H
