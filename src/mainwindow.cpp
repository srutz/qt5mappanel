#include "mainwindow.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimer>
#include <QMainWindow>
#include <QResizeEvent>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setStyleSheet("background-color: white;");

    auto centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    auto layout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(layout);

    auto mapPanel = new MapPanel(TileServer{.baseUrl = "https://tile.openstreetmap.org"});
    layout->addWidget(mapPanel);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}

MainWindow::~MainWindow()
{
}
