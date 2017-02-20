#include "preferences.h"

#include <QApplication>
#include <QDesktopWidget>

#include <QToolBar>
#include <QHBoxLayout>

#include <QStackedWidget>

#include "vocabularylist.h"

Preferences::Preferences()
{
    QRect screen = QApplication::desktop()->screenGeometry();

    setMinimumSize(screen.width() * 0.4, screen.height() * 0.4 );

    _stackedWidget = new QStackedWidget;
    setupStackedWidget();

    QHBoxLayout* layout = new QHBoxLayout;
    QToolBar* toolBar = new QToolBar;
    QIcon icon(":/icons/vocabularies.png");
    bool valid = icon.isNull();
    toolBar->addAction(icon, "Vocabularies");
    toolBar->setStyleSheet("QToolButton { width: 300px;} ");
    toolBar->setStyleSheet("background-color:#b6cded;QToolButton { width: 500px;}");
    toolBar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    layout->addWidget(toolBar);
    layout->addWidget(_stackedWidget);

    setLayout(layout);
}

void Preferences::setupStackedWidget()
{
    VocabularyList* vlist = new VocabularyList;
    _stackedWidget->addWidget(vlist);
}


void Preferences::vocabularies()
{

}
