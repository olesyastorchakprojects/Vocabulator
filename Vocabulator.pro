#-------------------------------------------------
#
# Project created by QtCreator 2017-02-11T14:17:59
#
#-------------------------------------------------

QT       += core gui sql

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
    UI/Models/dommodel.cpp \
    Objects/oxfordentry.cpp \
    Database/database.cpp \
    Objects/word.cpp \
    Objects/synonym.cpp \
    Objects/example.cpp \
    Objects/definition.cpp \
    Objects/project.cpp \
    Database/projectstable.cpp \
    Database/wordstable.cpp \
    Database/definitionstable.cpp \
    Database/synonymstable.cpp \
    Database/examplestable.cpp \
    Tests/Database/testdatabase.cpp \
    Tests/Database/testsynonymstable.cpp \
    Config/config.cpp \
    Tests/Database/testexamplestable.cpp \
    Tests/Database/testprojectstable.cpp \
    UI/definitionwidget.cpp \
    UI/showdefinitionswidget.cpp \
    Export/export.cpp \
    UI/AddDefinition/selectprojectwidget.cpp \
    Database/phrasestable.cpp \
    Objects/phrase.cpp \
    UI/showphraseswidget.cpp

HEADERS  += UI/mainwindow.h \
    UI/treeitem.h \
    UI/treemodel.h \
    UI/Preferences/preferences.h \
    UI/Preferences/treemodelpreferences.h \
    UI/Preferences/vocabularylist.h \
    UI/Models/jsonmodel.h \
    UI/Models/xmlmodel.h \
    UI/Models/domitem.h \
    UI/Models/dommodel.h \
    Objects/oxfordentry.h \
    Database/database.h \
    Objects/word.h \
    Objects/synonym.h \
    Objects/example.h \
    Objects/definition.h \
    Objects/project.h \
    Database/projectstable.h \
    Database/wordstable.h \
    Database/definitionstable.h \
    Database/synonymstable.h \
    Database/examplestable.h \
    Tests/Database/testdatabase.h \
    Tests/Database/testsynonymstable.h \
    Config/config.h \
    Tests/Database/testexamplestable.h \
    Tests/Database/testprojectstable.h \
    UI/definitionwidget.h \
    UI/showdefinitionswidget.h \
    Export/export.h \
    UI/AddDefinition/selectprojectwidget.h \
    Database/phrasestable.h \
    Objects/phrase.h \
    UI/showphraseswidget.h

RESOURCES += \
    rc.qrc
