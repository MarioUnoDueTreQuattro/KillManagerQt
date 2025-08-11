#-------------------------------------------------
#
# Project created by QtCreator 2025-07-24T04:32:40
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets winextras

TARGET = KillManagerQt
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    addexedialog.cpp \
    myqlistwidget.cpp \
    applicationitemslist.cpp \
    utility.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    addexedialog.h \
    myqlistwidget.h \
    applicationitemslist.h \
    utility.h

FORMS    += mainwindow.ui \
    dialog.ui \
    addexedialog.ui

RESOURCES += \
    icons.qrc

win32:RC_ICONS = KillManager.ico

VERSION = 1.0.0.7
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += Q_FORCE_ASSERTS
win32:LIBS += -lpsapi
