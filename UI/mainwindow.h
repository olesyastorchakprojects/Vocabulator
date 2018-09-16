#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QComboBox>

#include "Objects/word.h"
#include "Objects/project.h"

class QMenu;
class QAction;
class QTextEdit;
class QWebEngineView;
class QListWidgetItem;
class QTreeWidget;
class QLabel;
class QTreeWidgetItem;
class QGroupBox;
class QNetworkAccessManager;
class QTimer;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void closeListWidgets();

private slots:
    void openUrl();
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
    void highlightWords(bool lookAll = false);
    void highlightPhrases();
    void selectionChanged();
    void openProject();
    void jsTimeout();
    void textChangedAddPhraseExample();

private:
    void createMenus();
    void createActions();
    void showList(const QList<QPair<QString,QStringList>>& data, const QString& title, const QString& word);
    QGroupBox* createAddWordGroup();
    QGroupBox* createAddPhraseGroup();
    void insertStyleSheet(const QString &name, const QString &source, bool immediately);


private:
    QMenu *fileMenu;
    QMenu *preferencesMenu;
    QAction* openFileAction;
    QAction* openUrlAction;
    QAction* openPhrasesAction;
    QAction* openPreferencesAction;
    QAction* exportAction;
    QAction* openProjectAction;
    QTextEdit* _textEditContent;
    QTextEdit* _textEditWords;
    QTextEdit* _definition;
    QTextEdit* _textEditPhrase;
    QTextEdit* _textEditPhraseExample;
    QWebEngineView* _view;
    QString _url;
    QString jQuery;
    QString jMark;
    Project _project;
    QNetworkAccessManager* _pearsonNetworkManager;
    QNetworkAccessManager* _oxfordNetworkManager;
    QTimer* _jsTimer;
    QList<Project> _projects;
};

#endif // MAINWINDOW_H
