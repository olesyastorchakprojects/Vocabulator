#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QRect>

namespace Config
{
    QString testDbName();
    QString dbName();
    QRect screenGeometry();
}

#endif // CONFIG_H
