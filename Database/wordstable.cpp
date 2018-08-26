#include "wordstable.h"

#include <QSqlQuery>
#include <QVariant>

#include "../Objects/definition.h"
#include "definitionstable.h"
#include "database.h"

int WordsTable::insertWord(const QString& word, const int projectId)
{
    QSqlQuery q;
    if(q.exec(QString("insert into words(word, projectId, created) values(\"%1\", %2, \"%3\")").arg(word, QString::number(projectId), Database::currentTime())))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}

QList<Word> WordsTable::words(const int projectId )
{
    QList<Word> ret;

    QSqlQuery q;
    q.exec(QString("SELECT id, word, created from words where projectId = %1").arg(QString::number(projectId)));
    while(q.next())
    {
        QList<Definition> defs = DefinitionsTable::definitions(q.value(0).toInt());
        Word word(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), defs);
        ret.push_back(word);
    }

    return ret;
}

