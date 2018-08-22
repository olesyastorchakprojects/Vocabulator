#ifndef EXAMPLESTABLE_H
#define EXAMPLESTABLE_H

#include "../Objects/example.h"

class ExamplesTable
{
public:
    static QList<Example> examples(const int definitionId);
    static int insertExample(const QString& example, const int definitionId);
};

#endif // EXAMPLESTABLE_H
