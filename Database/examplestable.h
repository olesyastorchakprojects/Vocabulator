#ifndef EXAMPLESTABLE_H
#define EXAMPLESTABLE_H

#include "../Objects/example.h"

class ExamplesTable
{
public:

    static QList<Example> selectAll();
    static QList<Example> examples(const int definitionId);

    static int insertExample(const QString& example, const int definitionId);

    static Example getExample(int exampleId);
    static Example getExample(const QString& exampleName);

    static bool removeExample(int exampleId);
    static bool removeExample(const QString& exampleName);

    static bool removeAll();

    static bool updateExample(const Example& example);
};

#endif // EXAMPLESTABLE_H
