#ifndef UTIL_H
#define UTIL_H

#include <QLabel>
#include <QPushButton>

struct Util {
    static void setLucideIcon(QPushButton *button, const QString &iconName);
    static void setLucideIcon(QLabel *label, const QString &iconName);
    static void applyButtonStyle(QPushButton *button);
    static void applyFlatButtonStyle(QPushButton *button);
    static void applyButtonHover(QPushButton *button);
};

#endif // UTIL_H
