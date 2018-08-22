#include "example.h"


Example::Example(const int id, const QString& name, const QString& createdAt)
    : _exampleId(id), _value(name), _createdAt(createdAt)
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
