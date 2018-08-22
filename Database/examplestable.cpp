#include "examplestable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "database.h"

QList<Example> ExamplesTable::examples(const int definitionId)
{
    QList<Example> ret;

    QSqlQuery q;
    q.exec(QString("SELECT id, example, created from examples where definitionId = %1").arg(QString::number(definitionId)));
    while(q.next())
    {
        Example example(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString());
        ret.push_back(example);
    }

    return ret;
}

int ExamplesTable::insertExample(const QString& example, const int definitionId)
{
    QSqlQuery q;
    if(q.exec(QString("insert into examples(example, definitionId, created) values(\"%1\", %2)").arg(example, QString::number(definitionId), Database::currentTime())))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}
