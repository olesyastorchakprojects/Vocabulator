#include "examplestable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "database.h"

QList<Example> ExamplesTable::selectAll()
{
    QList<Example> list;

    QSqlQuery q;
    if(q.exec(QString("SELECT id, example, created from examples")))
    {
        while(q.next())
        {
            Example example(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString());
            list.push_back(example);
        }
    }

    return list;
}

QList<Example> ExamplesTable::examples(const int definitionId)
{
    QList<Example> list;

    QSqlQuery q;
    if(q.exec(QString("SELECT id, example, created from examples where definitionId=%1").arg(definitionId)))
    {
        while(q.next())
        {
            Example example(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString());
            list.push_back(example);
        }
    }

    return list;
}


int ExamplesTable::insertExample(const QString& example, const int definitionId)
{
    QSqlQuery q;
    if(q.exec(QString("insert into examples (example, definitionId, created) values (\"%1\", %2, \"%3\")").arg(
                  example, QString::number(definitionId), Database::currentTime())))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}


Example ExamplesTable::getExample(int exampleId)
{
    QSqlQuery q;
    if(q.exec(QString("select example, created from examples where id=%1").arg(QString::number(exampleId))))
    {
        if(q.next())
        {
            return Example(exampleId, q.value(0).toString(), q.value(1).toString());
        }
    }

    return Example();
}

Example ExamplesTable::getExample(const QString& exampleName)
{
    QSqlQuery q;
    if(q.exec(QString("select id, created from examples where example=\"%1\"").arg(exampleName)))
    {
        if(q.next())
        {
            return Example(q.value(0).toInt(), exampleName, q.value(1).toString());
        }
    }

    return Example();
}


bool ExamplesTable::removeExample(int exampleId)
{
    QSqlQuery q;
    if(q.exec(QString("delete from examples where id = \"%1\"").arg(QString::number(exampleId))))
    {
        return true;
    }

    return false;
}

bool ExamplesTable::removeExample(const QString& exampleName)
{
    QSqlQuery q;
    if(q.exec(QString("delete from examples where example = \"%1\"").arg(exampleName)))
    {
        return true;
    }

    return false;
}


bool ExamplesTable::removeAll()
{
    QSqlQuery q;
    if(q.exec(QString("delete from examples")))
    {
        return true;
    }

    return false;
}


bool ExamplesTable::updateExample(const Example& example)
{
    QSqlQuery q;
    if(q.exec(QString("update examples set example=\"%1\", created = \"%2\" where id = %3").arg(example.value(), example.createdAt(), QString::number(example.id()))))
    {
        return true;
    }

    return false;
}
