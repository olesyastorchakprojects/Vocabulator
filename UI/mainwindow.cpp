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
#include <QListWidget>
#include <QTreeWidget>
#include <QComboBox>
#include <QMessageBox>
#include <QLabel>
#include <QSqlDatabase>
#include <QList>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QFile>
#include <QTextStream>

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
#include "Database/database.h"
#include "Database/projectstable.h"
#include "Database/wordstable.h"
#include "Database/definitionstable.h"
#include "Database/examplestable.h"

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
    _textEditWords->setReadOnly(false);

    QPushButton* button = new QPushButton("Add word");
    button->setFixedWidth(200);
    connect(button, &QPushButton::clicked, this, &MainWindow::addWord);

    QPushButton* button2 = new QPushButton("Get definitions");
    button2->setFixedWidth(200);
    connect(button2, &QPushButton::clicked, this, &MainWindow::TOMOVE_getWordFromServer);

    _view = new QWebEngineView();
    _view->setZoomFactor(5);
    _view->load(QUrl("https://www.wsj.com/articles/wall-street-erases-the-line-between-its-jocks-and-nerds-1534564810"));
    connect(_view, &QWebEngineView::loadFinished, this, &MainWindow::loadFinished);

    layout->addWidget(_view);
    //layout->addWidget(_textEditContent);
    layout->addWidget(button);
    layout->addWidget(_textEditWords);
    layout->addWidget(button2);

    centralWidget()->setLayout(layout);

    createActions();
    createMenus();

    //_textEditWords->append("earth");
    //TOMOVE_getWordFromServer();

    if (!QSqlDatabase::drivers().contains("QSQLITE"))
        QMessageBox::critical(this, "Unable to load database", "This demo needs the SQLITE driver");

    // initialize the database
    QSqlError err = Database::initDb("vocabulary.db");
    if (err.type() != QSqlError::NoError) {
        qDebug() << err;
        return;
    }

    // export
    {
        QString filename = "Data.txt";
        QFile file(filename);
        if (file.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&file);

            Project* proj = ProjectsTable::project("Bad blood");
            if(proj)
            {
                QList<Word> data = proj->words();
                int index = 0;
                foreach (Word word, data)
                {
                    stream << QString::number(++index) << ". " << word.value() << "\r\n";
                    QList<Definition> defs = word.definitions();
                    foreach (Definition def, defs)
                    {
                        stream << "Definition: " << def.value() << "\r\n";
                        QList<Example> exs = def.examples();
                        foreach (Example ex, exs)
                        {
                            stream << "Example: " << ex.value() << "\r\n";
                        }
                    }
                   stream << "\r\n";
                }
            }
        }


    }
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

void ShowDefinitionsController::clickedPrev(bool)
{
    if(--_index < 0)
        _index = _data.size()-1;

    updateLabels();
}

void ShowDefinitionsController::clickedNext(bool)
{
    if(++_index == _data.size())
        _index = 0;

    updateLabels();
}

void ShowDefinitionsController::clickedShowDefinition(bool)
{
    if(_data.isEmpty())
        return;

    QList<Definition> defs = _data[_index].definitions();
    QString defStr;
    foreach (const Definition& def, defs)
    {
        defStr += def.value();
        QList<Example> exs = def.examples();
        defStr += "\n";
        foreach (const Example& ex, exs)
        {
            defStr += "Example: ";
            defStr += ex.value();
            defStr += "\n";
        }
        defStr += "\n\n";
    }
    _definition->setText(QString("Definition: %1").arg(defStr));
}

void ShowDefinitionsController::updateLabels()
{
    if(_data.isEmpty())
        return;

    _word->setText(_data[_index].value());

    _definition->setText("Definition");
}

void ShowDefinitionsController::currentTextChangedInComboBox(const QString& text)
{
    _data.clear();
    _index = 0;
    fillData();
    updateLabels();
}

void ShowDefinitionsController::fillData()
{
    QString projectName =_comboBox->currentText();
    Project* proj = ProjectsTable::project(projectName);
    if(proj)
    {
        _data = proj->words();
    }
}

