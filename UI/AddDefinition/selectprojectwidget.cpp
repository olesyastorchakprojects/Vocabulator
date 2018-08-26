#include "selectprojectwidget.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QDateTime>

#include "Config/config.h"

#include "Database/projectstable.h"

SelectProjectController::SelectProjectController(SelectProjectWidget* widget)
    : _widget(widget)
{
    setupConnections();
    QStringList projects = projectData();
    _widget->fillData(projects);
}

QStringList SelectProjectController::projectData()
{
    QList<Project> projects = ProjectsTable::projects();

    QMultiMap<QDateTime, Project> uiData;
    foreach (const Project& project, projects)
    {
        _data.insert(project.value(), project);
        uiData.insert(QDateTime::fromString(project.editedAt()), project);
    }

    QStringList list;
    QMapIterator<QDateTime, Project> it(uiData);
    while(it.hasNext())
    {
        it.next();
        list.push_front(it.value().value());
    }

    return list;
}

void SelectProjectController::setupConnections()
{
    connect(_widget->projectsComboBox(), SIGNAL(currentIndexChanged(QString)), this, SLOT(currentIndexChanged(QString)));
    connect(_widget->addToProjectButton(), SIGNAL(clicked(bool)), _widget, SLOT(close()));
    connect(_widget, SIGNAL(addNewProject(QString,QString)), this, SLOT(addNewProject(QString,QString)));
}

void SelectProjectController::currentIndexChanged(const QString& text)
{
    if(_data.contains(text))
    {
        _selectedProjectId = _data[text].id();
    }
}

int SelectProjectController::selectedProjectId()const
{
    return _selectedProjectId;
}

void SelectProjectController::addNewProject(const QString& name, const QString& url)
{
    Project* proj = ProjectsTable::project(name);
    if(proj)
        return;

    int projectId = ProjectsTable::insertProject(name, url);
    if(projectId != -1)
    {
        Project* project = ProjectsTable::project(name);
        if(project)
            _data.insert(project->value(), *project);
    }
}

/***********************************************/
SelectProjectWidget::SelectProjectWidget(const QString &defaultNewName, const QString &url)
    : _defaultNewName(defaultNewName), _defaultProjectUrl(url)
{
    setupLayout();
}

void SelectProjectWidget::setupLayout()
{
    QRect screenRect = Config::screenGeometry();
    setMinimumSize(screenRect.width()*0.2, screenRect.height()*0.2);

    QVBoxLayout* layout = new QVBoxLayout;
    _addNewProjectButton = new QPushButton("New project");
    _addNewProjectButton->setMaximumWidth(100);
    _projectsComboBox = new QComboBox;
    _addToProjectButton = new QPushButton("Add to project");
    _addToProjectButton->setMaximumWidth(100);

    QLabel* label = new QLabel();
    label->setText(QString("New word with definitions and examples will be added to selected project"));
    label->setWordWrap(true);

    layout->addWidget(_addNewProjectButton);
    //layout->addStretch();
    layout->addWidget(_projectsComboBox);
    layout->addStretch();
    layout->addWidget(label);
    layout->addWidget(_addToProjectButton);

    setLayout(layout);

    connect(_addNewProjectButton, SIGNAL(clicked(bool)), this, SLOT(addNewProjectButtonClicked(bool)));
}

void SelectProjectWidget::fillData(const QStringList& data)
{
    foreach(const QString& str, data)
    {
        _projectsComboBox->addItem(str);
    }
}

QPushButton* SelectProjectWidget::addNewProjectButton()
{
    return _addNewProjectButton;
}

QPushButton* SelectProjectWidget::addToProjectButton()
{
    return _addToProjectButton;
}

QComboBox* SelectProjectWidget::projectsComboBox()
{
    return _projectsComboBox;
}

void SelectProjectWidget::addNewProjectButtonClicked(bool)
{
    QDialog* dialog = new QDialog;

    QRect screenRect = Config::screenGeometry();
    dialog->setMinimumSize(screenRect.width()*0.3, screenRect.height()*0.1);

    QVBoxLayout* layout = new QVBoxLayout;

    QHBoxLayout* nameHLayout = new QHBoxLayout;

    //{
        QLabel* labelName = new QLabel("name:");
        labelName->setMinimumWidth(50);
        QLineEdit* name = new QLineEdit();
        name->setText(_defaultNewName);

        nameHLayout->addWidget(labelName);
        nameHLayout->addWidget(name);
    //}

    QHBoxLayout* urlHLayout = new QHBoxLayout;
    //{
        QLabel* labelUrl = new QLabel("url:");
        labelUrl->setMinimumWidth(50);
        QLineEdit* url = new QLineEdit();
        url->setText(_defaultProjectUrl);

        urlHLayout->addWidget(labelUrl);
        urlHLayout->addWidget(url);
    //}

    QHBoxLayout* buttonHLayout = new QHBoxLayout;
    {
        QPushButton* addButton = new QPushButton("Add");
        connect(addButton, SIGNAL(clicked(bool)), dialog, SLOT(accept()));

        buttonHLayout->addSpacerItem(new QSpacerItem(width()*0.8, 0, QSizePolicy::Minimum, QSizePolicy::Expanding));
        buttonHLayout->addWidget(addButton);
    }

    layout->addLayout(nameHLayout);
    layout->addLayout(urlHLayout);
    layout->addLayout(buttonHLayout);

    dialog->setLayout(layout);

    if( (dialog->exec() != QDialog::Rejected) && !name->text().isEmpty())
    {
        emit addNewProject(name->text(), url->text());
        _projectsComboBox->insertItem(0, name->text());
        _projectsComboBox->setCurrentIndex(0);
    }
}

