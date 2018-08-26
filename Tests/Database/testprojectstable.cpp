#include "testprojectstable.h"

#include "Objects/project.h"
#include "Database/projectstable.h"

#include <QDebug>

bool TestProjectsTable::runTests()
{
    QString name = "In ‘Small Fry,’ Steve Jobs Comes Across as a Jerk. His Daughter Forgives Him. Should We?";
    QString url = "https://www.nytimes.com/2018/08/23/books/steve-jobs-lisa-brennan-jobs-small-fry.html?rref=collection%2Fsectioncollection%2Ftechnology&action=click&contentCollection=technology&region=rank&module=package&version=highlights&contentPlacement=1&pgtype=sectionfront";

    if(!ProjectsTable::insertProject(name, url))
    {
        qDebug() << "TestProjectsTable::runTests(): failed(insert project)";
        return false;
    }

    Project* proj = ProjectsTable::project(name);
    if(!proj)
    {
        qDebug() << "TestProjectsTable::runTests(): failed(get project)";
        return false;
    }

    if(proj->url() != url)
    {
        qDebug() << "TestProjectsTable::runTests(): failed(get project)";
        return false;
    }

    QList<Project> list = ProjectsTable::projects();
    if(list.isEmpty())
    {
        qDebug() << "TestProjectsTable::runTests(): failed(get projects)";
        return false;
    }

    int id = ProjectsTable::insertProject("name", "url");
    ProjectsTable::updateEdited(id, "aaa");

    list = ProjectsTable::projects();
    bool ret = false;
    foreach (const Project& p, list)
    {
        if(p.editedAt() != "aaa")
        {
            ret = true;
            break;
        }
    }

    return ret;
}
