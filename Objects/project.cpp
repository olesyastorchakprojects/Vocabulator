#include "project.h"

Project::Project(const int id, const QString& val, const QString& createdAt, const QString& editedAt, const QList<Word>& words, const QString &url)
    : _projectId(id), _createdAt(createdAt), _editedAt(editedAt), _value(val), _url(url), _words(words)
{

}

Project::Project()
    : _projectId(-1)
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

QString Project::url()const
{
    return _url;
}

void Project::setEditedAt(const QString& edited)
{
    _editedAt = edited;
}

QString Project::editedAt()const
{
    return _editedAt;
}
