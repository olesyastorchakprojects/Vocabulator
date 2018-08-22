#include "jsonmodel.h"

#include <QDebug>

#include "../treeitem.h"

JsonModel::JsonModel(const QJsonObject &json, QList<QPair<QString,QStringList>> &definitions, QObject *parent)
    : _definitions(definitions), QAbstractItemModel(parent)
{
    QList<QVariant> rootData;
    rootData << "clumsy";
    _rootItem = new TreeItem(rootData);

    setupModelData(json, _rootItem);
}

JsonModel::~JsonModel()
{
    delete _rootItem;
}

QVariant JsonModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags JsonModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant JsonModel::headerData(int section, Qt::Orientation orientation, int role ) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return _rootItem->data(section);

    return QVariant();
}

QModelIndex JsonModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeItem *parentItem;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex JsonModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == _rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int JsonModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = _rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int JsonModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return _rootItem->columnCount();
}

void handleVariantString(const QVariant& value, TreeItem *parent, QList<QPair<QString,QStringList>> &definitions, const QString& parentKey = "", const QString& key = "" )
{
    if(value.type() != QVariant::String)
        return;

    if(parentKey.contains("definition", Qt::CaseInsensitive))
    {
        qDebug() << "!!!! handleVariantString: " << parentKey << " : "<<  value.toString();
        definitions.push_back(QPair<QString,QStringList>(value.toString(), QStringList()));
    }
    else if(parentKey.contains("example", Qt::CaseInsensitive))
    {
        qDebug() << "!!!! handleVariantString: " << parentKey << " : "<<  value.toString();
        if(!definitions.isEmpty())
            definitions.back().second.push_back(value.toString());
    }
    QString data = (!key.isEmpty()) ? QString("%1: %2").arg(key, value.toString()) : value.toString();

    QVariantList list;
    list.append(data);
    parent->appendChild(new TreeItem(list, parent));
}

void handleVariantDouble(const QVariant& value, TreeItem *parent, const QString& key = "" )
{
    if(value.type() != QVariant::Double)
        return;

    QString data = (!key.isEmpty()) ? QString("%1: %2").arg(key, QString::number(value.toDouble())) : QString::number(value.toDouble());

    QVariantList list;
    list.append(data);
    parent->appendChild(new TreeItem(list, parent));
}

void handleVariantBool(const QVariant& value, TreeItem *parent, const QString& key = "")
{
    if(value.type() != QVariant::Bool)
        return;

    QString data = (!key.isEmpty()) ? QString("%1: %2").arg(key, value.toBool()) : QString::number(value.toBool());

    QVariantList list;
    list.append(data);
    parent->appendChild(new TreeItem(list, parent));
}

void handleVariantMap(const QVariantMap& map, TreeItem *parent , QList<QPair<QString,QStringList>> &definitions, const QString& key = "");

void handleVariantList( const QVariantList& list, TreeItem *parent, QList<QPair<QString,QStringList>> &definitions, const QString& key = "")
{
    TreeItem* newParent = parent;
    if(!key.isEmpty() && !( ( list.size() == 1) && (list.at(0).type() == QVariant::Map)) )
    {
        QVariantList list;
        list.append(key);
        newParent = new TreeItem(list, parent);
        parent->appendChild(newParent);
    }

    for( QVariantList::ConstIterator it = list.cbegin(); it != list.cend(); ++it)
    {
        switch (it->type()) {
        case QVariant::String:
            handleVariantString(*it, newParent, definitions, key);
            break;
        case QVariant::Double:
            handleVariantDouble(*it, newParent);
            break;
        case QVariant::Bool:
            handleVariantBool(*it, newParent);
            break;
        case QVariant::Map:
            handleVariantMap(it->toMap(), newParent, definitions, key);
            break;
        case QVariant::List:
            handleVariantList(it->toList(), newParent, definitions, key);
            break;
        default:
            break;
        }
    }
}

void handleVariantMap( const QVariantMap& map, TreeItem *parent, QList<QPair<QString,QStringList>> &definitions, const QString& key)
{
    TreeItem* newParent = parent;
    if(!key.isEmpty())
    {
        QVariantList list;
        list.append(key);
        newParent = new TreeItem(list, parent);
        parent->appendChild(newParent);
    }

    for( QVariantMap::ConstIterator it = map.cbegin(); it != map.cend(); ++it)
    {
        switch (it.value().type()) {
        case QVariant::String:
            handleVariantString(it.value(), newParent, definitions, key, it.key());
            break;
        case QVariant::Double:
            handleVariantDouble(it.value(), newParent, it.key());
            break;
        case QVariant::Bool:
            handleVariantBool(it.value(), newParent, it.key());
            break;
        case QVariant::Map:
            handleVariantMap(it.value().toMap(), newParent, definitions, key);
            break;
        case QVariant::List:
            handleVariantList(it.value().toList(), newParent, definitions, it.key());
            break;
        default:
            break;
        }
    }
}

void JsonModel::setupModelData(const QJsonObject &json, TreeItem *parent)
{
    handleVariantMap(json.toVariantMap(), parent, _definitions);
}
