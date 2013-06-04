#include "planettreemodel.hpp"

PlanetTreeModel::PlanetTreeModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

Qt::ItemFlags PlanetTreeModel::flags(const QModelIndex& index) const
{
   return (QStandardItemModel::flags(index) & ~Qt::ItemIsEditable);
}
