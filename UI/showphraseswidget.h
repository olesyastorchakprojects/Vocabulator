#ifndef SHOWPHRASESWIDGET_H
#define SHOWPHRASESWIDGET_H

#include <QWidget>

#include "Database/phrasestable.h"

class ShowPhrasesWidget;
class QComboBox;
class QLabel;
class QPushButton;

class ShowPhrasesController : public QObject
{
    Q_OBJECT

public:
    explicit ShowPhrasesController(ShowPhrasesWidget* widget);

private slots:
    void clickedPrev(bool);
    void clickedNext(bool);
    void clickedShowExample(bool);
    void currentTextChangedInComboBox(const QString& text);

private:
    void fillData();
    void updateLabels();

    void setupConnections();

private:
    ShowPhrasesWidget*   _widget;
    QList<Phrase>            _data;
    int                      _index;
};

class ShowPhrasesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowPhrasesWidget(QWidget *parent = 0);

    QComboBox* comboBox();
    QLabel*    phraseLabel();
    QLabel*    exampleLabel();
    QPushButton* prevButton();
    QPushButton* nextButton();
    QPushButton* showExampleButton();

private:
    void setupLayout();

private:
    QComboBox*   _comboBox;
    QLabel*      _phrase;
    QLabel*      _example;
    QPushButton* _prevButton;
    QPushButton* _nextButton;
    QPushButton* _showExampleButton;
};


#endif // SHOWPHRASESWIDGET_H
