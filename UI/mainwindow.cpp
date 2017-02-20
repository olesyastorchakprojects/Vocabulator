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

#include "treemodel.h"
#include "Preferences/preferences.h"

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

void printString(const QJsonValue& value, int identation, const QString& key = "" )
{
    if(value.type() != QJsonValue::String)
        return;

    QString idents;
    for(int i = 0; i < identation * 4; ++i)
        idents += " ";
    qDebug() << idents << key << value.toString();
}

void printDouble(const QJsonValue& value, int identation )
{
    if(value.type() != QJsonValue::Double)
        return;

    QString idents;
    for(int i = 0; i < identation * 4; ++i)
        idents += " ";
    qDebug() << idents<< value.toDouble();
}

void printBool(const QJsonValue& value, int identation )
{
    if(value.type() != QJsonValue::Bool)
        return;
    QString idents;
    for(int i = 0; i < identation * 4; ++i)
        idents += " ";
    qDebug() << idents<< value.toBool();
}

void printArray( const QJsonArray& array, int identation );

void printObject(const QJsonValue& value, int identation )
{
    if(value.type() != QJsonValue::Object)
        return;

    QJsonObject obj = value.toObject();
    for(QJsonObject::iterator it = obj.begin(); it != obj.end(); ++it)
    {
        switch(it.value().type())
        {
        case QJsonValue::String:
            printString(it.value(), identation, it.key());
            break;
        case QJsonValue::Double:
            printDouble(it.value(), identation);
            break;
        case QJsonValue::Bool:
            printBool(it.value(), identation);
            break;
        case QJsonValue::Array:
            printArray(it.value().toArray(), identation++);
            break;
        case QJsonValue::Object:
            printObject(it.value(), identation++);
            break;
        default:
            break;
        }
    }
}

void printArray( const QJsonArray& array, int identation )
{
    for(int i = 0; i < array.count(); ++i)
    {
        switch(array[i].type())
        {
        case QJsonValue::String:
            printString(array[i], identation);
            break;
        case QJsonValue::Double:
            printDouble(array[i], identation);
            break;
        case QJsonValue::Bool:
            printBool(array[i], identation);
            break;
        case QJsonValue::Array:
            printArray(array[i].toArray(), identation++);
            break;
        case QJsonValue::Object:
            printObject(array[i], identation++);
            break;
        default:
            break;
        }
    }
}

void printVariantString(const QVariant& value, int identation, const QString& key = "" )
{
    if(value.type() != QVariant::String)
        return;

    QString idents;
    for(int i = 0; i < identation * 4; ++i)
        idents += " ";
    if(key.isEmpty())
        qDebug() << idents << value.toString();
    else
        qDebug() << idents << key << value.toString();
}

void printVariantDouble(const QVariant& value, int identation, const QString& key = "" )
{
    if(value.type() != QVariant::Double)
        return;

    QString idents;
    for(int i = 0; i < identation * 4; ++i)
        idents += " ";
    if(key.isEmpty())
        qDebug() << idents << value.toDouble();
    else
        qDebug() << idents << key << value.toDouble();
}

void printVariantBool(const QVariant& value, int identation, const QString& key = "")
{
    if(value.type() != QVariant::Bool)
        return;
    QString idents;
    for(int i = 0; i < identation * 4; ++i)
        idents += " ";
    if(key.isEmpty())
        qDebug() << idents << value.toBool();
    else
        qDebug() << idents << key << value.toBool();
}

void printVariantMap( const QVariantMap& map, int identation, const QString& key = "" );

void printVariantList( const QVariantList& list, int identation, const QString& key = "")
{
    if(!key.isEmpty() && !( ( list.size() == 1) && (list.at(0).type() == QVariant::Map)) )
    {
        QString idents;
        for(int i = 0; i < identation * 4; ++i)
            idents += " ";
        qDebug() << idents << key;
    }

    for( QVariantList::ConstIterator it = list.cbegin(); it != list.cend(); ++it)
    {
        switch (it->type()) {
        case QVariant::String:
            printVariantString(*it, identation + 1);
            break;
        case QVariant::Double:
            printVariantDouble(*it, identation + 1);
            break;
        case QVariant::Bool:
            printVariantBool(*it, identation + 1);
            break;
        case QVariant::Map:
            printVariantMap(it->toMap(), identation + 1, key);
            break;
        case QVariant::List:
            printVariantList(it->toList(), identation + 1, key);
            break;
        default:
            break;
        }
    }
}

