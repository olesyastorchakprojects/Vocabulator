#include "UI/mainwindow.h"
#include <QApplication>

#include "Tests/Database/testdatabase.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    TestDatabase testDatabase;
    testDatabase.runTests();

    MainWindow w;
    w.show();

    return a.exec();
}
