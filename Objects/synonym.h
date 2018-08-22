#ifndef SYNONYM_H
#define SYNONYM_H

#include <QObject>

class Synonym
{
public:

    Synonym(const int id, const QString& name, const QString& createdAt);

    QString value()const;
    int id()const;
    QString createdAt()const;

private:
    int _synonymId;
    QString _createdAt;
    QString _value;
};


#endif // SYNONYM_H
