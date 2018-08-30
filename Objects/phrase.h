#ifndef PHRASE_H
#define PHRASE_H

#include <QObject>

#include "example.h"

class Phrase
{
public:
    Phrase(const int id, const QString& val, const QString& createdAt, const QList<Example>& examples);

    QString value() const;
    int id()const;
    QString createdAt()const;
    QList<Example> examples()const;

private:
    int _phraseId;
    QString _createdAt;
    QString _value;
    QList<Example> _examples;
};


#endif // PHRASE_H
