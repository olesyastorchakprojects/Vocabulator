#include "testdatabase.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "Database/database.h"
#include "Config/config.h"

#include "testsynonymstable.h"

TestDatabase::TestDatabase()
{
    QSqlError err = Database::initDb(Config::testDbName());
    if (err.type() != QSqlError::NoError)
    {
        qDebug() << err;
    }
}

bool TestDatabase::runTests()
{
    if(!TestSynonymsTable::runTests())
        return false;

    return true;
}
