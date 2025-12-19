
#include "infoitem.h"
#include <QLabel>
#include <QVBoxLayout>

InfoItem::InfoItem(const InfoValue &info, InfoItemSize size, QWidget *parent) : QWidget(parent), m_info(info)
{
    QString fontsize;
    switch (size) {
    case InfoItemSize::Small:
        fontsize = "8";
        break;
    case InfoItemSize::Medium:
        fontsize = "10";
        break;
    case InfoItemSize::Large:
        fontsize = "14";
        break;
    }
    auto layout = new QVBoxLayout(this);
    setStyleSheet("background-color: #ffffff; font-family: 'Roboto';");
    layout->setContentsMargins(8, 4, 8, 4);
    layout->setSpacing(1);
    m_keyLabel = new QLabel(this);
    m_keyLabel->setText(m_info.key);
    m_keyLabel->setStyleSheet("color: #555555; font-size: " + fontsize + "px");
    layout->addWidget(m_keyLabel);
    m_valueLabel = new QLabel(this);
    m_valueLabel->setText(m_info.value);
    m_valueLabel->setStyleSheet("color: #000000; font-size: " + fontsize + "px");
    layout->addWidget(m_valueLabel);
}

InfoItem::~InfoItem() {}

InfoValue InfoItem::info() const { return m_info; }

void InfoItem::setInfo(const InfoValue &info)
{
    m_info = info;
    m_keyLabel->setText(m_info.key);
    m_valueLabel->setText(m_info.value);
}
