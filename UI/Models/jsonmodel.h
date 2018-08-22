#ifndef JSONMODEL_H
#define JSONMODEL_H

#include <QAbstractItemModel>
#include <QJsonObject>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

class JsonModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit JsonModel(const QJsonObject &json, QList<QPair<QString,QStringList>>& definitions, QObject *parent = 0);
    ~JsonModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

private:
    void setupModelData(const QJsonObject &json, TreeItem *parent);

    TreeItem *_rootItem;
    QList<QPair<QString,QStringList>>& _definitions;
};

#endif // JSONMODEL_H
