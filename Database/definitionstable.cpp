#include "definitionstable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "../Objects/synonym.h"
#include "../Objects/example.h"

#include "synonymstable.h"
#include "examplestable.h"
#include "database.h"

QList<Definition> DefinitionsTable::definitions( const int wordId )
{
    QList<Definition> ret;

    QSqlQuery q;
    q.exec(QString("SELECT id, definition, created from definitions where wordId = %1").arg(QString::number(wordId)));
    while(q.next())
    {
        QList<Synonym> syns = SynonymsTable::synonyms(q.value(0).toInt());
        QList<Example> exs = ExamplesTable::examples(q.value(0).toInt(), Example::EXAMPLE_WORD);
        Definition def(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), syns, exs);
        ret.push_back(def);
    }

    return ret;
}


int DefinitionsTable::insertDefinition(const QString& definition, const int wordId)
{
    QSqlQuery q;
    if(q.exec(QString("insert into definitions(definition, wordId, created) values(\"%1\", %2, \"%3\")").arg(definition, QString::number(wordId), Database::currentTime())))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}
