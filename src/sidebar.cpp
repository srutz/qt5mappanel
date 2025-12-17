
#include "sidebar.h"
#include "mappanel.h"
#include "util.h"
#include <QFont>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

SideBar::SideBar(QWidget *parent) : QWidget(parent)
{
    setFixedWidth(36);
    setStyleSheet("background-color: #ffffff; border-right: 1px solid #e7e7e7;");

    // Set Roboto-Regular font for this widget
    QFont robotoFont("Roboto");
    setFont(robotoFont);

    auto layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    setLayout(layout);

    auto menuButton = new QPushButton(this);
    menuButton->setFlat(true);
    Util::setLucideIcon(menuButton, QString::fromUtf8("\uea3a"));
    layout->addWidget(menuButton);
    layout->addStretch();

    setupSheet();

    connect(menuButton, &QPushButton::clicked, this, [this]() {
        auto centralWidget = this->parentWidget();
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
    // setup the sheet's content
    m_sheetContent = new QWidget(this);
    m_sheetContent->setStyleSheet("background-color: #ffffff;");
    auto sheetLayout = new QVBoxLayout(m_sheetContent);
    sheetLayout->setContentsMargins(9, 0, 13, 0);

    auto actions = new QWidget(this);
    auto actionsLayout = new QVBoxLayout(actions);
    actionsLayout->setContentsMargins(0, 0, 0, 0);
    actionsLayout->setSpacing(8);

    auto settingsButton = new QPushButton("Toggle Debug", this);
    Util::applyButtonStyle(settingsButton);
    connect(settingsButton, &QPushButton::clicked, this, [this]() {
        auto parentMapPanel = this->parentWidget()->findChild<MapPanel *>();
        if (parentMapPanel) {
            parentMapPanel->setDebug(!parentMapPanel->debug());
        }
        m_sheet->hideSheet();
    });
    actionsLayout->addWidget(settingsButton);

    auto aboutButton = new QPushButton("About Qt-Mappanel", this);
    Util::applyButtonStyle(aboutButton);
    connect(aboutButton, &QPushButton::clicked, this, [this]() {
        QTimer::singleShot(50, this, [this]() {

        });
        m_sheet->hideSheet();
    });
    actionsLayout->addWidget(aboutButton);

    auto header = new QLabel(this);
    header->setText(R"(<html>
        <h3>Qt5 based Tile-Viewer</h3>
        <p>stepan.rutz@stepanrutz.com</p>
        )");
    header->setWordWrap(true);

    sheetLayout->addWidget(header);
    sheetLayout->addSpacing(16);
    sheetLayout->addWidget(actions, 0, Qt::AlignHCenter);
    sheetLayout->addStretch();

    // add another close button to the sheet
    auto sheetButton = new QPushButton("Close", this);
    Util::applyFlatButtonStyle(sheetButton);
    connect(sheetButton, &QPushButton::clicked, this, [this] { this->m_sheet->hideSheet(); });
    sheetLayout->addWidget(sheetButton, 0, Qt::AlignRight);
    sheetLayout->addSpacing(8);

    // create the sheet panel
    m_sheet = new Sheet(m_sheetContent, this);
    m_sheet->setWidth(280);
    m_sheet->setStyleSheet("QWidget { background-color: #ffffff; }");
}
