#include "phrasestable.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>

#include "../Objects/example.h"
#include "../Objects/phrase.h"

#include "examplestable.h"
#include "database.h"

QList<Phrase> PhrasesTable::phrases( const int projectId )
{
    QList<Phrase> ret;

    QSqlQuery q;
    q.exec(QString("SELECT id, phrase, created from phrases where projectId = %1").arg(QString::number(projectId)));
    while(q.next())
    {
        QList<Example> exs = ExamplesTable::examples(q.value(0).toInt(), Example::EXAMPLE_PHRASE);
        Phrase def(q.value(0).toInt(), q.value(1).toString(), q.value(2).toString(), exs);
        ret.push_back(def);
    }

    return ret;
}


int PhrasesTable::insertPhrase(const QString& phrase, const int projectId)
{
    QSqlQuery q;
    if(q.exec(QString("insert into phrases(phrase, projectId, created) values(\"%1\", %2, \"%3\")").arg(phrase, QString::number(projectId), Database::currentTime())))
    {
        return q.lastInsertId().toInt();
    }

    return -1;
}
