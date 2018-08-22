#ifndef OXFORDENTRY_H
#define OXFORDENTRY_H

#include <QObject>
#include <QJsonObject>

class Sense : public QObject
{
    Q_OBJECT

public:
    explicit Sense(const QJsonObject& json);

private:
    QString _definition;
    QString _domain;
    QString _example;

    QList<Sense> _subsenses;
};

class OxfordEntry : public QObject
{
    Q_OBJECT
public:
    explicit OxfordEntry(const QJsonObject& json, QObject *parent = 0);

private:
    QString _lexicalCategory;
    QString _etymologies;

    QList<Sense> _senses;
};

#endif // OXFORDENTRY_H
