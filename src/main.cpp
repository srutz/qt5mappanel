#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QPalette>
#include <QScreen>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QString s1 = "frank";
    QString s2 = s1;

    QApplication::setStyle(QStyleFactory::create("Fusion"));
    QApplication a(argc, argv);
    MainWindow w;
    w.resize(1100, 800);
    w.setWindowTitle("Qt Mappanel");
    auto *screen = QGuiApplication::primaryScreen();
    auto screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - w.width()) / 2;
    int y = (screenGeometry.height() - w.height()) / 2;
    w.move(x, y);
    w.show();
    return QApplication::exec();
}
