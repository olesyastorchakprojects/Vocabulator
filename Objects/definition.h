#ifndef DEFINITION_H
#define DEFINITION_H

#include <QObject>

#include "example.h"
#include "synonym.h"

class Definition
{
public:
    Definition(const int id, const QString& val, const QString& createdAt, const QList<Synonym>& synonyms, const QList<Example>& examples);

    QString value() const;
    int id()const;
    QString createdAt()const;
    QList<Example> examples()const;
    QList<Synonym> synonyms()const;

private:
    int _definitionId;
    QString _createdAt;
    QString _value;
    QList<Synonym> _synonyms;
    QList<Example> _examples;
};



#endif // DEFINITION_H
