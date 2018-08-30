#include "mainwindow.h"

#include <QApplication>

#include <QDebug>
#include <QFile>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QTextStream>
#include <QTextCursor>
#include <QWebEngineView>
#include <QTextDocument>
#include <QMessageBox>
#include <QLabel>
#include <QSqlDatabase>
#include <QTreeView>
#include <QGroupBox>

// to move
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QNetworkRequest>
#include <QDomDocument>

#include "treemodel.h"
#include "Preferences/preferences.h"
#include "Models/jsonmodel.h"
#include "Models/dommodel.h"
#include "Database/database.h"
#include "Database/projectstable.h"
#include "Database/wordstable.h"
#include "Database/definitionstable.h"
#include "Database/phrasestable.h"
#include "Database/examplestable.h"
#include "Config/config.h"
#include "definitionwidget.h"
#include "showdefinitionswidget.h"
#include "Export/export.h"
#include "UI/AddDefinition/selectprojectwidget.h"
#include "showphraseswidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QRect screen = Config::screenGeometry();

    setMinimumSize(screen.width() * 0.75, screen.height() * 0.75 );

    setCentralWidget(new QWidget);

    QVBoxLayout* layout = new QVBoxLayout;

    _textEditWords = new QTextEdit();
    //_textEditWords->setStyleSheet("font-size:24px;");
    _textEditWords->setReadOnly(true);
    _textEditWords->setMaximumHeight(height()*0.1);
    _textEditWords->setReadOnly(false);

    _view = new QWebEngineView();
    _url = "https://www.wsj.com/articles/dicks-says-under-armour-new-gun-sales-policy-dragged-on-results-1535565173";
    _view->load(QUrl(_url));
    connect(_view, &QWebEngineView::loadFinished, this, &MainWindow::loadFinished);

    layout->addWidget(_view);

    /*QHBoxLayout* bottomLayout = new QHBoxLayout;
    QVBoxLayout* buttonsLayout = new QVBoxLayout;

    buttonsLayout->addWidget(button);
    buttonsLayout->addWidget(button2);

    bottomLayout->addWidget(_textEditWords);
    bottomLayout->addLayout(buttonsLayout);*/

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(createAddWordGroup(), 0, 0);
    grid->addWidget(createAddPhraseGroup(), 0, 1);
    setLayout(grid);

    layout->addLayout(grid);
    centralWidget()->setLayout(layout);

    createActions();
    createMenus();
}

QGroupBox* MainWindow::createAddWordGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Add word:"));

    QHBoxLayout *vbox = new QHBoxLayout;

    vbox->addWidget(_textEditWords);

    QPushButton* button = new QPushButton("Add word");
    button->setFixedWidth(200);
    connect(button, &QPushButton::clicked, this, &MainWindow::addWord);

    QPushButton* button2 = new QPushButton("Get definitions");
    button2->setFixedWidth(200);
    connect(button2, &QPushButton::clicked, this, &MainWindow::TOMOVE_getWordFromServer);

    QVBoxLayout* buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(button);
    buttonsLayout->addWidget(button2);

    vbox->addLayout(buttonsLayout);
    groupBox->setLayout(vbox);

    groupBox->setMaximumHeight(height()*0.13);
    groupBox->setMaximumWidth(width()*0.4);

    return groupBox;
}

QGroupBox* MainWindow::createAddPhraseGroup()
{
    QGroupBox *groupBox = new QGroupBox(tr("Add phrase:"));

    QHBoxLayout *vbox = new QHBoxLayout;

    QVBoxLayout* editsLayout = new QVBoxLayout;

    _textEditPhrase = new QTextEdit();
    //_textEditPhrase->setStyleSheet("font-size:24px;");
    _textEditPhrase->setReadOnly(true);
    _textEditPhrase->setMaximumHeight(height()*0.1);
    _textEditPhrase->setReadOnly(false);

    _textEditPhraseExample = new QTextEdit();
    //_textEditPhraseExample->setStyleSheet("font-size:24px;");
    _textEditPhraseExample->setReadOnly(true);
    _textEditPhraseExample->setMaximumHeight(height()*0.1);
    _textEditPhraseExample->setReadOnly(false);

    editsLayout->addWidget(_textEditPhrase);
    editsLayout->addWidget(_textEditPhraseExample);

    QPushButton* button1 = new QPushButton("Add phrase");
    button1->setFixedWidth(200);
    connect(button1, &QPushButton::clicked, this, &MainWindow::addPhrase);

    QPushButton* button2 = new QPushButton("Add example");
    button2->setFixedWidth(200);
    connect(button2, &QPushButton::clicked, this, &MainWindow::addPhraseExample);

    QPushButton* button3 = new QPushButton("Save phrase");
    button3->setFixedWidth(200);
    connect(button3, &QPushButton::clicked, this, &MainWindow::savePhrase);

    QVBoxLayout* buttonsLayout = new QVBoxLayout;
    buttonsLayout->addWidget(button1);
    buttonsLayout->addWidget(button2);
    buttonsLayout->addWidget(button3);

    vbox->addLayout(editsLayout);
    vbox->addLayout(buttonsLayout);
    groupBox->setLayout(vbox);

    groupBox->setMaximumHeight(height()*0.13);

    return groupBox;
}

