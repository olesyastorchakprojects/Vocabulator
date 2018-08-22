#include "synonymstable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "database.h"

QList<Synonym> SynonymsTable::selectAll()
{
    QList<Synonym> ret;

    QSqlQuery q;
    q.exec(QString("SELECT id, synonym, created from synonyms"));
    while(q.next())
    {
        Synonym synonym(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString());
        ret.push_back(synonym);
    }

    return ret;
}

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
    if(q.exec(QString("insert into synonyms(synonym, definitionId, created) values(\"%1\", %2, \"%3\")").arg(synonym, QString::number(definitionId), Database::currentTime())))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}

Synonym SynonymsTable::getSynonym(int synonymId)
{
    QSqlQuery q;
    if(q.exec(QString("SELECT synonym, created from synonyms where id=%1").arg(QString::number(synonymId))))
    {
        if(q.next())
        {
            QString value = q.value(0).toString();
            QString created = q.value(1).toString();

            return Synonym(synonymId, value, created);
        }
    }

    return Synonym();
}

Synonym SynonymsTable::getSynonym(const QString& synonymName)
{
    QSqlQuery q;
    q.prepare(QString("SELECT id, created from synonyms where synonym=\"%1\"").arg(synonymName));
    if(q.exec())
    {
        if(q.next())
        {
            int synonymId = q.value(0).toInt();
            QString created = q.value(1).toString();

            return Synonym(synonymId, synonymName, created);
        }
    }

    return Synonym();
}

bool SynonymsTable::removeSynonym(int synonymId)
{
    QSqlQuery q;
    if(q.exec(QString("delete from synonyms where id = \"%1\"").arg(QString::number(synonymId))))
    {
        return true;
    }

    return false;
}

bool SynonymsTable::removeSynonym(const QString& synonymName)
{
    QSqlQuery q;
    if(q.exec(QString("delete from synonyms where synonym = \"%1\"").arg(synonymName)))
    {
        return true;
    }

    return false;
}

bool SynonymsTable::removeAll()
{
    QSqlQuery q;
    if(q.exec(QString("delete from synonyms")))
    {
        return true;
    }

    return false;
}
