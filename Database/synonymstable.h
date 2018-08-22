#ifndef SYNONYMSTABLE_H
#define SYNONYMSTABLE_H

#include "../Objects/synonym.h"

class SynonymsTable
{
public:
    static QList<Synonym> selectAll();

    static QList<Synonym> synonyms(const int definitionId);
    static int insertSynonym(const QString& synonym, const int definitionId);

    static Synonym getSynonym(int synonymId);
    static Synonym getSynonym(const QString& synonymName);

    static bool removeSynonym(int synonymId);
    static bool removeSynonym(const QString& synonymName);

    static bool removeAll();
};

#endif // SYNONYMSTABLE_H
