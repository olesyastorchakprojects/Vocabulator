#ifndef DEFINITIONSTABLE_H
#define DEFINITIONSTABLE_H

#include "../Objects/definition.h"

class DefinitionsTable
{
public:
    static QList<Definition> definitions( const int wordId );
    static int insertDefinition(const QString& definition, const int wordId);
};

#endif // DEFINITIONSTABLE_H
