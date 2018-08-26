#ifndef SELECTPROJECTWIDGET_H
#define SELECTPROJECTWIDGET_H

#include <QDialog>
#include <QMap>

#include "Objects/project.h"

class SelectProjectWidget;
class QPushButton;
class QComboBox;

class SelectProjectController : public QObject
{
    Q_OBJECT

public:
    explicit SelectProjectController(SelectProjectWidget* widget);
    int selectedProjectId()const;

private slots:
    void currentIndexChanged(const QString& text);
    void addNewProject(const QString& name, const QString& url);

private:
    void setupConnections();
    QStringList projectData();

private:
    int _selectedProjectId;
    SelectProjectWidget* _widget;
    QMap<QString, Project> _data;
};

class SelectProjectWidget : public QDialog
{
    Q_OBJECT
public:
    SelectProjectWidget(const QString& defaultNewName, const QString& url);

    void fillData(const QStringList& data);

    QPushButton* addNewProjectButton();
    QPushButton* addToProjectButton();
    QComboBox* projectsComboBox();

signals:
    void addNewProject(const QString& name, const QString& url);

public slots:
    void addNewProjectButtonClicked(bool);

private:
    void setupLayout();

private:
    QPushButton* _addNewProjectButton;
    QPushButton* _addToProjectButton;
    QComboBox* _projectsComboBox;
    QString _defaultNewName;
    QString _defaultProjectUrl;
};

#endif // SELECTPROJECTWIDGET_H
