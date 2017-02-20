#include "treemodelpreferences.h"

#include "../treeitem.h"
#include "treemodelpreferences.h"

#include <QDebug>
#include <QStringList>
#include <QVariantMap>

//! [0]
TreeModelPreferences::TreeModelPreferences(const QVariantMap &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "vocabularies";
    rootItem = new TreeItem(rootData);
    setupModelData(data, rootItem);
}
//! [0]

//! [1]
TreeModelPreferences::~TreeModelPreferences()
{
    delete rootItem;
}
//! [1]

//! [2]
int TreeModelPreferences::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}
//! [2]

//! [3]
QVariant TreeModelPreferences::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}
//! [3]

//! [4]
Qt::ItemFlags TreeModelPreferences::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}
//! [4]

//! [5]
QVariant TreeModelPreferences::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
//! [5]

//! [6]
QModelIndex TreeModelPreferences::index(int row, int column, const QModelIndex &parent)
            const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}
//! [6]

//! [7]
QModelIndex TreeModelPreferences::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}
//! [7]

//! [8]
int TreeModelPreferences::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}
//! [8]

void TreeModelPreferences::setupModelData(const QVariantMap &data, TreeItem *parent)
{
    for(QVariantMap::const_iterator it = data.cbegin(); it != data.cend(); ++it)
    {
        QVariantList list;
        list.append(it.key());
        TreeItem* urlChild = new TreeItem(list, parent);
        parent->appendChild(urlChild);
        qDebug() << "key: " << it.key() << ", value: " << it.value();

        list.clear();
        list.append(it.value());
        urlChild->appendChild(new TreeItem(list, urlChild));
    }
}

void TreeModelPreferences::appendItem(const QString& name, const QString& url)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());

    QVariantList list;
    list.append(name);
    TreeItem* nameChild = new TreeItem(list, rootItem);
    rootItem->appendChild(nameChild);

    list.clear();
    list.append(url);
    nameChild->appendChild(new TreeItem(list, nameChild));

    endInsertRows();
}

void TreeModelPreferences::removeRowFromModel(const QString& data, const QModelIndex& index)
{
    QModelIndex index2Remove = (index.parent() == QModelIndex()) ? index : index.parent();
    beginRemoveRows(QModelIndex(), index2Remove.row(), index2Remove.row());

    int childcount = rootItem->childCount();
    int item2Remove = -1;
    for(int i = 0; i < childcount; ++i)
    {
        if((rootItem->child(i)->data(0).toString() == data) || (rootItem->child(i)->child(0)->data(0).toString() == data ))
        {
            item2Remove = i;
            break;
        }
    }
    if(item2Remove != -1)
        rootItem->removeChild(item2Remove);


    endRemoveRows();

    //emit layoutChanged();
}

