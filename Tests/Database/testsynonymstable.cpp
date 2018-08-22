#include "testsynonymstable.h"

#include <QDebug>

#include "Objects/synonym.h"
#include "Database/synonymstable.h"

bool TestSynonymsTable::runTests()
{
    SynonymsTable::removeAll();

    QString synonymValue = "synonym";
    SynonymsTable::insertSynonym(synonymValue, 111);

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

    return true;
}
