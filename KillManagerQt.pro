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

# Show warnings and enable debugging symbols
QMAKE_CXXFLAGS += -Wall -Wextra
QMAKE_CXXFLAGS_DEBUG += -g

## Aggiungi le opzioni di ottimizzazione solo per le build di rilascio
#CONFIG(release, debug|release) {
#    QMAKE_CXXFLAGS_RELEASE -= -O2
#    QMAKE_CXXFLAGS_RELEASE += -O3 -march=native
#    # Usa -march=native solo se il programma sarà eseguito solo sulla tua macchina
#    # Se devi distribuirlo, è meglio rimuoverlo.
#    # QMAKE_CXXFLAGS_RELEASE += -march=native
#}

# Aggiungi un singolo set di flag per i warning per evitare duplicati
QMAKE_CXXFLAGS += -Wall -Wextra

#DEFINES += QT_NO_DEBUG_OUTPUT

# Optional: Enable logging macros
#DEFINES += QT_MESSAGELOGCONTEXT

# Optional: Use separate build folders for debug/release
#CONFIG += debug_and_release
#CONFIG += build_all

# Enable debug output
#CONFIG += debug

SOURCES += main.cpp\
        mainwindow.cpp \
    dialog.cpp \
    addexedialog.cpp \
    myqlistwidget.cpp \
    applicationitemslist.cpp \
    processitemslist.cpp \
    utility.cpp \
    scheduler.cpp \
    SimpleZipper/src/SimpleZipper.cxx \
    SimpleZipper/miniz/miniz.c \
    zipfiles.cpp

HEADERS  += mainwindow.h \
    dialog.h \
    addexedialog.h \
    myqlistwidget.h \
    applicationitemslist.h \
    processitemslist.h \
    utility.h \
    scheduler.h \
    SimpleZipper/src/SimpleZipper.h \
    SimpleZipper/miniz/miniz.h \
    zipfiles.h

INCLUDEPATH += src \
               miniz

FORMS    += mainwindow.ui \
    dialog.ui \
    addexedialog.ui

#RC_FILE = KillManagerQt.manifest

RC_FILE = app.rc

RESOURCES += \
    icons.qrc

win32:RC_ICONS = KillManager.ico

VERSION = 1.0.0.9
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
win32:LIBS += -lpsapi

DISTFILES += \
    KillManagerQt.manifest \
    app.rc
