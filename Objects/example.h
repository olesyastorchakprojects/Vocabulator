#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QObject>

class Example
{
public:    
    Example(const int id, const QString& name, const QString& createdAt);
    Example();

    QString value()const;
    int id()const;
    QString createdAt()const;

    bool isValid()const;

    void setValue(const QString &value);

private:
    int _exampleId;
    bool _isValid;

    QString _createdAt;
    QString _value;
};


#endif // EXAMPLE_H