MainWindow::~MainWindow()
{

}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(openPhrasesAction);
    fileMenu->addAction(exportAction);

    fileMenu = menuBar()->addMenu(tr("&Preferences"));
    fileMenu->addAction(openPreferencesAction);
}

void MainWindow::createActions()
{
    openFileAction = new QAction(tr("&Open words"), this);
    connect(openFileAction, &QAction::triggered, this, &MainWindow::openFile);

    openPreferencesAction = new QAction(tr("&Open"), this);
    connect(openPreferencesAction, &QAction::triggered, this, &MainWindow::openPreferences);

    exportAction = new QAction(tr("&Export to txt"), this);
    connect(exportAction, &QAction::triggered, this, &MainWindow::exportToTxt);

    openPhrasesAction = new QAction(tr("&Open phrases"), this);
    connect(openPhrasesAction, &QAction::triggered, this, &MainWindow::openPhrases);
}


void MainWindow::openFile()
{
    ShowDefinitionsWidget* widget = new ShowDefinitionsWidget();

    ShowDefinitionsController* controller = new ShowDefinitionsController(widget);

    widget->show();
}

void MainWindow::openPhrases()
{
    ShowPhrasesWidget* widget = new ShowPhrasesWidget();

    ShowPhrasesController* controller = new ShowPhrasesController(widget);

    widget->show();
}


void MainWindow::openPreferences()
{
    Preferences preferences;
    preferences.exec();
}

void MainWindow::exportToTxt()
{
    Export exp;
    exp.exportToTxt();
}

void MainWindow::loadFinished(bool)
{
    //_view->setZoomFactor(1.5);
    //QString title = _view->page()->title();
}

void MainWindow::addWord()
{
    QString text = _view->selectedText();
    _textEditWords->clear();
    _textEditWords->append(text);
}

void MainWindow::addPhrase()
{
    QString text = _view->selectedText();
    _textEditPhrase->clear();
    _textEditPhrase->append(text);
}

void MainWindow::addPhraseExample()
{
    QString text = _view->selectedText();
    _textEditPhraseExample->clear();
    _textEditPhraseExample->append(text);
}

void MainWindow::savePhrase()
{
    SelectProjectWidget widget(_view->page()->title(), _url);
    SelectProjectController controller(&widget);
    widget.exec();
    int projectId = controller.selectedProjectId();
    QString phrase = _textEditPhrase->toPlainText();
    QString example = _textEditPhraseExample->toPlainText();

    if(projectId != -1 && !phrase.isEmpty() && !example.isEmpty())
    {
        int phraseId = PhrasesTable::insertPhrase(phrase, projectId);
        if(phraseId != -1)
        {
            ProjectsTable::updateEdited(projectId, Database::currentTime());
            ExamplesTable::insertExample(example, phraseId, Example::EXAMPLE_PHRASE);
        }
    }
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

void MainWindow::showList(const QList<QPair<QString,QStringList>>& data, const QString& title, const QString& word)
{
    DefinitionWidget* definitionsWidget = new DefinitionWidget(data, title, word);
    WordsListController* controller = new WordsListController(definitionsWidget, word, _url, _view->page()->title());

    connect(controller, SIGNAL(addedToProject()), this, SIGNAL(closeListWidgets()));
    connect(this, SIGNAL(closeListWidgets()), definitionsWidget, SLOT(close()));

    definitionsWidget->show();
}

void MainWindow::TOMOVE_getWordFromServer()
{
    QTextDocument* document = _textEditWords->document();
    QString words = document->toPlainText();
    if(words.split("\n").isEmpty())
        return;

    QString entry = words.split("\n").at(0).trimmed();
    QRect screen = Config::screenGeometry();

    {
        QList<QPair<QString,QStringList>> definitions;
        JsonModel* model = new JsonModel(getEntryOxford(entry), definitions );

        QTreeView * view = new QTreeView();
        view->setModel(model);
        view->setWindowTitle(QObject::tr("Oxford vocabulary"));
        view->setMinimumSize(screen.width() * 0.3, screen.height() * 0.7);
        view->expandAll();
        //view->show();

        showList(definitions, "Oxford vocabulary", entry);
    }

    {
        QList<QPair<QString,QStringList>> definitions;
        JsonModel* model = new JsonModel(getEntryPearson(entry), definitions);

        QTreeView * view = new QTreeView();
        view->setModel(model);
        view->setWindowTitle(QObject::tr("Pearson vocabulary"));
        view->setMinimumSize(screen.width() * 0.3, screen.height() * 0.7);
        view->expandAll();
        //view->show();

        showList(definitions, "Pearson vocabulary", entry);
    }

   /* {
        DomModel* model = new DomModel(getEntryMerriamWebster(entry));

        QTreeView * view = new QTreeView();
        view->setModel(model);
        view->setWindowTitle(QObject::tr("Merriam Webster vocabulary"));
        view->setMinimumSize(screen.width() * 0.3, screen.height() * 0.7);
        view->expandAll();
        view->show();
    }*/
}
