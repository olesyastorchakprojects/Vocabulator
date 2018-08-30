#ifndef PHRASESTABLE_H
#define PHRASESTABLE_H

#include <QString>

#include "Objects/phrase.h"

class PhrasesTable
{
public:
    static QList<Phrase> phrases( const int projectId );
    static int insertPhrase(const QString& phrase, const int projectId);
};

#endif // PHRASESTABLE_H
