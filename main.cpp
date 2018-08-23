#include "UI/mainwindow.h"
#include <QApplication>
#include <QDebug>

#include "Tests/Database/testdatabase.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication a(argc, argv);

    TestDatabase testDatabase;
    if(!testDatabase.runTests())
    {
        qDebug() << "Tests failed, quitting...";
        return 0;
    }

    MainWindow w;
    w.show();

    return a.exec();
}
