#ifndef TREEMODELPREFERENCES_H
#define TREEMODELPREFERENCES_H


#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

//! [0]
class TreeModelPreferences : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModelPreferences(const QVariantMap &data, QObject *parent = 0);
    ~TreeModelPreferences();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    void appendItem(const QString& name, const QString& url);
    void removeRowFromModel(const QString &data , const QModelIndex &index);

private:
    void setupModelData(const QVariantMap &data, TreeItem *parent);

    TreeItem *rootItem;
};

#endif // TREEMODELPREFERENCES_H
