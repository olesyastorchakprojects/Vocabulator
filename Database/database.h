#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlError>

#include "Objects/word.h"
#include "Objects/project.h"

class Database : public QObject
{
    Q_OBJECT
public:

    static QSqlError initDb(const QString& dbName);
    static QString currentTime();
};

#endif // DATABASE_H
