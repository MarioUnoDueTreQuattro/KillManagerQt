#-------------------------------------------------
#
# Project created by QtCreator 2025-07-24T04:32:40
#
#-------------------------------------------------

CONFIG += c++11

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets winextras

TARGET = KillManager
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
#      3rdparty/qhotkey/QHotkey/qhotkey.cpp \
#    3rdparty/qhotkey/QHotkey/qhotkey_win.cpp \
    clickable_label.cpp \
      mainwindow.cpp \
    dialog.cpp \
    addexedialog.cpp \
    myqlistwidget.cpp \
    processitemslist.cpp \
    utility.cpp \
    scheduler.cpp \
    SimpleZipper/src/SimpleZipper.cxx \
    SimpleZipper/miniz/miniz.c \
    zipfiles.cpp

HEADERS  += mainwindow.h \
#       3rdparty/qhotkey/QHotkey/qhotkey.h \
#    3rdparty/qhotkey/QHotkey/qhotkey_p.h \
    clickable_label.h \
 dialog.h \
    addexedialog.h \
    myqlistwidget.h \
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

#RC_FILE = app.rc
RC_FILE = appicon.rc

RESOURCES += \
    icons.qrc

#CONFIG += embed_manifest

win32:RC_ICONS = KillManager.ico

win32:LIBS += -lpsapi

DISTFILES += \
    KillManagerQt.manifest \
    appicon.rc

VERSION = 1.0.1.1
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
