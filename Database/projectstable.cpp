#include "projectstable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "wordstable.h"
#include "database.h"

Project* ProjectsTable::project(const QString& projectName)
{
    QSqlQuery q;
    if(q.exec(QString("SELECT id, created, url, edited from projects where project = \"%1\"").arg(projectName)))
    {
        q.next();
        QList<Word> ws = WordsTable::words(q.value(0).toInt());
        Project* p = new Project(q.value(0).toInt(), projectName, q.value(1).toString(), q.value(3).toString(), ws, q.value(2).toString());
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

QList<QPair<int, QString>> ProjectsTable::projectNamesIdsDesc()
{
     QList<QPair<int, QString>> ret;
     QSqlQuery q;
     q.exec(QString("SELECT id, project from projects order by id desc"));
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

QStringList ProjectsTable::projectNamesDesc()
{
    QStringList ret;
    QSqlQuery q;
    q.exec(QString("SELECT project from projects order by id desc"));
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
    q.exec(QString("SELECT id, project, created, url, edited from projects"));
    while(q.next())
    {
        QList<Word> ws = WordsTable::words(q.value(0).toInt());
        Project project(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), q.value(4).toString(), ws, q.value(3).toString());
        ret.push_back(project);
    }

    return ret;
}

QList<Project> ProjectsTable::projectsDesc()
{
    QList<Project> ret;

    QSqlQuery q;
    q.exec(QString("SELECT id, project, created, url, edited from projects order by id desc"));
    while(q.next())
    {
        QList<Word> ws = WordsTable::words(q.value(0).toInt());
        Project project(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), q.value(4).toString(), ws, q.value(3).toString());
        ret.push_back(project);
    }

    return ret;
}

int ProjectsTable::insertProject(const QString& project, const QString &url)
{
    QString query = QString("insert into projects(project, url, created, edited) values(\"%1\", \"%2\", \"%3\", \"%4\")").arg(project, url, Database::currentTime(), Database::currentTime());
    QSqlQuery q;
    if(q.exec(query))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}

bool ProjectsTable::updateEdited(int id, const QString& edited)
{
    QSqlQuery q;
    if(q.exec(QString("update projects set edited=\"%1\" where id = %2").arg(edited, QString::number(id))))
    {
        return true;
    }

    return false;
}
