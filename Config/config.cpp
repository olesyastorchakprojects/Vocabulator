#include "config.h"

#include <QApplication>
#include <QDesktopWidget>

namespace Config
{
    QString testDbName()
    {
        return QString("test_vocabulary.db");
    }

    QString dbName()
    {
        return QString("vocabulary.db");
    }

    QRect screenGeometry()
    {
        return QApplication::desktop()->screenGeometry();
    }
}
