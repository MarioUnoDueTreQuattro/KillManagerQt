#-------------------------------------------------
#
# Project created by QtCreator 2025-07-24T04:32:40
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KillManagerQt
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    addexedialog.cpp \
    runningprocesseslist.cpp \
    myqlistwidget.cpp \
    applicationitemlist.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    addexedialog.h \
    runningprocesseslist.h \
    log_macros.h \
    myqlistwidget.h \
    applicationitemlist.h

FORMS    += mainwindow.ui \
    dialog.ui \
    addexedialog.ui

RESOURCES += \
    icons.qrc

win32:RC_ICONS = KillManager.ico

VERSION = 1.0.0.4
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:LIBS += -lpsapi
