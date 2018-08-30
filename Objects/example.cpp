#include "example.h"


Example::Example(const int id, const QString& name, const QString& createdAt, const ExampleType exampleType)
    : _exampleId(id), _value(name), _createdAt(createdAt), _exampleType(exampleType)
{
    _isValid = (_exampleId != -1);
}

Example::Example()
    : _exampleId(-1), _isValid(false)
{

}

QString Example::value() const
{
    return _value;
}

int Example::id() const
{
    return _exampleId;
}

QString Example::createdAt() const
{
    return _createdAt;
}

bool Example::isValid()const
{
    return _isValid;
}

void Example::setValue(const QString &value)
{
    _value = value;
}

Example::ExampleType Example::exampleType()const
{
    return _exampleType;
}
