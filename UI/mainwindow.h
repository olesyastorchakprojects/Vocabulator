#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QComboBox>

#include "Objects/word.h"

class QMenu;
class QAction;
class QTextEdit;
class QWebEngineView;
class QListWidgetItem;
class QTreeWidget;
class QLabel;
class QTreeWidgetItem;

class WordsListController : public QObject
{
    Q_OBJECT
public:
    WordsListController( QTreeWidget* widget, const QString& word) : _treeWidget(widget), _word(word){}

private slots:
    void clickedWord(bool);
    void clickedProject(bool);
    void itemDoubleClicked(QTreeWidgetItem*item, int);

private:
    QTreeWidget* _treeWidget;
    QComboBox*   _comboBox;
    QString      _word;
    QMap<QString, int> _projects;
};

class ShowDefinitionsController : public QObject
{
    Q_OBJECT

public:
    ShowDefinitionsController(QLabel* word, QLabel* definition, QComboBox* combo): _comboBox(combo), _word(word), _definition(definition), _index(0)
    {
        currentTextChangedInComboBox(_comboBox->currentText());
    }

private slots:
    void clickedPrev(bool);
    void clickedNext(bool);
    void clickedShowDefinition(bool);
    void currentTextChangedInComboBox(const QString& text);

private:
    void fillData();
    void updateLabels();

private:
    QComboBox*   _comboBox;
    QLabel*      _word;
    QLabel*      _definition;
    QList<Word> _data;
    int          _index;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openFile();
    void openPreferences();
    void addWord();
    void TOMOVE_getWordFromServer();
    void loadFinished(bool);
    void highlightChecked(QListWidgetItem*);

private:
    void createMenus();
    void createActions();
    void showList(const QList<QPair<QString,QStringList>>& data, const QString& title, const QString& word);

private:
    QMenu *fileMenu;
    QMenu *preferencesMenu;
    QAction* openFileAction;
    QAction* openPreferencesAction;
    QTextEdit* _textEditContent;
    QTextEdit* _textEditWords;
    QWebEngineView* _view;
};

#endif // MAINWINDOW_H