void printVariantMap( const QVariantMap& map, int identation, const QString& key)
{
    if(!key.isEmpty())
    {
        QString idents;
        for(int i = 0; i < identation * 4; ++i)
            idents += " ";
        qDebug() << idents << key;
    }

    for( QVariantMap::ConstIterator it = map.cbegin(); it != map.cend(); ++it)
    {
        switch (it.value().type()) {
        case QVariant::String:
            printVariantString(it.value(), identation + 1, it.key());
            break;
        case QVariant::Double:
            printVariantDouble(it.value(), identation + 1, it.key());
            break;
        case QVariant::Bool:
            printVariantBool(it.value(), identation + 1, it.key());
            break;
        case QVariant::Map:
            printVariantMap(it.value().toMap(), identation + 1, key);
            break;
        case QVariant::List:
            printVariantList(it.value().toList(), identation + 1, it.key());
            break;
        default:
            break;
        }
    }
}

QVariantMap xmlStreamToVariant(QXmlStreamReader &xml, const QString &prefix = QLatin1String("."), const int maxDepth = 1024)
{
    if (maxDepth < 0) {
        qWarning() << QObject::tr("max depth exceeded");
        return QVariantMap();
    }

    if (xml.hasError()) {
        qWarning() << xml.errorString();
        return QVariantMap();
    }

    if (xml.tokenType() == QXmlStreamReader::NoToken)
        xml.readNext();

    if ((xml.tokenType() != QXmlStreamReader::StartDocument) &&
        (xml.tokenType() != QXmlStreamReader::StartElement)) {
        qWarning() << QObject::tr("unexpected XML tokenType %1 (%2)")
                      .arg(xml.tokenString()).arg(xml.tokenType());
        return QVariantMap();
    }

    QVariantMap map;
    if (xml.tokenType() == QXmlStreamReader::StartDocument) {
        map.insert(prefix + QLatin1String("DocumentEncoding"), xml.documentEncoding().toString());
        map.insert(prefix + QLatin1String("DocumentVersion"), xml.documentVersion().toString());
        map.insert(prefix + QLatin1String("StandaloneDocument"), xml.isStandaloneDocument());
    } else {
        if (!xml.namespaceUri().isEmpty())
            map.insert(prefix + QLatin1String("NamespaceUri"), xml.namespaceUri().toString());
        foreach (const QXmlStreamAttribute &attribute, xml.attributes()) {
            QVariantMap attributeMap;
            attributeMap.insert(QLatin1String("Value"), attribute.value().toString());
            if (!attribute.namespaceUri().isEmpty())
                attributeMap.insert(QLatin1String("NamespaceUri"), attribute.namespaceUri().toString());
            if (!attribute.prefix().isEmpty())
                attributeMap.insert(QLatin1String("Prefix"), attribute.prefix().toString());
            attributeMap.insert(QLatin1String("QualifiedName"), attribute.qualifiedName().toString());
            map.insertMulti(prefix + attribute.name().toString(), attributeMap);
        }
    }

    for (xml.readNext(); (!xml.atEnd()) && (xml.tokenType() != QXmlStreamReader::EndElement)
          && (xml.tokenType() != QXmlStreamReader::EndDocument); xml.readNext()) {
        switch (xml.tokenType()) {
        case QXmlStreamReader::Characters:
        case QXmlStreamReader::Comment:
        case QXmlStreamReader::DTD:
        case QXmlStreamReader::EntityReference:
            map.insertMulti(prefix + xml.tokenString(), xml.text().toString());
            break;
        case QXmlStreamReader::ProcessingInstruction:
            map.insertMulti(prefix + xml.processingInstructionTarget().toString(),
                            xml.processingInstructionData().toString());
            break;
        case QXmlStreamReader::StartElement:
            map.insertMulti(xml.name().toString(), xmlStreamToVariant(xml, prefix, maxDepth-1));
            break;
        default:
            qWarning() << QObject::tr("unexpected XML tokenType %1 (%2)")
                          .arg(xml.tokenString()).arg(xml.tokenType());
        }
    }
    return map;
}

