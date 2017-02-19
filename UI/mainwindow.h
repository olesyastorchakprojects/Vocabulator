#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QMenu;
class QAction;
class QTextEdit;
class QWebEngineView;

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

private:
    void createMenus();
    void createActions();

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
