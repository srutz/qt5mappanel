#include "sheet.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QKeySequence>
#include <QPalette>
#include <QPushButton>
#include <QResizeEvent>
#include <QShortcut>
#include <QTimer>
#include <QVBoxLayout>

Sheet::Sheet(QWidget *content, QWidget *parent) : QWidget(parent), m_content(content)
{
    m_destination = nullptr;
    m_backdrop = new SheetAnimatedWidget(this);
    m_sidepanel = new SheetAnimatedWidget(this);
    resize(0, 0);
    if (m_content) {
        m_content->hide();
    }
    m_backdrop->setBackgroundColor(QColor::fromRgbF(0, 0, 0, 0.0));
    m_backdrop->hide();
    m_sidepanel->setStyleSheet("QWidget { background-color: #ffffff; }");

    auto contentLayout = new QVBoxLayout(m_sidepanel);
    contentLayout->setContentsMargins(QMargins(0, 0, 0, 0));
    contentLayout->setSpacing(0);

    m_sidepanel->hide();

    // buttonsbar
    m_buttonBar = new QWidget(this);
    auto buttonBarLayout = new QHBoxLayout(m_buttonBar);
    buttonBarLayout->addStretch();
    buttonBarLayout->setContentsMargins(0, 2, 0, 1);
    auto closeButton = new QPushButton(this);
    connect(closeButton, &QPushButton::clicked, this, [this] { this->hideSheet(true); });
    closeButton->setFlat(true);
    closeButton->setIcon(QIcon::fromTheme("window-close-symbolic"));
    buttonBarLayout->addWidget(closeButton);

    QShortcut *shortcut = new QShortcut(QKeySequence("Escape"), this);
    connect(shortcut, &QShortcut::activated, this, [this] {
        if (m_backdrop->isVisible()) {
            hideSheet(true);
        }
    });
    connect(m_backdrop, &SheetAnimatedWidget::mouseClicked, this, [this](QMouseEvent *event) {
        Q_UNUSED(event);
        hideSheet(true);
    });
}

Sheet::~Sheet() {}

void Sheet::showSheet(QWidget *destination, Side side)
{
    if (!destination) {
        hideSheet(false);
        return;
    }
    m_side = side;
    m_content->show();
    // remove all children from the sidepanel
    while (auto child = m_sidepanel->findChild<QWidget *>()) {
        child->setParent(nullptr);
    }
    m_buttonBar->setParent(m_sidepanel);
    auto contentLayout = dynamic_cast<QVBoxLayout *>(m_sidepanel->layout());
    contentLayout->addWidget(m_buttonBar, 0);
    if (m_content) {
        m_content->setParent(m_sidepanel);
        contentLayout->addWidget(m_content, 1);
    } else {
        contentLayout->addStretch();
    }
    // show the panel backdeop all over the destination widget
    // animate the color, also slide in the sidepanel on top
    // of the backdrop
    m_destination = destination;
    m_backdrop->setParent(destination);
    m_backdrop->show();
    m_sidepanel->setParent(destination);
    m_sidepanel->show();

    m_buttonBar->setVisible(m_showTopRightCloseButton);
    connect(destination, &QWidget::destroyed, [this] { hideSheet(false); });
    QTimer::singleShot(0, [this]() {
        layout(false);
        m_sidepanel->setPosition(QPoint(m_side == Side::Left ? -m_width : m_backdrop->width(), 0));
        layout(true);
        m_sidepanel->setPosition(QPoint(m_side == Side::Left ? -m_width : m_backdrop->width(), 0));
        m_backdrop->setBackgroundColor(QColor::fromRgb(0, 0, 0, 0));
    });
}

void Sheet::hideSheet(bool animated)
{
    m_destination = nullptr;
    if (animated) {
        m_sidepanel->setPositionA(QPoint(m_side == Side::Left ? -m_width : m_backdrop->width(), 0), m_hideDurationMs, [this] {
            m_backdrop->hide();
            m_sidepanel->hide();
        });
        m_backdrop->setBackgroundColorA(QColor::fromRgb(0, 0, 0, 0), m_hideDurationMs);
    } else {
        m_sidepanel->setPosition(QPoint(m_side == Side::Left ? -m_width : m_backdrop->width(), 0));
        m_backdrop->hide();
        m_sidepanel->hide();
        m_backdrop->setBackgroundColor(QColor::fromRgb(0, 0, 0, 0));
    }
}

void Sheet::layout(bool animated)
{
    if (m_destination) {
        // resize the backdrop to full size of the destination widget
        m_backdrop->resize(m_destination->size());
        m_backdrop->move(0, 0);
        // layout the sidepanel on the outside so that it can slide in
        const auto panelWidth = m_width;
        m_sidepanel->resize(panelWidth, m_backdrop->size().height());
        if (animated) {
            m_sidepanel->setPositionA(QPoint(m_side == Side::Left ? 0 : m_backdrop->width() - panelWidth, 0), m_showDurationMs);
            m_backdrop->setBackgroundColorA(QColor::fromRgb(0, 0, 0, 192), m_showDurationMs);
        } else {
            m_sidepanel->setPosition(QPoint(m_side == Side::Left ? 0 : m_backdrop->width() - panelWidth, 0));
            m_backdrop->setBackgroundColor(QColor::fromRgb(0, 0, 0, 192));
        }
    }
}
