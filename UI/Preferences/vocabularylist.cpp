#include "vocabularylist.h"

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeView>

#include "treemodelpreferences.h"
#include "../treeitem.h"

VocabularyList::VocabularyList(QWidget *parent) : QWidget(parent)
{
    setupLayout();
}

void VocabularyList::setupLayout()
{
    QVBoxLayout* layout = new QVBoxLayout;

    QVariantMap map;
    map.insert("Pearson", "http://api.pearson.com/v2/dictionaries/ldoce5/");
    map.insert("Oxford", "https://od-api.oxforddictionaries.com:443/api/v1/entries/");

    _view = new QTreeView();
    _model = new TreeModelPreferences(map);
    _view->setModel(_model);

    layout->addWidget(_view);

    QPushButton* addButton = new QPushButton("Add vocabulary");
    QPushButton* deleteButton = new QPushButton("Delete vocabulary");
    QPushButton* editButton = new QPushButton("Edit vocabulary");

    connect(addButton, &QPushButton::clicked, this, &VocabularyList::addVocabulary);
    connect(deleteButton, &QPushButton::clicked, this, &VocabularyList::deleteVocabulary);
    connect(editButton, &QPushButton::clicked, this, &VocabularyList::editVocabulary);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(editButton);

    layout->addLayout(buttonsLayout);

    setLayout(layout);
}

void VocabularyList::addVocabulary()
{
    _model->appendItem("xxx", "some url");
}

void VocabularyList::deleteVocabulary()
{
    QModelIndex index = _view->currentIndex();
    TreeItem* item = reinterpret_cast<TreeItem*>(index.internalPointer());
    _model->removeRowFromModel( item->data(0).toString(), index );

    //_model->removeRow(index.row());
    //emit _model->layoutChanged();
    //_model->dataChanged(index, index);

    /*QModelIndexList list = _view->selectionModel()->selectedRows();
    uint size = list.size();
    if (size>0) {
        for (int i = size - 1; i >= 0; --i)
        {
            //QModelIndex index = proxyModel->mapToSource(list[i]);
            _model->removeRow(list[i].row(), list[i].parent());
         }
    }*/

}

void VocabularyList::editVocabulary()
{

}
