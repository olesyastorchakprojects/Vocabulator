#ifndef PROJECTS_H
#define PROJECTS_H

#include <QObject>

#include "word.h"

class Project
{
public:
    Project(const int id, const QString& val, const QString& createdAt, const QList<Word>& words);

    QList<Word> words()const;
    QString value() const;
    int id()const;
    QString createdAt()const;

private:
    int _projectId;
    QString _createdAt;
    QString _value;
    QList<Word> _words;
};

#endif // PROJECTS_H
