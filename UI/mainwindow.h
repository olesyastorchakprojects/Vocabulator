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
class QGroupBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void closeListWidgets();

private slots:
    void openFile();
    void openPhrases();
    void openPreferences();
    void addWord();
    void addPhrase();
    void addPhraseExample();
    void savePhrase();
    void TOMOVE_getWordFromServer();
    void loadFinished(bool);
    void exportToTxt();

private:
    void createMenus();
    void createActions();
    void showList(const QList<QPair<QString,QStringList>>& data, const QString& title, const QString& word);
    QGroupBox* createAddWordGroup();
    QGroupBox* createAddPhraseGroup();

private:
    QMenu *fileMenu;
    QMenu *preferencesMenu;
    QAction* openFileAction;
    QAction* openPhrasesAction;
    QAction* openPreferencesAction;
    QAction* exportAction;
    QTextEdit* _textEditContent;
    QTextEdit* _textEditWords;
    QTextEdit* _textEditPhrase;
    QTextEdit* _textEditPhraseExample;
    QWebEngineView* _view;
    QString _url;
};

#endif // MAINWINDOW_H
