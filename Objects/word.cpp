#include "word.h"

Word::Word(const int id, const QString& val, const QString& createdAt, const QList<Definition>& definitions)
    : _wordId(id), _createdAt(createdAt), _value(val), _definitions(definitions)
{

}

QList<Definition> Word::definitions()const
{
    return _definitions;
}

QString Word::value()const
{
    return _value;
}

int Word::id()const
{
    return _wordId;
}

QString Word::createdAt()const
{
    return _createdAt;
}



