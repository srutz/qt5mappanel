#include "mainwindow.h"
#include "mappanel.h"
#include "maputil.h"
#include "searchpanel.h"
#include "sidebar.h"
#include "util.h"
#include <QDebug>
#include <QFontDatabase>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QResizeEvent>
#include <QSplitter>
#include <QString>
#include <QTabWidget>
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
    centralWidget->setObjectName("centralWidget");
    setCentralWidget(centralWidget);

    auto layout = new QHBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    centralWidget->setLayout(layout);

    auto content = new QWidget(centralWidget);
    layout->addWidget(content);

    auto contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);
    content->setLayout(contentLayout);

    auto splitter = new QSplitter(Qt::Horizontal, content);
    contentLayout->addWidget(splitter);

    auto mapPanel = new MapPanel(TILE_SERVERS[0], centralWidget);
    mapPanel->setZoom(6);
    // mapPanel->setMapPositionCentered(MapUtil::latLonToTileXY(51.4778684, -0.004053, mapPanel->zoom()));
    QTimer::singleShot(1, this, [mapPanel]() {
        mapPanel->setMapPositionCentered(MapUtil::latLonToPosition(51.5074, -0.1278, mapPanel->zoom()));
    });

    auto tabs = new QTabWidget(splitter);
    tabs->setTabPosition(QTabWidget::South);

    auto sideBar = new SideBar(mapPanel, splitter);
    auto searchPanel = new SearchPanel(mapPanel, splitter);
    tabs->addTab(searchPanel, "Search");
    tabs->addTab(sideBar, "Control");
    auto tabContainer = new QWidget(this);
    tabContainer->setMaximumWidth(460);
    auto tabContainerLayout = new QVBoxLayout(tabContainer);
    tabContainerLayout->setContentsMargins(9, 9, 9, 9);
    tabContainerLayout->addWidget(tabs);

    splitter->addWidget(tabContainer);
    splitter->addWidget(mapPanel);

    splitter->setSizes({200, 800});
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
