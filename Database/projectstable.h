#ifndef PROJECTSTABLE_H
#define PROJECTSTABLE_H

#include "../Objects/project.h"

class ProjectsTable
{
public:
    static QList<Project> projects();
    static QList<Project> projectsDesc();
    static Project* project(const QString& projectName);
    static QStringList projectNames();
    static QStringList projectNamesDesc();
    static QList<QPair<int, QString>> projectNamesIds();
    static QList<QPair<int, QString>> projectNamesIdsDesc();

    static bool updateEdited(int id, const QString& edited);

    static int insertProject(const QString& project, const QString& url);
};

#endif // PROJECTSTABLE_H
