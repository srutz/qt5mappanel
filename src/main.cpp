#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    // set Ui-theme
    // List available styles
    QStringList styles = QStyleFactory::keys();
    qDebug() << "Available styles:" << styles;

    // Apply a style
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QApplication a(argc, argv);
    MainWindow w;
    // center the window on the screen
    w.resize(800, 600);
    auto screen = QGuiApplication::primaryScreen();
    auto screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - w.width()) / 2;
    int y = (screenGeometry.height() - w.height()) / 2;
    w.move(x, y);
    w.show();
    return a.exec();
}
