#include "phrase.h"

Phrase::Phrase(const int id, const QString& val, const QString& createdAt, const QList<Example>& examples)
    : _phraseId(id), _createdAt(createdAt), _value(val), _examples(examples)
{

}

QString Phrase::value() const
{
    return _value;
}

int Phrase::id() const
{
    return _phraseId;
}

QString Phrase::createdAt() const
{
    return _createdAt;
}

QList<Example> Phrase::examples()const
{
    return _examples;
}

