#ifndef MAPPANEL_H
#define MAPPANEL_H

#include <QString>
#include <QDebug>
#include <QWidget>
#include <QPoint>
#include "tilecache.h"

const int TILE_SIZE = 256;

struct TileServer
{
    QString baseUrl;
};

class MapPanel : public QWidget
{
    Q_OBJECT

private:
    TileServer tileServer;
    TileCache tileCache;
    QPoint mapPosition = {0, 0};
    int zoom = 8;

    void paintTile(QPainter &painter, int dx, int dy, int x, int y);

public:
    MapPanel(const TileServer &server, QWidget *parent = nullptr);
    ~MapPanel();

protected:
    // override paint
    void paintEvent(QPaintEvent *event) override;
};

#endif // MAPPANEL_H
