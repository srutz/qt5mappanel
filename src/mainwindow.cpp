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
    int fontId = QFontDatabase::addApplicationFont(":/common/resources/fonts/Lucide.ttf");
    if (fontId != -1) {
        QString iconFontFamily = QFontDatabase::applicationFontFamilies(fontId).first();
        qDebug() << "Icon font loaded:" << iconFontFamily;
    } else {
        qWarning("Failed to load icon font.");
        exit(1);
    }

    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    centralWidget->setLayout(layout);

    auto sideBar = new SideBar(centralWidget);
    layout->addWidget(sideBar);

    auto mapPanel = new MapPanel(TileServer{.baseUrl = "https://tile.openstreetmap.org"}, centralWidget);
    mapPanel->setZoom(13);
    mapPanel->setMapPositionCentered(MapUtil::latLonToTileXY(51.4778684, -0.004053, mapPanel->zoom()));
    layout->addWidget(mapPanel);
}

void MainWindow::resizeEvent(QResizeEvent *event) { QMainWindow::resizeEvent(event); }

MainWindow::~MainWindow() {}
