#include "synonymstable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "database.h"

QList<Synonym> SynonymsTable::synonyms(const int definitionId)
{
    QList<Synonym> ret;

    QSqlQuery q;
    q.exec(QString("SELECT id, synonym, created from synonyms where definitionId = %1").arg(QString::number(definitionId)));
    while(q.next())
    {
        Synonym synonym(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString());
        ret.push_back(synonym);
    }

    return ret;
}

int SynonymsTable::insertSynonym(const QString& synonym, const int definitionId)
{
    QSqlQuery q;
    if(q.exec(QString("insert into synonyms(synonym, definitionId, created) values(\"%1\", %2)").arg(synonym, QString::number(definitionId), Database::currentTime())))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}
