#ifndef WORD_H
#define WORD_H

#include <QObject>

#include "definition.h"

class Word
{
public:
    Word(const int id, const QString& val, const QString& createdAt, const QList<Definition>& definitions);

    QList<Definition> definitions()const;
    QString value()const;
    int id()const;
    QString createdAt()const;

private:
    int _wordId;
    QString _createdAt;
    QString _value;
    QList<Definition> _definitions;
};

#endif // WORD_H
