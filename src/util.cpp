#include "util.h"

void Util::setLucideIcon(QPushButton *button, const QString &iconName, int fontSize)
{
    button->setFont(QFont("Lucide", fontSize));
    button->setText(iconName);
}

void Util::setLucideIcon(QLabel *label, const QString &iconName, int fontSize)
{
    label->setFont(QFont("Lucide", fontSize));
    label->setText(iconName);
}

void Util::applyButtonStyle(QPushButton *button)
{
    button->setMinimumWidth(160);
    button->setStyleSheet(
        "QPushButton {"
        "    background-color: #000000;"
        "    border: 1px solid #1c1c1c;"
        "    border-radius: 4px;"
        "    padding: 8px;"
        "    font-size: 14px;"
        "    font-weight: 500;"
        "    color: #ffffff;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2f2f2f;"
        "    border-color: #202020;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1f1f1f;"
        "    border-color: #303030;"
        "}"
        "QPushButton:focus {"
        "    outline: none;"
        "}");
}

void Util::applyFlatButtonStyle(QPushButton *button)
{
    button->setMinimumWidth(112);
    button->setStyleSheet(
        "QPushButton {"
        "    border: 1px solid #d4d4d4;"
        "    border-radius: 4px;"
        "    padding: 8px;"
        "    font-size: 14px;"
        "    font-weight: 500;"
        "    color: #0f0f0f;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e5e5e5;"
        "    border-color: #d4d4d4;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #a3a3a3;"
        "    border-color: #d4d4d4;"
        "}"
        "QPushButton:disabled {"
        "    background-color: #f0f0f0;"
        "    border-color: #d4d4d4;"
        "    color: #a0a0a0;"
        "}"
        "QPushButton:focus {"
        "    outline: none;"
        //"    box-shadow: 0 0 0 2px rgba(66, 165, 245, 0.5);"
        "}");
}

void Util::applyButtonHover(QPushButton *button)
{
    button->setStyleSheet(
        "QPushButton {"
        "    border: none;"
        "}"
        "QPushButton:hover {"
        "    background-color: #f5f5f5;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #e5e5e5;"
        "}");
}

void Util::applyLabelStyle(QLabel *label, int fontSize)
{
    label->setStyleSheet(QString(
                             "QLabel {"
                             "    color: #555555;"
                             "    font-size: %1px;"
                             "}")
                             .arg(fontSize - 1));
}