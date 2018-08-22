#include "projectstable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "wordstable.h"
#include "database.h"

Project* ProjectsTable::project(const QString& projectName)
{
    QSqlQuery q;
    if(q.exec(QString("SELECT id, created from projects where project = \"%1\"").arg(projectName)))
    {
        q.next();
        QList<Word> ws = WordsTable::words(q.value(0).toInt());
        Project* p = new Project(q.value(0).toInt(), projectName, q.value(1).toString(), ws);
        return p;
    }
    return NULL;
}

QList<QPair<int, QString>> ProjectsTable::projectNamesIds()
{
     QList<QPair<int, QString>> ret;
     QSqlQuery q;
     q.exec(QString("SELECT id, project from projects"));
     while(q.next())
     {
         ret.push_back(QPair<int, QString>(q.value(0).toInt(), q.value(1).toString()));
     }

     return ret;

}

QStringList ProjectsTable::projectNames()
{
    QStringList ret;
    QSqlQuery q;
    q.exec(QString("SELECT project from projects"));
    while(q.next())
    {
        ret.push_back(q.value(0).toString());
    }

    return ret;
}

QList<Project> ProjectsTable::projects()
{
    QList<Project> ret;

    QSqlQuery q;
    q.exec(QString("SELECT id, project, createdAt from projects"));
    while(q.next())
    {
        QList<Word> ws = WordsTable::words(q.value(0).toInt());
        Project project(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), ws);
        ret.push_back(project);
    }

    return ret;
}

int ProjectsTable::insertProject(const QString& project)
{
    QString query = QString("insert into projects(project, created) values(\"%1\", \"%2\")").arg(project, Database::currentTime());
    QSqlQuery q;
    if(q.exec(query))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}

