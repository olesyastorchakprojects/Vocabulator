#ifndef EXPORT_H
#define EXPORT_H

#include <QObject>
#include <QDate>
#include <QMap>

class QTextEdit;
class QListWidget;

class Export : public QObject
{
    Q_OBJECT

public:
    void exportToTxt();

private slots:
    void dateClicked(const QDate& date);

private:
    void getProjectsAndDates(QList<QString>& projects, QString &start, QString &end);

private:
    QTextEdit* _start;
    QTextEdit* _end;
    QListWidget* _listWidget;

    QMap<QString, QDate> _projectDates;
};

#endif // EXPORT_H
