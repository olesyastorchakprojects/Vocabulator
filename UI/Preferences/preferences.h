#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

class QStackedWidget;

class Preferences : public QDialog
{
public:
    Preferences();

private slots:
    void vocabularies();

private:
    void setupStackedWidget();

private:
    QStackedWidget* _stackedWidget;
};

#endif // PREFERENCES_H
