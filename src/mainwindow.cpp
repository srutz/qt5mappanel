#include "mainwindow.h"
#include "maputil.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setStyleSheet("background-color: white;");

    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(layout);

    auto leftPanel = new QWidget(centralWidget);
    leftPanel->setFixedWidth(48);
    leftPanel->setStyleSheet("background-color: #ffffff; border-right: 1px solid #cccccc;");
    layout->addWidget(leftPanel);

    auto mapPanel = new MapPanel(TileServer{.baseUrl = "https://tile.openstreetmap.org"}, centralWidget);
    mapPanel->setZoom(13);
    mapPanel->setMapPositionCentered(MapUtil::latLonToTileXY(51.4778684, -0.004053, mapPanel->getZoom()));
    layout->addWidget(mapPanel);
}

void MainWindow::resizeEvent(QResizeEvent *event) { QMainWindow::resizeEvent(event); }

MainWindow::~MainWindow() {}
