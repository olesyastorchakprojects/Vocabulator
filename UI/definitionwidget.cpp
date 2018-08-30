#include "definitionwidget.h"

#include <QVBoxLayout>
#include <QTreeWidget>
#include <QPushButton>
#include <QComboBox>
#include <QDialog>
#include <QLineEdit>

#include "Database/definitionstable.h"
#include "Database/wordstable.h"
#include "Database/projectstable.h"
#include "Database/examplestable.h"
#include "Database/database.h"

#include "UI/AddDefinition/selectprojectwidget.h"

WordsListController::WordsListController(DefinitionWidget* widget, const QString& word, const QString& url, const QString &webPageTitle)
    : _definitionsWidget(widget), _word(word), _url(url), _webPageTitle(webPageTitle)
{
    setupConnections();
}

void WordsListController::clickedAddWordToVocabulary(bool)
{
    QStringList definitions;

    QTreeWidgetItem* item = 0;
    QTreeWidget* treeWidget = _definitionsWidget->definitionsTreeWidget();
    for(int i = 0; i < treeWidget->topLevelItemCount(); ++i)
    {
        item = treeWidget->topLevelItem(i);

        if(item->checkState(0) == Qt::Checked)
        {
            definitions.push_back(item->text(0));
        }
    }

    if(!definitions.isEmpty())
    {
        SelectProjectWidget widget(_webPageTitle, _url);
        SelectProjectController controller(&widget);
        widget.exec();
        int projectId = controller.selectedProjectId();

        if(projectId != -1)
        {
            int wordId = WordsTable::insertWord(_word, projectId);
            if(wordId != -1)
            {
                ProjectsTable::updateEdited(projectId, Database::currentTime());
            }

            for(int i = 0; i < treeWidget->topLevelItemCount(); ++i)
            {
                item = treeWidget->topLevelItem(i);

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
                                    ExamplesTable::insertExample(child->text(0), definitionId, Example::EXAMPLE_WORD);
                                }
                            }
                        }
                    }
                }
            }
        }

        emit addedToProject();
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
            childItem->setCheckState(0, Qt::Checked);
            item->addChild(childItem);
            item->setCheckState(0, Qt::Checked);
        }
    }
}

void WordsListController::setupConnections()
{
    connect(_definitionsWidget->addDefinitionButton(), SIGNAL(clicked(bool)), this, SLOT(clickedAddWordToVocabulary(bool)));
    connect(_definitionsWidget->definitionsTreeWidget(), SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)), this, SLOT(itemDoubleClicked(QTreeWidgetItem*,int)));
}

/****************************************************/
DefinitionWidget::DefinitionWidget(const QList<QPair<QString, QStringList> > &data, const QString &title, const QString &word)
    : _data(data), _title(title), _word(word)
{
    setupLayout();
}

void DefinitionWidget::setupLayout()
{
    setWindowTitle(_title);
    setMinimumSize(1000,500);

    QVBoxLayout* layout = new QVBoxLayout;

    _definitionsTreeWidget = new QTreeWidget();

    QPair<QString,QStringList> pair;
    foreach (pair, _data)
    {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(_definitionsTreeWidget);
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

    _addDefinitionButton = new QPushButton("Add to dictionary");


    layout->addWidget(_definitionsTreeWidget);
    layout->addWidget(_addDefinitionButton);

    setLayout(layout);
}

QPushButton* DefinitionWidget::addDefinitionButton()
{
    return _addDefinitionButton;
}

QTreeWidget* DefinitionWidget::definitionsTreeWidget()
{
    return _definitionsTreeWidget;
}

