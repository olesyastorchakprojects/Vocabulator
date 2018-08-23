#include "testsynonymstable.h"

#include <QDebug>

#include "Objects/synonym.h"
#include "Database/synonymstable.h"

bool TestSynonymsTable::runTests()
{
    SynonymsTable::removeAll();

    QString synonymValue = "tree";
    int synonymId = -1;
    if(( synonymId = SynonymsTable::insertSynonym(synonymValue, 111)) == -1)
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(insert/get by name)";
        return false;
    }

    Synonym synonym1 = SynonymsTable::getSynonym(synonymValue);
    if(!synonym1.isValid())
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(insert/get by name)";
        return false;
    }

    Synonym synonym2 = SynonymsTable::getSynonym(synonym1.id());
    if(!synonym2.isValid())
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(insert/get by id)";
        return false;
    }

    QList<Synonym> synonymList = SynonymsTable::selectAll();
    if(synonymList.isEmpty() || !synonymList.at(0).isValid())
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(selectAll)";
        return false;
    }

    if(!SynonymsTable::removeSynonym(synonymValue))
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(removeSynonym)";
        return false;
    }

    synonymList = SynonymsTable::selectAll();
    if(!synonymList.isEmpty())
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(removeSynonym)";
        return false;
    }

    if(( synonymId = SynonymsTable::insertSynonym(synonymValue, 111)) == -1)
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(insert/get by name)";
        return false;
    }

    synonym1 = SynonymsTable::getSynonym(synonymValue);
    if(!synonym1.isValid())
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(insert/get by name)";
        return false;
    }

    QString newValue = "new value";
    synonym1.setValue(newValue);
    if(!SynonymsTable::updateSynonym(synonym1))
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(update)";
        return false;
    }

    Synonym synonym3 = SynonymsTable::getSynonym(newValue);
    if((synonym3.id() != synonym1.id()) || (synonym1.createdAt() != synonym3.createdAt()))
    {
        qDebug() << "TestSynonymsTable::runTests(): failed(update)";
        return false;
    }

    return true;
}