void MainWindow::TOMOVE_getWordFromServer() // 66acad7f-e386-42dd-9033-d68e57af27a9
{
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    QEventLoop loop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()));

    QString app_id = "6ae681d2";
    QString app_key = "0f403d7468babf21b4d13c3f55bb496a";

    QString language = "en";
    QString word_id = "clumsy";

    QString url = "https://od-api.oxforddictionaries.com:443/api/v1/entries/" + language + "/" + word_id.toLower();

    //r = requests.get(url, headers = {'app_id': app_id, 'app_key': app_key})
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("app_id", app_id.toUtf8());
    request.setRawHeader("app_key", app_key.toUtf8());


    QString path1;
    QString path2 = "http://www.dictionaryapi.com/api/v1/references/thesaurus/xml/clumsy?key=66acad7f-e386-42dd-9033-d68e57af27a9";
    //QNetworkReply* reply = manager->get(QNetworkRequest(QUrl("http://api.pearson.com/v2/dictionaries/ldoce5/entries?search=clumsy")));
    QNetworkReply* reply = manager->get(request);
    loop.exec();

    QStringList propertyNames;
    QStringList propertyKeys;
    QString strReply = (QString)reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
    QJsonObject jsonObject = jsonResponse.object();
    QJsonArray jsonArray = jsonObject["results"].toArray();

    qDebug() << "size: " << jsonArray.size();
    //printArray(jsonArray, 0);
    //printObject(jsonObject, 0);
    QVariantMap map = jsonObject.toVariantMap();
    printVariantMap(map, 0);

    TreeModel* model = new TreeModel(map);

    QTreeView * view = new QTreeView();
    view->setModel(model);
    view->setWindowTitle(QObject::tr("Search result for clumsy"));
    view->setMinimumSize(2000, 1600);
    view->show();


    //qDebug() << strReply;
    /*foreach (const QJsonValue & value, jsonArray)
    {
        QJsonObject obj = value.toObject();
        qDebug() << obj.keys();
        QStringList keys = obj.keys();
        qDebug() << "size: " << obj.count();
        for(QJsonObject::iterator it = obj.begin(); it != obj.end(); ++it)
        {
            qDebug() << "    " << it.key() << ", type = " << it.value().type();
            if(it.value().type() == QJsonValue::String)
            {
                qDebug() << "        " << it.value().toString();
            }
            else if(it.value().type() == QJsonValue::Array)
            {
                QJsonArray obj1 = it.value().toArray();
                //qDebug() << "    size1: " << obj1.count();
                for(int i = 0; i < obj1.count(); ++i)
                {
                    //qDebug() << "        type:" << obj1[i].type();
                    if(obj1[i].type() == QJsonValue::String)
                    {
                        qDebug() << "            " << obj1[i].toString() << ", type = " << obj1[i].type();
                    }
                    else
                        qDebug() << "            " << obj1[i].type();
                }
                for(QJsonArray::iterator it1 = obj1.begin(); it1 != obj1.end(); ++it1)
                {
                    qDebug() << "            " << it1->operator *() << ", type = " << it1.value().type();
                    if(it1.value().type() == QJsonValue::String)
                    {
                        qDebug() << "            " << it1.value().toString();
                    }
                }
            }
        }*/

        /*foreach(QString str, keys)
        {
            QJsonArray obj2 = obj[str].toArray();
            foreach (const QJsonValue & value2, obj2)
            {
                qDebug() << "    " << str << ": " << value2.toObject().keys();
                //qDebug() << "    " << value2.toObject()["definition"].toString();
                //qDebug() << "    " << value2.toObject()["definition"];

                QJsonArray obj3 = value2.toObject()["definition"].toArray();
                foreach (const QJsonValue & value3, obj3)
                {
                    qDebug() << "        " << value3.toString();
                }
            }
        }*/

        //qDebug() << obj["senses"].toString();
        //propertyNames.append(obj["PropertyName"].toString());
        //propertyKeys.append(obj["key"].toString());
   // }
}
