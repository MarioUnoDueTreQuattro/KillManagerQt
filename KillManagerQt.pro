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
win32:RC_ICONS = KillManager.ico

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    addexedialog.cpp \
    myqlistwidget.cpp \
    applicationitemslist.cpp \
    processitemslist.cpp \
    utility.cpp \
    scheduler.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    addexedialog.h \
    myqlistwidget.h \
    applicationitemslist.h \
    processitemslist.h \
    utility.h \
    scheduler.h

FORMS    += mainwindow.ui \
    dialog.ui \
    addexedialog.ui

#RC_FILE = KillManagerQt.manifest

RC_FILE = app.rc

RESOURCES += \
    icons.qrc

VERSION = 1.0.0.8
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
win32:LIBS += -lpsapi

DISTFILES += \
    KillManagerQt.manifest \
    app.rc
