#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QObject>

class Example
{
public:
    enum ExampleType
    {
        EXAMPLE_WORD = 1,
        EXAMPLE_PHRASE
    };

    Example(const int id, const QString& name, const QString& createdAt, const ExampleType exampleType);
    Example();

    QString value()const;
    int id()const;
    QString createdAt()const;

    ExampleType exampleType()const;

    bool isValid()const;

    void setValue(const QString &value);

private:
    int _exampleId;
    bool _isValid;

    ExampleType _exampleType;

    QString _createdAt;
    QString _value;
};


#endif // EXAMPLE_H
