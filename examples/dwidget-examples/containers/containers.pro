QT += core gui svg
QT += widgets
QT += dtkcore

TARGET = containers
TEMPLATE = app
CONFIG += c++11

DEFINES += QT_MESSAGELOGCONTEXT

unix {
    QT += dbus
}

!isEmpty(DTK_STATIC_LIB){
    DEFINES += DTK_STATIC_LIB
}

SOURCES += src/main.cxx \
    src/main_window/main_window.cxx \
    src/container_widget/container_widget.cxx

HEADERS += src/container_widget/container_widget.hxx \
    src/main_window/main_window.hxx

RESOURCES += res/containers.qrc

win32* {
    CONFIG += no_lflags_merge
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../../src/release -ldtk2widget
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../../src/debug -ldtk2widgetd
else:unix: LIBS += -L$$OUT_PWD/../../../src -ldtk2widget

INCLUDEPATH += $$PWD/../../../src
INCLUDEPATH += $$PWD/../../../src/widgets
INCLUDEPATH += $$PWD/../../../src/util
INCLUDEPATH += /usr/include/dtk2/DWidget
DEPENDPATH += $$PWD/../../../src

unix {
    QMAKE_RPATHDIR += $$OUT_PWD/../src
    QMAKE_LFLAGS += -Wl,-rpath,\$$ORIGIN/../src
}
