#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QObject>

class Example
{
public:    
    Example(const int id, const QString& name, const QString& createdAt);

    QString value()const;
    int id()const;
    QString createdAt()const;

private:
    int _exampleId;
    QString _createdAt;
    QString _value;
};


#endif // EXAMPLE_H
