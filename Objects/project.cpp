#include "project.h"

Project::Project(const int id, const QString& val, const QString& createdAt, const QList<Word>& words)
    : _projectId(id), _createdAt(createdAt), _value(val), _words(words)
{

}

QString Project::value() const
{
    return _value;
}

int Project::id() const
{
    return _projectId;
}

QString Project::createdAt() const
{
    return _createdAt;
}

QList<Word> Project::words()const
{
    return _words;
}
