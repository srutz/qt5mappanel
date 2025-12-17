
#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "sheet.h"
#include <QWidget>

class SideBar : public QWidget
{
    Q_OBJECT

    Sheet *m_sheet;
    QWidget *m_sheetContent;
    void setupSheet();

  public:
    SideBar(QWidget *parent = nullptr);
    ~SideBar();
};

#endif // SIDEBAR_H
