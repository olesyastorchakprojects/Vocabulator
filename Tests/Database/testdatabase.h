#ifndef TESTDATABASE_H
#define TESTDATABASE_H


class TestDatabase
{
public:
    TestDatabase();

    bool runTests();


private:
    bool _connected;
};

#endif // TESTDATABASE_H