void MainWindow::openFile()
{
    QWidget* widget = new QWidget();
    QRect screen = QApplication::desktop()->screenGeometry();
    widget->setMinimumSize(screen.width() * 0.4, screen.height() * 0.4 );

    QVBoxLayout* layout = new QVBoxLayout;

    QComboBox* comboBox = new QComboBox;
    comboBox->setEditable(true);

    QStringList projects = ProjectsTable::projectNames();
    foreach (const QString& p, projects)
    {
        comboBox->addItem(p);
    }

    layout->addWidget(comboBox);

    QLabel* word = new QLabel;
    word->setStyleSheet(QString("font:14px"));
    layout->addWidget(word);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    QPushButton* prevButton = new QPushButton("<<");
    QPushButton* nextButton = new QPushButton(">>");
    QPushButton* showButton = new QPushButton("show definition");

    buttonsLayout->addWidget(prevButton);
    buttonsLayout->addWidget(nextButton);
    buttonsLayout->addWidget(showButton);

    layout->addLayout(buttonsLayout);

    QLabel* definition = new QLabel;
    definition->setStyleSheet(QString("font:14px"));
    layout->addWidget(definition);

    widget->setLayout(layout);


    ShowDefinitionsController* controller = new ShowDefinitionsController(word, definition, comboBox);

    connect(prevButton, SIGNAL(clicked(bool)),controller, SLOT(clickedPrev(bool)));
    connect(nextButton, SIGNAL(clicked(bool)),controller, SLOT(clickedNext(bool)));
    connect(showButton, SIGNAL(clicked(bool)),controller, SLOT(clickedShowDefinition(bool)));

    connect(comboBox, SIGNAL(currentIndexChanged(QString)), controller, SLOT(currentTextChangedInComboBox(QString)));

    widget->show();
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

    // parse document
    {
        if(jsonResponse.isArray())
        {

        }
        else if(jsonResponse.isObject())
        {
            qDebug() << jsonResponse.object().keys();
            qDebug() << jsonResponse.object()["results"].toArray().at(0).toObject()["lexicalEntries"].toArray().count();
            qDebug() << jsonResponse.object()["results"].toArray().at(0).toObject()["lexicalEntries"].toArray().at(0);
        }
    }
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

void MainWindow::highlightChecked(QListWidgetItem *item){
    if(item->checkState() == Qt::Checked)
    {
        QString data = item->data(Qt::DisplayRole).toString();

        //QMessageBox::information(NULL, "Save definition", )
    }
}

void WordsListController::clickedProject(bool)
{
    QString projectName = _comboBox->currentText();

    QStringList definitions;

    QTreeWidgetItem* item = 0;
    for(int i = 0; i < _treeWidget->topLevelItemCount(); ++i)
    {
        item = _treeWidget->topLevelItem(i);

        if(item->checkState(0) == Qt::Checked)
        {
            definitions.push_back(item->text(0));
        }
    }

    int projectId = -1;
    if(!_projects.contains(projectName))
    {
        projectId = ProjectsTable::insertProject(projectName);
        _projects[projectName] = projectId;
        if(projectId == -1)
            return;
    }

    if(projectId == -1)
        projectId = _projects[projectName];
    int wordId = WordsTable::insertWord(_word, projectId);

    for(int i = 0; i < _treeWidget->topLevelItemCount(); ++i)
    {
        item = _treeWidget->topLevelItem(i);

        if(item->checkState(0) == Qt::Checked)
        {
            int definitionId = -1;
            if(wordId != -1)
            {
                definitionId = DefinitionsTable::insertDefinition(item->text(0), wordId);
                if(definitionId != -1)
                {
                    QTreeWidgetItem* child = 0;
                    for( int j = 0; j < item->childCount(); ++j)
                    {
                        child = item->child(j);
                        if(child->checkState(0) == Qt::Checked)
                        {
                            ExamplesTable::insertExample(child->text(0), definitionId);
                        }
                    }
                }
            }
        }
    }

    /*foreach(const QString& def, definitions)
    {
        int wordId = Database::insertWord(_word, projectId);
        if(wordId != -1)
        {
            Database::insertDefinition(def, wordId);
        }
    }*/
}

void WordsListController::clickedWord(bool)
{
    QStringList definitions;

    QTreeWidgetItem* item = 0;
    for(int i = 0; i < _treeWidget->topLevelItemCount(); ++i)
    {
        item = _treeWidget->topLevelItem(i);

        if(item->checkState(0) == Qt::Checked)
        {
            definitions.push_back(item->text(0));
        }
    }

    if(!definitions.isEmpty())
    {
        QWidget* widget = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout;

        widget->setMinimumSize(500, 500);

        _comboBox = new QComboBox;
        _comboBox->setEditable(true);

        QList<QPair<int, QString>> projects = ProjectsTable::projectNamesIds();
        QPair<int, QString> pair;
        foreach (pair, projects)
        {
            _projects.insert(pair.second, pair.first);
            _comboBox->addItem(pair.second);
        }

        QPushButton* button = new QPushButton("Add to project");
        connect(button, SIGNAL(clicked(bool)), this, SLOT(clickedProject(bool)));
        connect(button, SIGNAL(clicked(bool)), widget, SLOT(close()));

        layout->addWidget(_comboBox);
        layout->addWidget(button);

        widget->setLayout(layout);
        widget->show();
    }
}

void WordsListController::itemDoubleClicked(QTreeWidgetItem* item,int)
{
    if( item->parent() == NULL)
    {
        QDialog* widget = new QDialog();
        widget->setMinimumSize(800, 500);
        QVBoxLayout* layout = new QVBoxLayout();
        QLineEdit* edit = new QLineEdit();
        QPushButton* button = new QPushButton("Add Example");

        layout->addWidget(edit);
        layout->addWidget(button);

        connect(button, SIGNAL(clicked(bool)), widget, SLOT(close()));

        widget->setLayout(layout);
        widget->exec();

        QString text = edit->text();
        if(!text.isEmpty())
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            childItem->setText(0, text);
            childItem->setFlags(childItem->flags() | Qt::ItemIsUserCheckable);
            childItem->setCheckState(0, Qt::Unchecked);
            item->addChild(childItem);
        }
    }
}

