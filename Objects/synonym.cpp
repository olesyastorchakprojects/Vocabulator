#include "synonym.h"

Synonym::Synonym(const int id, const QString& name, const QString& createdAt)
    : _synonymId(id), _value(name), _createdAt(createdAt)
{

}

QString Synonym::value() const
{
    return _value;
}

int Synonym::id() const
{
    return _synonymId;
}

QString Synonym::createdAt() const
{
    return _createdAt;
}
