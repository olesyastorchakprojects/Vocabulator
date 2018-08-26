#ifndef DEFINITIONWIDGET_H
#define DEFINITIONWIDGET_H

#include <QWidget>
#include <QMap>
#include "Objects/word.h"

class QLabel;
class QPushButton;
class QTreeWidget;
class DefinitionWidget;
class QTreeWidgetItem;
class QComboBox;

class WordsListController : public QObject
{
    Q_OBJECT
public:
    WordsListController( DefinitionWidget* widget, const QString& word, const QString& url, const QString& webPageTitle);

signals:
    void addedToProject();

private slots:
    void clickedAddWordToVocabulary(bool);
    //void clickedAddToProject(bool);
    void itemDoubleClicked(QTreeWidgetItem*item, int);

private:
    void setupConnections();

private:
    DefinitionWidget* _definitionsWidget;
    QComboBox*   _comboBox;
    QString      _word;
    QString      _url;
    QString      _webPageTitle;
    QMap<QString, int> _projects;
};

/****************************************************/
class DefinitionWidget : public QWidget
{
    Q_OBJECT

public:
    DefinitionWidget(const QList<QPair<QString,QStringList>>& data, const QString& title, const QString& word);
    QPushButton* addDefinitionButton();
    QTreeWidget* definitionsTreeWidget();

private:
    void setupLayout();

private:
    QPushButton* _addDefinitionButton;
    QTreeWidget* _definitionsTreeWidget;
    QList<QPair<QString,QStringList>> _data;
    QString _title;
    QString _word;
};

#endif // DEFINITIONWIDGET_H