void MainWindow::showList(const QList<QPair<QString,QStringList>>& data, const QString& title, const QString& word)
{
    QWidget* widget = new QWidget();
    widget->setWindowTitle(title);
    widget->setMinimumSize(1500,750);

    QVBoxLayout* layout = new QVBoxLayout;

    QStringList defs;

    QTreeWidget* treeWidget = new QTreeWidget();
    //listWidget->addItems(defs);
    treeWidget->setWindowTitle(title);

    QPair<QString,QStringList> pair;
    foreach (pair, data)
    {
        //defs.push_back(pair.first);
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(treeWidget);
        treeItem->setText(0, pair.first);
        treeItem->setFlags(treeItem->flags() | Qt::ItemIsUserCheckable);
        treeItem->setCheckState(0, Qt::Unchecked);
        foreach (const QString& example, pair.second)
        {
            QTreeWidgetItem *childItem = new QTreeWidgetItem();
            childItem->setText(0, example);
            childItem->setFlags(childItem->flags() | Qt::ItemIsUserCheckable);
            childItem->setCheckState(0, Qt::Unchecked);
            treeItem->addChild(childItem);
        }
    }


    /*QListWidgetItem* item = 0;
    for(int i = 0; i < listWidget->count(); ++i)
    {
        item = listWidget->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }*/

    QPushButton* button = new QPushButton("Add to dictionary");

    WordsListController* controller = new WordsListController(treeWidget, word);

    connect(button, SIGNAL(clicked(bool)), controller, SLOT(clickedWord(bool)));
    connect(widget, SIGNAL(destroyed(QObject*)), controller, SLOT(deleteLater()));
    connect((treeWidget), SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), controller, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));

    layout->addWidget(treeWidget);
    layout->addWidget(button);

    widget->setLayout(layout);

    widget->show();
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
