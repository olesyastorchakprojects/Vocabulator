#include "examplestable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "database.h"

QList<Example> ExamplesTable::selectAll()
{
    QList<Example> list;

    QSqlQuery q;
    if(q.exec(QString("SELECT id, example, created, type from examples")))
    {
        while(q.next())
        {
            Example example(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), static_cast<Example::ExampleType>(q.value(3).toInt()));
            list.push_back(example);
        }
    }

    return list;
}

QList<Example> ExamplesTable::examples(const int anchorId, Example::ExampleType exampleType)
{
    QList<Example> list;

    QSqlQuery q;
    if(q.exec(QString("SELECT id, example, created from examples where definitionId=%1 and type=%2").arg(QString::number(anchorId), QString::number(exampleType))))
    {
        while(q.next())
        {
            Example example(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), exampleType);
            list.push_back(example);
        }
    }

    return list;
}

int ExamplesTable::insertExample(const QString& example, const int definitionId, const Example::ExampleType exampleType)
{
    QSqlQuery q;
    if(q.exec(QString("insert into examples (example, definitionId, created, type) values (\"%1\", %2, \"%3\", %4)").arg(
                  example, QString::number(definitionId), Database::currentTime(), QString::number(exampleType))))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}


Example ExamplesTable::getExample(int exampleId)
{
    QSqlQuery q;
    if(q.exec(QString("select example, created, type from examples where id=%1").arg(QString::number(exampleId))))
    {
        if(q.next())
        {
            return Example(exampleId, q.value(0).toString(), q.value(1).toString(), static_cast<Example::ExampleType>(q.value(2).toInt()));
        }
    }

    return Example();
}

Example ExamplesTable::getExample(const QString& exampleName)
{
    QSqlQuery q;
    if(q.exec(QString("select id, created, type from examples where example=\"%1\"").arg(exampleName)))
    {
        if(q.next())
        {
            return Example(q.value(0).toInt(), exampleName, q.value(1).toString(), static_cast<Example::ExampleType>(q.value(2).toInt()));
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
    if(q.exec(QString("update examples set example=\"%1\", created = \"%2\", type = %4 where id = %3")
              .arg(example.value(), example.createdAt(), QString::number(example.id()), QString::number(example.exampleType()))))
    {
        return true;
    }

    return false;
}
