#include "database.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

QSqlError Database::initDb(const QString &dbName)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName/*"vocabulary.db"*/);

    if (!db.open())
        return db.lastError();

    QStringList tables = db.tables();
    if (tables.contains("projects", Qt::CaseInsensitive) &&
        tables.contains("words", Qt::CaseInsensitive) &&
        tables.contains("definitions", Qt::CaseInsensitive) &&
        tables.contains("synonyms", Qt::CaseInsensitive) &&
        tables.contains("examples", Qt::CaseInsensitive))
        return QSqlError();

    QSqlQuery q;
    if (!q.exec(QLatin1String("create table projects(id integer primary key, project varchar, created varchar)")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table words(id integer primary key, projectId integer, word varchar, created varchar)")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table definitions(id integer primary key, wordId integer, definition varchar, created varchar)")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table examples(id integer primary key, definitionId integer, example varchar, created varchar)")))
        return q.lastError();
    if (!q.exec(QLatin1String("create table synonyms(id integer primary key, definitionId integer, synonym varchar, created varchar)")))
        return q.lastError();

    return QSqlError();
}


QString Database::currentTime()
{
    return QDateTime::currentDateTimeUtc().toString();
}
