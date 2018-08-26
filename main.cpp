#include "UI/mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "Tests/Database/testdatabase.h"
#include "Database/database.h"
#include "Config/config.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    if(!TestDatabase::runTests())
    {
        qDebug() << "Tests failed, quitting...";
        return 0;
    }

    if (!Database::initDb(Config::dbName()))
    {
        qDebug() << "Failed to init db, quitting...";
        return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
