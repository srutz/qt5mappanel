
#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "mappanel.h"
#include "sheet.h"
#include <QWidget>

class SideBar : public QWidget
{
    Q_OBJECT

    MapPanel *m_mapPanel;
    Sheet *m_sheet;
    QWidget *m_sheetContent;
    void setupSheet();

  public:
    SideBar(MapPanel *mapPanel, QWidget *parent = nullptr);
    ~SideBar();
};

#endif // SIDEBAR_H
