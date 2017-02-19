#include "preferences.h"

#include <QApplication>
#include <QDesktopWidget>

#include <QToolBar>
#include <QVBoxLayout>

Preferences::Preferences()
{
    QRect screen = QApplication::desktop()->screenGeometry();

    setMinimumSize(screen.width() * 0.4, screen.height() * 0.4 );

    QVBoxLayout* layout = new QVBoxLayout;
    QToolBar* toolBar = new QToolBar;
    QIcon icon(":/icons/vocabularies.png");
    bool valid = icon.isNull();
    toolBar->addAction(icon, "Vocabularies");
    toolBar->setStyleSheet("QToolButton { width: 300px;} ");
    toolBar->setStyleSheet("background-color:#b6cded");
    toolBar->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );
    toolBar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    layout->addWidget(toolBar);

    setLayout(layout);
}
