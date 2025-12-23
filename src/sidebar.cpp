
#include "sidebar.h"
#include "infoitem.h"
#include "mappanel.h"
#include "maputil.h"
#include "util.h"
#include <QComboBox>
#include <QFont>
#include <QPalette>
#include <QPushButton>
#include <QVBoxLayout>
#include <cmath>

SideBar::SideBar(MapPanel *mapPanel, QWidget *parent) : QWidget(parent), m_mapPanel(mapPanel)
{
    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(9, 27, 13, 0);
    setLayout(layout);

    setAutoFillBackground(true);
    setStyleSheet("font-family: 'Roboto'; QWidget { background-color: #ffffff; }");

    auto header = new QWidget(this);
    auto headerLayout = new QHBoxLayout(header);
    headerLayout->setSpacing(8);
    headerLayout->setContentsMargins(0, 0, 0, 0);

    auto headerIcon = new QLabel(header);
    headerIcon->setStyleSheet("font-family: 'Lucide'; color: #444444;");
    Util::setLucideIcon(headerIcon, QString::fromUtf8("\uea34"), 36);
    headerLayout->addWidget(headerIcon, 0, Qt::AlignVCenter);

    auto headerLabel = new QLabel(header);
    headerLabel->setText(R"(<html>
        <h3>MapPanel</h3>
        )");
    headerLabel->setWordWrap(true);
    headerLayout->addWidget(headerLabel, 1);

    layout->addWidget(header);

    auto mousePositionInfo =
        new InfoItem(InfoValue{.key = "Latitude, Longitude at cursor", .value = "-"}, InfoItem::InfoItemSize::Medium, this);
    connect(m_mapPanel, &MapPanel::mousePositionChanged, this, [mousePositionInfo, this](const QPoint &position) {
        auto coordinate = MapUtil::positionToLatLon(position + m_mapPanel->mapPosition(), m_mapPanel->zoom());
        auto t = QString("%1, %2").arg(coordinate.lat, 0, 'f', 6).arg(coordinate.lon, 0, 'f', 6);
        mousePositionInfo->setValue(t);
    });
    layout->addWidget(mousePositionInfo);
    layout->addSpacing(12);

    auto zoomInfo = new InfoItem(
        InfoValue{.key = "Zoom Level", .value = QString::number(m_mapPanel->zoom())}, InfoItem::InfoItemSize::Medium, this);
    connect(m_mapPanel, &MapPanel::zoomChanged, this, [zoomInfo](int oldZoom, int zoom) {
        Q_UNUSED(oldZoom);
        zoomInfo->setValue(QString::number(zoom));
    });
    layout->addWidget(zoomInfo);
    layout->addSpacing(12);

    auto tileServerLabel = new QLabel("Tile Server:", this);
    Util::applyLabelStyle(tileServerLabel);
    layout->addWidget(tileServerLabel, 0, Qt::AlignLeft);
    auto tileServerChooser = new QComboBox(this);
    for (const auto &server : TILE_SERVERS) {
        tileServerChooser->addItem(server.baseUrl);
    }
    connect(tileServerChooser, &QComboBox::currentTextChanged, this, [this](const QString &baseUrl) {
        for (const auto &server : TILE_SERVERS) {
            if (server.baseUrl == baseUrl) {
                m_mapPanel->setTileServer(server);
                m_mapPanel->setZoom(std::min(server.maxZoom, m_mapPanel->zoom()), true);
                break;
            }
        }
    });
    layout->addWidget(tileServerChooser, 0, Qt::AlignLeft);

    layout->addStretch();

    auto aboutButton = new QPushButton("About Qt-Mappanel", this);
    Util::applyButtonStyle(aboutButton);
    connect(aboutButton, &QPushButton::clicked, this, [this]() { m_sheet->hideSheet(); });
    layout->addWidget(aboutButton, 0, Qt::AlignCenter);
    layout->addSpacing(2);

    auto debugButton = new QPushButton("Toggle Debug", this);
    Util::applyButtonStyle(debugButton);
    connect(debugButton, &QPushButton::clicked, this, [this]() {
        m_mapPanel->setDebug(!m_mapPanel->debug());
        m_sheet->hideSheet();
    });
    layout->addWidget(debugButton, 0, Qt::AlignCenter);
    layout->addSpacing(16);

    setupSheet();
    connect(aboutButton, &QPushButton::clicked, this, [this]() {
        auto centralWidget = this->window()->findChild<QWidget *>("centralWidget");
        this->m_sheet->showSheet(centralWidget, Sheet::Side::Left);
    });
}

SideBar::~SideBar() {}

/* Sets up the sheet and its content
 * the sheet is a sliding sidepanel and the content can be any QWidget
 * To close the sheet from your code invoke hideSheet(true);
 * To open the sheet from your code invoke showSheet(QWidget *destination, QWidget *content);
 */
void SideBar::setupSheet()
{
    auto centralWidget = this->window()->findChild<QWidget *>("centralWidget");
    // setup the sheet's content
    m_sheetContent = new QWidget(centralWidget);
    m_sheetContent->setStyleSheet("background-color: #ffffff; font-family: 'Roboto';");
    auto sheetLayout = new QVBoxLayout(m_sheetContent);
    sheetLayout->setContentsMargins(9, 0, 13, 0);

    auto header = new QLabel(this);
    header->setText(R"(<html>
        <h3>About MapPanel</h3>
        )");
    header->setWordWrap(true);

    sheetLayout->addWidget(header);
    sheetLayout->addSpacing(16);
    sheetLayout->addWidget(new InfoItem(
        InfoValue{.key = "Author", .value = "mailto://stepan.rutz@stepanrutz.com"}, InfoItem::InfoItemSize::Medium,
        m_sheetContent));
    sheetLayout->addWidget(new InfoItem(
        InfoValue{.key = "Sourcecode", .value = "https://github.com/srutz/qt5mappanel"}, InfoItem::InfoItemSize::Medium,
        m_sheetContent));
    sheetLayout->addWidget(new InfoItem(
        InfoValue{.key = "Made with C++/Qt5", .value = "https://www.qt.io/"}, InfoItem::InfoItemSize::Medium, m_sheetContent));

    sheetLayout->addStretch();

    // add another close button to the sheet
    auto sheetButton = new QPushButton("Close", this);
    Util::applyFlatButtonStyle(sheetButton);
    connect(sheetButton, &QPushButton::clicked, this, [this] { this->m_sheet->hideSheet(); });
    sheetLayout->addWidget(sheetButton, 0, Qt::AlignRight);
    sheetLayout->addSpacing(8);

    // create the sheet panel
    m_sheet = new Sheet(m_sheetContent, centralWidget);
    m_sheet->setWidth(360);
    m_sheet->setStyleSheet("QWidget { background-color: #ffffff; }");
}
