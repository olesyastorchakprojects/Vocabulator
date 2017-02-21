#-------------------------------------------------
#
# Project created by QtCreator 2017-02-11T14:17:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network webenginewidgets

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
    UI/Models/xmlmodel.cpp

HEADERS  += UI/mainwindow.h \
    UI/treeitem.h \
    UI/treemodel.h \
    UI/Preferences/preferences.h \
    UI/Preferences/treemodelpreferences.h \
    UI/Preferences/vocabularylist.h \
    UI/Models/jsonmodel.h \
    UI/Models/xmlmodel.h

RESOURCES += \
    rc.qrc
