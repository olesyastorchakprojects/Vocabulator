#ifndef SYNONYM_H
#define SYNONYM_H

#include <QObject>

class Synonym
{
public:

    Synonym(const int id, const QString& name, const QString& createdAt);
    Synonym();

    QString value()const;
    int id()const;
    QString createdAt()const;

    bool isValid()const;

private:
    int _synonymId;
    bool _isValid;

    QString _createdAt;
    QString _value;    
};


#endif // SYNONYM_H
