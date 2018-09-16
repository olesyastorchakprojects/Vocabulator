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
#include <QLineEdit>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>

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
#include <QTimer>

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
    : QMainWindow(parent), _oxfordNetworkManager(NULL), _pearsonNetworkManager(NULL)
{
    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    jQuery.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
    file.close();

    QFile file2;
    file2.setFileName(":/mark.min.js");
    file2.open(QIODevice::ReadOnly);
    jMark = file2.readAll();
    //jMark.append("\nmark{background: red;};");
    file2.close();

    _jsTimer = new QTimer();
    connect(_jsTimer, SIGNAL(timeout()), this, SLOT(jsTimeout()));

    QRect screen = Config::screenGeometry();

    setMinimumSize(screen.width() * 0.75, screen.height() * 0.85 );

    setCentralWidget(new QWidget);

    QVBoxLayout* layout = new QVBoxLayout;

    _textEditWords = new QTextEdit();
    //_textEditWords->setStyleSheet("font-size:24px;");
    _textEditWords->setMaximumHeight(height()*0.1);
    _textEditWords->setReadOnly(false);

    _definition = new QTextEdit();
    _definition->setReadOnly(true);
    _definition->setMaximumHeight(height()*0.03);

    _view = new QWebEngineView();
    _url = "https://www.nytimes.com/2018/09/07/technology/monopoly-antitrust-lina-khan-amazon.html";
    //_url = "https://www.facebook.com/rebecca.solnit/posts/10156368013805552";
    _view->load(QUrl(_url));
    connect(_view, &QWebEngineView::loadFinished, this, &MainWindow::loadFinished);
    connect(_view, &QWebEngineView::selectionChanged, this, &MainWindow::selectionChanged);

    layout->addWidget(_view);
    layout->addWidget(_definition);


    QGridLayout *grid = new QGridLayout;
    grid->addWidget(createAddWordGroup(), 0, 0);
    grid->addWidget(createAddPhraseGroup(), 0, 1);
    setLayout(grid);

    layout->addLayout(grid);
    centralWidget()->setLayout(layout);

    createActions();
    createMenus();
}

void MainWindow::selectionChanged()
{
    QString text = _view->selectedText();
    qDebug() << "Selected text: " << text;

    if(text.isEmpty())
        return;

    QList<Word> words = _project.words();
    foreach (const Word& w, words)
    {
        if((text.trimmed().startsWith(w.value().trimmed(), Qt::CaseInsensitive) ) && w.definitions().size())
        {
            _definition->setText(w.definitions().at(0).value());
            return;
        }
    }

    foreach (const Project& p, _projects)
    {
        QList<Word> words = p.words();
        foreach (const Word& w, words)
        {
            if((text.trimmed().startsWith(w.value().trimmed(), Qt::CaseInsensitive) ) && w.definitions().size())
            {
                _definition->setText(w.definitions().at(0).value());
                return;
            }
        }
    }
}

void MainWindow::jsTimeout()
{
    _jsTimer->stop();

    highlightWords(true);
    highlightPhrases();
}

void MainWindow::loadFinished(bool)
{
    insertStyleSheet("test", ".red{background: #AFCDE8;}", true);
    insertStyleSheet("test", ".blue{background: #FDAD82;}", true);
    insertStyleSheet("test", ".green{background: #C8FFC8;}", true);
    _view->page()->runJavaScript(jQuery,[this](const QVariant &v) { qDebug()<<v.toString();});
    _view->page()->runJavaScript(jMark,[this](const QVariant &v) { qDebug()<<v.toString();});

    _jsTimer->start(100);
}

void MainWindow::insertStyleSheet(const QString &name, const QString &source, bool immediately)
{
    QWebEngineScript script;
    QString s = QString::fromLatin1("(function() {"\
                                    "    css = document.createElement('style');"\
                                    "    css.type = 'text/css';"\
                                    "    css.id = '%1';"\
                                    "    document.head.appendChild(css);"\
                                    "    css.innerText = '%2';"\
                                    "})()").arg(name).arg(source.simplified());
    if (immediately)
        _view->page()->runJavaScript(s);

    script.setName(name);
    script.setSourceCode(s);
    script.setInjectionPoint(QWebEngineScript::DocumentReady);
    script.setRunsOnSubFrames(true);
    script.setWorldId(QWebEngineScript::ApplicationWorld);
    _view->page()->scripts().insert(script);
}

void MainWindow::highlightWords(bool lookAll)
{
    _projects = ProjectsTable::projects();
    foreach (const Project& p, _projects)
    {
        QString url = _view->page()->url().toString();
        QString style = "green";
        QString accuracy = "exactly";
        if(url == p.url())
        {
            _project = p;
            style = "blue";
            accuracy = "complementary";
        }
        else if(!lookAll)
            continue;
        QList<Word> words = p.words();
        QStringList wordsList;
        QStringList wordsList2;
        foreach (const Word& w, words)
        {
            if(w.value().contains(" "))
                wordsList2 << QString("'%1'").arg(w.value());
            else
                wordsList << QString("'%1'").arg(w.value());
        }

        if(!wordsList.isEmpty())
        {
            QString param = (wordsList.size() == 1) ? wordsList.at(0) : wordsList.join(',');
            _view->page()->runJavaScript(QString("var instance = new Mark(document.querySelector('body'));instance.mark([%1], "
            "{accuracy: {value: '%2',limiters: ['.', ',', ' ', '-']}, 'className':'%3'});").arg(param, accuracy, style));
        }
        if(!wordsList2.isEmpty())
        {
            QString param = (wordsList2.size() == 1) ? wordsList2.at(0) : wordsList2.join(',');
            _view->page()->runJavaScript(QString("var instance = new Mark(document.querySelector('body'));instance.mark([%1], "
            "{accuracy: {value: '%2',limiters: ['.', ',', ' ', '-']}, 'separateWordSearch':false, 'className':'%3'});").arg(param, accuracy, style));
        }
    }
}

