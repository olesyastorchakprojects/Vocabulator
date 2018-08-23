#include "synonym.h"

Synonym::Synonym(const int id, const QString& name, const QString& createdAt)
    : _synonymId(id), _value(name), _createdAt(createdAt), _isValid(false)
{
    if(_synonymId != -1)
        _isValid = true;
}

Synonym::Synonym()
    : _synonymId(-1), _isValid(false)
{
}

QString Synonym::value() const
{
    return _value;
}

void Synonym::setValue(const QString& value)
{
    _value = value;
}

int Synonym::id() const
{
    return _synonymId;
}

QString Synonym::createdAt() const
{
    return _createdAt;
}

bool Synonym::isValid()const
{
    return _isValid;
}
