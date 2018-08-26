#include "showdefinitionswidget.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QDesktopWidget>

#include "Database/projectstable.h"

ShowDefinitionsController::ShowDefinitionsController(ShowDefinitionsWidget* widget): _widget(widget), _index(0)
{
    setupConnections();
    currentTextChangedInComboBox(_widget->comboBox()->currentText());
}

void ShowDefinitionsController::setupConnections()
{
    connect(_widget->prevButton(), SIGNAL(clicked(bool)),this, SLOT(clickedPrev(bool)));
    connect(_widget->nextButton(), SIGNAL(clicked(bool)), this, SLOT(clickedNext(bool)));
    connect(_widget->showDefinitionButton(), SIGNAL(clicked(bool)), this, SLOT(clickedShowDefinition(bool)));

    connect(_widget->comboBox(), SIGNAL(currentIndexChanged(QString)), this, SLOT(currentTextChangedInComboBox(QString)));
}

void ShowDefinitionsController::clickedPrev(bool)
{
    if(--_index < 0)
        _index = _data.size()-1;

    updateLabels();
}

void ShowDefinitionsController::clickedNext(bool)
{
    if(++_index == _data.size())
        _index = 0;

    updateLabels();
}

void ShowDefinitionsController::clickedShowDefinition(bool)
{
    if(_data.isEmpty())
        return;

    QList<Definition> defs = _data[_index].definitions();
    QString defStr;
    foreach (const Definition& def, defs)
    {
        defStr += def.value();
        QList<Example> exs = def.examples();
        defStr += "\n";
        foreach (const Example& ex, exs)
        {
            defStr += "Example: ";
            defStr += ex.value();
            defStr += "\n";
        }
        defStr += "\n\n";
    }
    _widget->definitionLabel()->setText(QString("Definition: %1").arg(defStr));
}

void ShowDefinitionsController::updateLabels()
{
    if(_data.isEmpty())
        return;

    _widget->wordLabel()->setText(QString("%1 (%2 of %3)").arg(_data[_index].value(), QString::number(_index+1), QString::number(_data.size())));

    _widget->definitionLabel()->setText("Definition");
}

void ShowDefinitionsController::currentTextChangedInComboBox(const QString& text)
{
    _data.clear();
    _index = 0;
    fillData();
    updateLabels();
}

void ShowDefinitionsController::fillData()
{
    QString projectName =_widget->comboBox()->currentText();
    Project* proj = ProjectsTable::project(projectName);
    if(proj)
    {
        _data = proj->words();
    }
}

ShowDefinitionsWidget::ShowDefinitionsWidget(QWidget *parent) : QWidget(parent)
{
    setupLayout();
}

void ShowDefinitionsWidget::setupLayout()
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

    _word = new QLabel;
    _word->setWordWrap(true);
    _word->setStyleSheet(QString("font:14px"));
    layout->addWidget(_word);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    _prevButton = new QPushButton("<<");
    _nextButton = new QPushButton(">>");
    _showDefinitionButton = new QPushButton("show definition");

    buttonsLayout->addWidget(_prevButton);
    buttonsLayout->addWidget(_nextButton);
    buttonsLayout->addWidget(_showDefinitionButton);

    layout->addLayout(buttonsLayout);

    _definition = new QLabel;
    _definition->setWordWrap(true);
    _definition->setStyleSheet(QString("font:14px"));
    layout->addWidget(_definition);

    setLayout(layout);


    /*ShowDefinitionsController* controller = new ShowDefinitionsController(word, definition, comboBox);

    connect(prevButton, SIGNAL(clicked(bool)),controller, SLOT(clickedPrev(bool)));
    connect(nextButton, SIGNAL(clicked(bool)),controller, SLOT(clickedNext(bool)));
    connect(showButton, SIGNAL(clicked(bool)),controller, SLOT(clickedShowDefinition(bool)));

    connect(comboBox, SIGNAL(currentIndexChanged(QString)), controller, SLOT(currentTextChangedInComboBox(QString)));

    widget->show();*/
}

QComboBox* ShowDefinitionsWidget::comboBox()
{
    return _comboBox;
}

QLabel* ShowDefinitionsWidget::wordLabel()
{
    return _word;
}

QLabel* ShowDefinitionsWidget::definitionLabel()
{
    return _definition;
}

QPushButton* ShowDefinitionsWidget::prevButton()
{
    return _prevButton;
}

QPushButton* ShowDefinitionsWidget::nextButton()
{
    return _nextButton;
}

QPushButton* ShowDefinitionsWidget::showDefinitionButton()
{
    return _showDefinitionButton;
}
