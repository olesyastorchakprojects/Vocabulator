#-------------------------------------------------
#
# Project created by QtCreator 2017-02-11T14:17:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network webenginewidgets xml

TARGET = Vocabulator
TEMPLATE = app


SOURCES += main.cpp\
        UI/mainwindow.cpp \
    UI/treeitem.cpp \
    UI/treemodel.cpp \
    UI/Preferences/preferences.cpp \
    UI/Preferences/treemodelpreferences.cpp \
    UI/Preferences/vocabularylist.cpp \
    UI/Models/jsonmodel.cpp \
    UI/Models/xmlmodel.cpp \
    UI/Models/domitem.cpp \
    UI/Models/dommodel.cpp

HEADERS  += UI/mainwindow.h \
    UI/treeitem.h \
    UI/treemodel.h \
    UI/Preferences/preferences.h \
    UI/Preferences/treemodelpreferences.h \
    UI/Preferences/vocabularylist.h \
    UI/Models/jsonmodel.h \
    UI/Models/xmlmodel.h \
    UI/Models/domitem.h \
    UI/Models/dommodel.h

RESOURCES += \
    rc.qrc
