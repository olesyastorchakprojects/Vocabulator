#ifndef PROJECTSTABLE_H
#define PROJECTSTABLE_H

#include "../Objects/project.h"

class ProjectsTable
{
public:
    static QList<Project> projects();
    static Project* project(const QString& projectName);
    static QStringList projectNames();
    static QList<QPair<int, QString>> projectNamesIds();

    static int insertProject(const QString& project);
};

#endif // PROJECTSTABLE_H
