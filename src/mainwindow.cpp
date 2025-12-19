#include "mainwindow.h"
#include "mappanel.h"
#include "maputil.h"
#include "sidebar.h"
#include "util.h"
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    for (const auto &family : {":/common/resources/fonts/Lucide.ttf", ":/common/resources/fonts/Roboto-Regular.ttf"}) {
        int fontId = QFontDatabase::addApplicationFont(family);
        if (fontId == -1) {
            qWarning() << "Failed to load font:" << family;
            exit(1);
        }
    }

    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    centralWidget->setLayout(layout);

    auto mapPanel = new MapPanel(TileServer{.baseUrl = "https://tile.openstreetmap.org"}, centralWidget);
    auto sideBar = new SideBar(mapPanel, centralWidget);
    layout->addWidget(sideBar);

    mapPanel->setZoom(6);
    mapPanel->setMapPositionCentered(MapUtil::latLonToTileXY(51.4778684, -0.004053, mapPanel->zoom()));
    layout->addWidget(mapPanel);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    auto sheet = findChild<Sheet *>();
    if (sheet) {
        sheet->layout(false);
    }
}

MainWindow::~MainWindow() {}
