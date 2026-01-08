QT       += core gui network widgets

CONFIG += c++23
QMAKE_CXXFLAGS += -Wunused

TARGET = qt5mappanel
TEMPLATE = app

# The following define makes your compiler warn you if you use any
# feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API for details on how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/mappanel.cpp \
    src/animatedwidget.cpp \
    src/datafetcher.cpp \
    src/infoitem.cpp \
    src/maputil.cpp \
    src/markerwidget.cpp \
    src/nominatim.cpp \
    src/searchpanel.cpp \
    src/sheet.cpp \
    src/sidebar.cpp \
    src/tilecache.cpp \
    src/util.cpp

HEADERS += \
    src/mainwindow.h \
    src/mappanel.h \
    src/animatedwidget.h \
    src/constants.h \
    src/datafetcher.h \
    src/infoitem.h \
    src/maputil.h \
    src/markerwidget.h \
    src/nominatim.h \
    src/raiiguard.h \
    src/searchpanel.h \
    src/sheet.h \
    src/sidebar.h \
    src/tilecache.h \
    src/util.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src/commonresources.qrc
