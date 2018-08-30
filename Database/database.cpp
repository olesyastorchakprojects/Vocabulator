#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>

bool Database::initDb(const QString &dbName)
{
    if (!QSqlDatabase::drivers().contains("QSQLITE"))
    {
        QMessageBox::critical(NULL, "Unable to load database", "This app needs the SQLITE driver");
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName/*"vocabulary.db"*/);

    if (!db.open())
    {
        qDebug() << "Unable to open db, error - " << db.lastError();
        return false;
    }

    QStringList tables = db.tables();
    if (tables.contains("projects", Qt::CaseInsensitive) &&
        tables.contains("words", Qt::CaseInsensitive) &&
        tables.contains("definitions", Qt::CaseInsensitive) &&
        tables.contains("synonyms", Qt::CaseInsensitive) &&
        tables.contains("examples", Qt::CaseInsensitive) &&
        tables.contains("phrases", Qt::CaseInsensitive))
    {
        qDebug() << "Tables already exists";
        return true;
    }

    QSqlQuery q;
    if (!q.exec(QLatin1String("create table projects(id integer primary key, project varchar, created varchar, url varchar, edited varchar)")))
    {
        qDebug() << "Failed to create projects table";
        return false;
    }
    if (!q.exec(QLatin1String("create table words(id integer primary key, projectId integer, word varchar, created varchar)")))
    {
        qDebug() << "Failed to create words table";
        return false;
    }
    if (!q.exec(QLatin1String("create table definitions(id integer primary key, wordId integer, definition varchar, created varchar)")))
    {
        qDebug() << "Failed to create definitions table";
        return false;
    }
    if (!q.exec(QLatin1String("create table examples(id integer primary key, definitionId integer, example varchar, created varchar, type integer)")))
    {
        qDebug() << "Failed to create examples table";
        return false;
    }
    if (!q.exec(QLatin1String("create table synonyms(id integer primary key, definitionId integer, synonym varchar, created varchar)")))
    {
        qDebug() << "Failed to create synonyms table";
        return false;
    }
    if (!q.exec(QLatin1String("create table phrases(id integer primary key, projectId integer, phrase varchar, created varchar)")))
    {
        qDebug() << "Failed to create synonyms table";
        return false;
    }

    return true;
}

void Database::closeDb()
{
    QSqlDatabase db = QSqlDatabase::database();
    if(db.isOpen())
        db.close();
}

QString Database::currentTime()
{
    return QDateTime::currentDateTimeUtc().toString();
}
