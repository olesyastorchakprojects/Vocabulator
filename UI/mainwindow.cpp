#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>

#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QTextStream>
#include <QTextCursor>
#include <QWebEngineView>
#include <QTextDocument>

// to move
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTreeView>
#include <QXmlStreamReader>
#include <QNetworkRequest>
#include <QDomDocument>

#include "treemodel.h"
#include "Preferences/preferences.h"
#include "Models/jsonmodel.h"
#include "Models/dommodel.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QRect screen = QApplication::desktop()->screenGeometry();

    setMinimumSize(screen.width() * 0.75, screen.height() * 0.75 );

    setCentralWidget(new QWidget);

    QVBoxLayout* layout = new QVBoxLayout;
    _textEditContent = new QTextEdit();
    _textEditContent->setStyleSheet("font-size:24px;");
    _textEditContent->setReadOnly(true);

    _textEditWords = new QTextEdit();
    _textEditWords->setStyleSheet("font-size:24px;");
    _textEditWords->setReadOnly(true);
    _textEditWords->setMaximumHeight(height()*0.2);

    QPushButton* button = new QPushButton("Add word");
    button->setFixedWidth(200);
    connect(button, &QPushButton::clicked, this, &MainWindow::addWord);

    QPushButton* button2 = new QPushButton("Get definitions");
    button2->setFixedWidth(200);
    connect(button2, &QPushButton::clicked, this, &MainWindow::TOMOVE_getWordFromServer);

    _view = new QWebEngineView();
    _view->setZoomFactor(5);
    _view->load(QUrl("http://www.newyorker.com/tech/elements/a-visit-to-micropia-amsterdams-microbe-museum"));
    connect(_view, &QWebEngineView::loadFinished, this, &MainWindow::loadFinished);

    layout->addWidget(_view);
    //layout->addWidget(_textEditContent);
    layout->addWidget(button);
    layout->addWidget(_textEditWords);
    layout->addWidget(button2);

    centralWidget()->setLayout(layout);

    createActions();
    createMenus();
}

MainWindow::~MainWindow()
{

}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);

    fileMenu = menuBar()->addMenu(tr("&Preferences"));
    fileMenu->addAction(openPreferencesAction);
}

void MainWindow::createActions()
{
    openFileAction = new QAction(tr("&Open"), this);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    openPreferencesAction = new QAction(tr("&Open"), this);
    connect(openPreferencesAction, &QAction::triggered, this, &MainWindow::openPreferences);
}

void MainWindow::openFile()
{
    /*QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "D:\\Programming\\Projects\\Vocabulator\\test files", tr("Txt Files (*.txt)"));

    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        QString content = in.readAll();
        content.replace("\n\n","\n");
        _textEditContent->setText(content);
    }*/
}

void MainWindow::openPreferences()
{
    Preferences preferences;
    preferences.exec();
}

void MainWindow::loadFinished(bool)
{
    _view->setZoomFactor(1.5);
}

void MainWindow::addWord()
{
    /*QTextCursor cursor = _textEditContent->textCursor();
    QString text = cursor.selectedText();
    _textEditWords->append(text);*/

    QString text = _view->selectedText();
    _textEditWords->append(text);
}

QDomDocument getEntryMerriamWebster( const QString& entry )
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // 5e439aa0-5b6a-42ed-bc04-60d5dafc8714 dictionary
    // 66acad7f-e386-42dd-9033-d68e57af27a9 Thesaurus
    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QString url = QString("http://www.dictionaryapi.com/api/v1/references/collegiate/xml/%1?key=5e439aa0-5b6a-42ed-bc04-60d5dafc8714").arg(entry);
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(url)));

    loop.exec();

    QString strReply = (QString)reply->readAll();
    QDomDocument document;
    document.setContent(strReply);

    return document;
}

QJsonObject getEntryOxford( const QString& entry )
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QString app_id = "6ae681d2";
    QString app_key = "0f403d7468babf21b4d13c3f55bb496a";

    QString language = "en";
    QString word_id = entry;

    QString url = "https://od-api.oxforddictionaries.com:443/api/v1/entries/" + language + "/" + word_id.toLower();
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("app_id", app_id.toUtf8());
    request.setRawHeader("app_key", app_key.toUtf8());

    QNetworkReply* reply = manager->get(request);
    loop.exec();

    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

    return jsonResponse.object();
}

QJsonObject getEntryPearson( const QString& entry )
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(QString("http://api.pearson.com/v2/dictionaries/ldoce5/entries?search=%1").arg(entry))));

    loop.exec();

    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());

    return jsonResponse.object();
}

void MainWindow::TOMOVE_getWordFromServer()
{
    QTextDocument* document = _textEditWords->document();
    QString words = document->toPlainText();
    if(words.split("\n").isEmpty())
        return;

    QString entry = words.split("\n").at(0).trimmed();
    QRect screen = QApplication::desktop()->screenGeometry();

    {
        JsonModel* model = new JsonModel(getEntryOxford(entry) );

        QTreeView * view = new QTreeView();
        view->setModel(model);
        view->setWindowTitle(QObject::tr("Oxford vocabulary"));
        view->setMinimumSize(screen.width() * 0.3, screen.height() * 0.7);
        view->expandAll();
        view->show();
    }

    {
        JsonModel* model = new JsonModel(getEntryPearson(entry));

        QTreeView * view = new QTreeView();
        view->setModel(model);
        view->setWindowTitle(QObject::tr("Pearson vocabulary"));
        view->setMinimumSize(screen.width() * 0.3, screen.height() * 0.7);
        view->expandAll();
        view->show();
    }

    {
        DomModel* model = new DomModel(getEntryMerriamWebster(entry));

        QTreeView * view = new QTreeView();
        view->setModel(model);
        view->setWindowTitle(QObject::tr("Merriam Webster vocabulary"));
        view->setMinimumSize(screen.width() * 0.3, screen.height() * 0.7);
        view->expandAll();
        view->show();
    }
}
