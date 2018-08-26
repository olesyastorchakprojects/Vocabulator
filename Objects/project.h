#ifndef PROJECTS_H
#define PROJECTS_H

#include <QObject>

#include "word.h"

class Project
{
public:
    Project(const int id, const QString& val, const QString& createdAt, const QString& editedAt, const QList<Word>& words, const QString& url);
    Project();

    QList<Word> words()const;
    QString value() const;
    int id()const;
    QString createdAt()const;
    QString url()const;

    void setEditedAt(const QString& edited);
    QString editedAt()const;

private:
    int _projectId;
    QString _createdAt;
    QString _editedAt;
    QString _value;
    QString _url;
    QList<Word> _words;
};

#endif // PROJECTS_H
