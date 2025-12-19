
#include "infoitem.h"
#include <QDesktopServices>
#include <QLabel>
#include <QUrl>
#include <QVBoxLayout>

InfoItem::InfoItem(const InfoValue &info, InfoItemSize size, QWidget *parent) : QWidget(parent), m_info(info)
{
    QString fontsize;
    switch (size) {
    case InfoItemSize::Small:
        fontsize = "8";
        break;
    case InfoItemSize::Medium:
        fontsize = "15";
        break;
    case InfoItemSize::Large:
        fontsize = "18";
        break;
    }
    auto layout = new QVBoxLayout(this);
    setStyleSheet("background-color: #ffffff; font-family: 'Roboto';");
    layout->setContentsMargins(0, 4, 0, 4);
    layout->setSpacing(1);
    m_keyLabel = new QLabel(this);
    m_keyLabel->setText(m_info.key);
    m_keyLabel->setStyleSheet("color: #555555; font-size: " + fontsize + "px");
    layout->addWidget(m_keyLabel);
    m_valueLabel = new QLabel(this);
    m_valueLabel->setText(m_info.value);
    m_valueLabel->setStyleSheet("color: #000000; font-size: " + fontsize + "px");
    m_valueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
    layout->addWidget(m_valueLabel);
    rehash();
}

InfoItem::~InfoItem() {}

InfoValue InfoItem::info() const { return m_info; }

void InfoItem::setInfo(const InfoValue &info)
{
    m_info = info;
    m_keyLabel->setText(m_info.key);
    m_valueLabel->setText(m_info.value);
    rehash();
}

void InfoItem::rehash()
{
    if (m_info.value.toLower().startsWith("https://") || m_info.value.toLower().startsWith("mailto://")) {
        // make it responds to clicks like a link and open in system-default
        m_valueLabel->setTextFormat(Qt::RichText);
        m_valueLabel->setTextInteractionFlags(Qt::TextBrowserInteraction);
        m_valueLabel->setOpenExternalLinks(true);
        QString linkText = m_info.value;
        if (m_info.value.toLower().startsWith("mailto://")) {
            linkText = "mailto:" + m_info.value.mid(8);
        }
        m_valueLabel->setText(QString("<a href=\"%1\">%2</a>").arg(linkText, m_info.value));
    } else {
        m_valueLabel->setTextFormat(Qt::PlainText);
        m_valueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
        m_valueLabel->setText(m_info.value);
    }
}