void MainWindow::highlightPhrases()
{
    QList<Project> projects = ProjectsTable::projects();
    foreach (const Project& p, projects)
    {
        QString url = _view->page()->url().toString();
        if(p.url() == _view->page()->url().toString())
        {
        _project = p;
        QList<Phrase> phrases = p.phrases();
        QStringList phrasesList;
        foreach (const Phrase& ph, phrases)
        {
                phrasesList << QString("'%1'").arg(ph.value());
        }

        if(!phrasesList.isEmpty())
        {
            QString param = phrasesList.join(',');
            _view->page()->runJavaScript(QString("var instance = new Mark(document.querySelector('body'));instance.mark([%1], "
            "{accuracy: {value: 'exactly',limiters: ['.', ',', ' ', '\“', '\”']}, 'separateWordSearch':false, 'className':'red'});").arg(param));
        }
            break;
        }
    }
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

void MainWindow::textChangedAddPhraseExample()
{
    QString str = _textEditPhraseExample->toPlainText();
    if(str.contains("Carreyrou, John. Bad Blood", Qt::CaseInsensitive))
    {
        int index = str.indexOf("Carreyrou, John. Bad Blood", Qt::CaseInsensitive);
        str = str.left(index);
        str = str.trimmed();

        _textEditPhraseExample->clear();
        _textEditPhraseExample->append(str);
    }
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

    connect(_textEditPhraseExample, SIGNAL(textChanged()), this, SLOT(textChangedAddPhraseExample()));

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
    fileMenu->addAction(openUrlAction);
    fileMenu->addAction(exportAction);
    fileMenu->addAction(openProjectAction);

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

    openUrlAction = new QAction(tr("&Open url"), this);
    connect(openUrlAction, &QAction::triggered, this, &MainWindow::openUrl);

    openProjectAction = new QAction(tr("&Open project"), this);
    connect(openProjectAction, &QAction::triggered, this, &MainWindow::openProject);
}

void MainWindow::openProject()
{
    QList<Project> projects = ProjectsTable::projects();
    QMap<QString, QString> data;
    foreach (const Project& p, projects)
    {
        data.insert(p.value(), p.url());
    }

    QDialog* dialog = new QDialog();
    QRect screen = Config::screenGeometry();
    dialog->setMinimumSize(screen.width() * 0.4, screen.height() * 0.1 );
    QHBoxLayout* layout = new QHBoxLayout;

    QComboBox* comboBox = new QComboBox;
    comboBox->setMinimumWidth(dialog->width()*0.9);
    comboBox->addItem(QString("None"));
    QMapIterator<QString,QString> it(data);
    while(it.hasNext())
    {
        comboBox->addItem(it.next().key());
    }

    QPushButton* button = new QPushButton("Open");

    layout->addWidget(comboBox);
    layout->addWidget(button);

    connect(button, SIGNAL(clicked(bool)), dialog, SLOT(close()));

    dialog->setLayout(layout);
    dialog->exec();

    QString selectedProject = comboBox->currentText();
    QString url = data.contains(selectedProject) ? data[selectedProject] : QString();

    if(!url.isEmpty())
    {
        _view->load(QUrl(url));
        _url = url;
    }

    highlightWords(true);
    highlightPhrases();
}

void MainWindow::openUrl()
{
    QDialog* widget = new QDialog(this);

    QRect screen = Config::screenGeometry();

    widget->setMinimumSize(screen.width() * 0.4, screen.height() * 0.1 );


    QHBoxLayout* layout = new QHBoxLayout;

    QLabel* label = new QLabel("url:");
    QLineEdit* lineEdit = new QLineEdit();
    lineEdit->setMinimumWidth(widget->width()*0.7);
    QPushButton* button = new QPushButton("Go");

    layout->addWidget(label);
    layout->addWidget(lineEdit);
    layout->addWidget(button);

    connect(button, SIGNAL(clicked(bool)), widget, SLOT(close()));

    widget->setLayout(layout);
    widget->exec();

    QString url = lineEdit->text();
    if(!url.isEmpty())
    {
        _url = url;
        _view->load(QUrl(_url));
    }
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
    highlightPhrases();
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

QJsonObject getEntryOxford( const QString& entry, QNetworkAccessManager* manager )
{
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

QJsonObject getEntryPearson( const QString& entry,  QNetworkAccessManager* manager )
{
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
    connect(controller, SIGNAL(addedToProject()), this, SLOT(highlightWords()));
    connect(controller, SIGNAL(addedToProject()), this, SLOT(highlightPhrases()));
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
        if(!_oxfordNetworkManager)
        {
            _oxfordNetworkManager = new QNetworkAccessManager();
        }

        QList<QPair<QString,QStringList>> definitions;
        JsonModel* model = new JsonModel(getEntryOxford(entry, _oxfordNetworkManager), definitions );

        QTreeView * view = new QTreeView();
        view->setModel(model);
        view->setWindowTitle(QObject::tr("Oxford vocabulary"));
        view->setMinimumSize(screen.width() * 0.3, screen.height() * 0.7);
        view->expandAll();
        //view->show();

        showList(definitions, "Oxford vocabulary", entry);
    }

    {
        if(!_pearsonNetworkManager)
        {
            _pearsonNetworkManager = new QNetworkAccessManager();
        }
        QList<QPair<QString,QStringList>> definitions;
        JsonModel* model = new JsonModel(getEntryPearson(entry, _pearsonNetworkManager), definitions);

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
