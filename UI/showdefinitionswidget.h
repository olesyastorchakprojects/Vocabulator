#ifndef SHOWDEFINITIONSWIDGET_H
#define SHOWDEFINITIONSWIDGET_H

#include <QWidget>

#include "Database/wordstable.h"

class ShowDefinitionsWidget;
class QComboBox;
class QLabel;
class QPushButton;

class ShowDefinitionsController : public QObject
{
    Q_OBJECT

public:
    explicit ShowDefinitionsController(ShowDefinitionsWidget* widget);

private slots:
    void clickedPrev(bool);
    void clickedNext(bool);
    void clickedShowDefinition(bool);
    void currentTextChangedInComboBox(const QString& text);

private:
    void fillData();
    void updateLabels();

    void setupConnections();

private:
    ShowDefinitionsWidget*   _widget;
    QList<Word>              _data;
    int                      _index;
};

class ShowDefinitionsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ShowDefinitionsWidget(QWidget *parent = 0);

    QComboBox* comboBox();
    QLabel*    wordLabel();
    QLabel*    definitionLabel();
    QPushButton* prevButton();
    QPushButton* nextButton();
    QPushButton* showDefinitionButton();

private:
    void setupLayout();

private:
    QComboBox*   _comboBox;
    QLabel*      _word;
    QLabel*      _definition;
    QPushButton* _prevButton;
    QPushButton* _nextButton;
    QPushButton* _showDefinitionButton;
};

#endif // SHOWDEFINITIONSWIDGET_H
