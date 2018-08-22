#ifndef SYNONYMSTABLE_H
#define SYNONYMSTABLE_H

#include "../Objects/synonym.h"

class SynonymsTable
{
public:
    static QList<Synonym> synonyms(const int definitionId);
    static int insertSynonym(const QString& synonym, const int definitionId);
};

#endif // SYNONYMSTABLE_H
