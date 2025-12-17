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
    auto layout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(layout);

    auto mapPanel = new MapPanel(TileServer{.baseUrl = "https://tile.openstreetmap.org"});
    mapPanel->setMapPositionCentered(MapUtil::latLonToTileXY(51.51, -0.11, mapPanel->getZoom()));
    layout->addWidget(mapPanel);
}

void MainWindow::resizeEvent(QResizeEvent *event) { QMainWindow::resizeEvent(event); }

MainWindow::~MainWindow() {}
