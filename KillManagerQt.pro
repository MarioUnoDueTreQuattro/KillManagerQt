#-------------------------------------------------
#
# Project created by QtCreator 2025-07-24T04:32:40
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = KillManagerQt
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    applicationitem.cpp \
    addexedialog.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    applicationitem.h \
    addexedialog.h

FORMS    += mainwindow.ui \
    dialog.ui \
    addexedialog.ui

RESOURCES += \
    icons.qrc

RC_ICONS = KillManager.ico

VERSION = 1.0.0.2
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32:LIBS += -lpsapi
