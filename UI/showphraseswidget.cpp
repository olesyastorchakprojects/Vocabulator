#include "showphraseswidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDesktopWidget>

#include "Database/projectstable.h"

ShowPhrasesController::ShowPhrasesController(ShowPhrasesWidget* widget): _widget(widget), _index(0)
{
    setupConnections();
    currentTextChangedInComboBox(_widget->comboBox()->currentText());
}

void ShowPhrasesController::setupConnections()
{
    connect(_widget->prevButton(), SIGNAL(clicked(bool)),this, SLOT(clickedPrev(bool)));
    connect(_widget->nextButton(), SIGNAL(clicked(bool)), this, SLOT(clickedNext(bool)));
    connect(_widget->showExampleButton(), SIGNAL(clicked(bool)), this, SLOT(clickedShowExample(bool)));

    connect(_widget->comboBox(), SIGNAL(currentIndexChanged(QString)), this, SLOT(currentTextChangedInComboBox(QString)));
}

void ShowPhrasesController::clickedPrev(bool)
{
    if(--_index < 0)
        _index = _data.size()-1;

    updateLabels();
}

void ShowPhrasesController::clickedNext(bool)
{
    if(++_index == _data.size())
        _index = 0;

    updateLabels();
}

void ShowPhrasesController::clickedShowExample(bool)
{
    if(_data.isEmpty())
        return;

    QList<Example> defs = _data[_index].examples();
    QString exStr;
    foreach (const Example& ex, defs)
    {
        exStr += ex.value();
        exStr += "\n";
    }
    _widget->exampleLabel()->setText(QString("Example: %1").arg(exStr));
}

void ShowPhrasesController::updateLabels()
{
    if(_data.isEmpty())
        return;

    _widget->phraseLabel()->setText(QString("%1 (%2 of %3)").arg(_data[_index].value(), QString::number(_index+1), QString::number(_data.size())));

    _widget->exampleLabel()->setText("Example");
}

void ShowPhrasesController::currentTextChangedInComboBox(const QString& text)
{
    _data.clear();
    _index = 0;
    fillData();
    updateLabels();
}

void ShowPhrasesController::fillData()
{
    QString projectName =_widget->comboBox()->currentText();
    Project* proj = ProjectsTable::project(projectName);
    if(proj)
    {
        _data = proj->phrases();
    }
}

/************************************************************************/
ShowPhrasesWidget::ShowPhrasesWidget(QWidget *parent) : QWidget(parent)
{
    setupLayout();
}

void ShowPhrasesWidget::setupLayout()
{
    QRect screen = QApplication::desktop()->screenGeometry();
    setMinimumSize(screen.width() * 0.4, screen.height() * 0.4 );

    QVBoxLayout* layout = new QVBoxLayout;

    _comboBox = new QComboBox;
    _comboBox->setEditable(true);

    QStringList projects = ProjectsTable::projectNamesDesc();
    foreach (const QString& p, projects)
    {
        _comboBox->addItem(p);
    }

    layout->addWidget(_comboBox);

    _phrase = new QLabel;
    _phrase->setWordWrap(true);
    _phrase->setStyleSheet(QString("font:14px"));
    layout->addWidget(_phrase);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    _prevButton = new QPushButton("<<");
    _nextButton = new QPushButton(">>");
    _showExampleButton = new QPushButton("show example");

    buttonsLayout->addWidget(_prevButton);
    buttonsLayout->addWidget(_nextButton);
    buttonsLayout->addWidget(_showExampleButton);

    layout->addLayout(buttonsLayout);

    _example = new QLabel;
    _example->setWordWrap(true);
    _example->setStyleSheet(QString("font:14px"));
    layout->addWidget(_example);

    setLayout(layout);
}

QComboBox* ShowPhrasesWidget::comboBox()
{
    return _comboBox;
}

QLabel* ShowPhrasesWidget::phraseLabel()
{
    return _phrase;
}

QLabel* ShowPhrasesWidget::exampleLabel()
{
    return _example;
}

QPushButton* ShowPhrasesWidget::prevButton()
{
    return _prevButton;
}

QPushButton* ShowPhrasesWidget::nextButton()
{
    return _nextButton;
}

QPushButton* ShowPhrasesWidget::showExampleButton()
{
    return _showExampleButton;
}
