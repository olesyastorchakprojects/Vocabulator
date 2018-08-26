#include "testexamplestable.h"

#include "Database/examplestable.h"
#include "Objects/example.h"

#include <QDebug>

bool TestExamplesTable::runTests()
{
    ExamplesTable::removeAll();

    QString exampleValue = "tree";
    int exampleId = -1;
    if(( exampleId = ExamplesTable::insertExample(exampleValue, 111)) == -1)
    {
        qDebug() << "TestExamplesTable::runTests(): failed(insert/get by name)";
        return false;
    }

    Example example1 = ExamplesTable::getExample(exampleValue);
    if(!example1.isValid())
    {
        qDebug() << "TestExamplesTable::runTests(): failed(insert/get by name)";
        return false;
    }

    Example example2 = ExamplesTable::getExample(example1.id());
    if(!example2.isValid())
    {
        qDebug() << "TestExamplesTable::runTests(): failed(insert/get by id)";
        return false;
    }

    QList<Example> exampleList = ExamplesTable::selectAll();
    if(exampleList.isEmpty() || !exampleList.at(0).isValid())
    {
        qDebug() << "TestExamplesTable::runTests(): failed(selectAll)";
        return false;
    }

    if(!ExamplesTable::removeExample(exampleValue))
    {
        qDebug() << "TestExamplesTable::runTests(): failed(removeExample)";
        return false;
    }

    exampleList = ExamplesTable::selectAll();
    if(!exampleList.isEmpty())
    {
        qDebug() << "TestExamplesTable::runTests(): failed(removeExample)";
        return false;
    }

    if(( exampleId = ExamplesTable::insertExample(exampleValue, 111)) == -1)
    {
        qDebug() << "TestExamplesTable::runTests(): failed(insert/get by name)";
        return false;
    }

    example1 = ExamplesTable::getExample(exampleValue);
    if(!example1.isValid())
    {
        qDebug() << "TestExamplesTable::runTests(): failed(insert/get by name)";
        return false;
    }

    int exampleId2 = ExamplesTable::insertExample("exampleValue", 111);

    QString newValue = "new value";
    example1.setValue(newValue);
    if(!ExamplesTable::updateExample(example1))
    {
        qDebug() << "TestExamplesTable::runTests(): failed(update)";
        return false;
    }

    example2 = ExamplesTable::getExample(exampleId2);
    if(example2.value() == newValue)
    {
        qDebug() << "TestExamplesTable::runTests(): failed(update, updated all records....)";
        return false;
    }

    Example example3 = ExamplesTable::getExample(newValue);
    if((example3.id() != example1.id()) || (example1.createdAt() != example3.createdAt()))
    {
        qDebug() << "TestExamplesTable::runTests(): failed(update)";
        return false;
    }

    if(!ExamplesTable::removeAll())
    {
        qDebug() << "TestExamplesTable::runTests(): failed(removeAll)";
        return false;
    }

    exampleList = ExamplesTable::selectAll();
    if(!exampleList.isEmpty())
    {
        qDebug() << "TestExamplesTable::runTests(): failed(removeAll)";
        return false;
    }

    return true;
}
