#ifndef UTIL_H
#define UTIL_H

#include <QLabel>
#include <QPushButton>

struct Util {
    static void setLucideIcon(QPushButton *button, const QString &iconName, int fontSize = 16);
    static void setLucideIcon(QLabel *label, const QString &iconName, int fontSize = 16);
    static void applyButtonStyle(QPushButton *button);
    static void applyFlatButtonStyle(QPushButton *button);
    static void applyButtonHover(QPushButton *button);
    static void applyLabelStyle(QLabel *label, int fontSize = 15);
};

#endif // UTIL_H
