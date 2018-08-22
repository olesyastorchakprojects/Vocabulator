#include "definition.h"

Definition::Definition(const int id, const QString& val, const QString& createdAt, const QList<Synonym>& synonyms, const QList<Example>& examples)
    : _definitionId(id), _createdAt(createdAt), _value(val), _synonyms(synonyms), _examples(examples)
{

}

QString Definition::value() const
{
    return _value;
}

int Definition::id() const
{
    return _definitionId;
}

QString Definition::createdAt() const
{
    return _createdAt;
}

QList<Example> Definition::examples()const
{
    return _examples;
}

QList<Synonym> Definition::synonyms()const
{
    return _synonyms;
}
