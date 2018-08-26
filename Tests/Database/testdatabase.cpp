#include "testdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "Database/database.h"
#include "Config/config.h"

#include "testsynonymstable.h"
#include "testexamplestable.h"
#include "testprojectstable.h"

bool TestDatabase::runTests()
{
    if(!Database::initDb(Config::testDbName()))
    {
        qDebug() << "Failed to init test db, quitting...";
        return false;
    }

    if(!TestSynonymsTable::runTests())
        return false;

    if(!TestExamplesTable::runTests())
        return false;

    if(!TestProjectsTable::runTests())
        return false;

    Database::closeDb();

    return true;
}
