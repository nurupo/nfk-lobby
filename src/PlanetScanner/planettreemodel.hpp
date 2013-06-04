#ifndef PLANETTREEMODEL_HPP
#define PLANETTREEMODEL_HPP

#include <QStandardItemModel>

class PlanetTreeModel : public QStandardItemModel
{
public:
    PlanetTreeModel(QObject* parent);
    Qt::ItemFlags flags(const QModelIndex& index) const;

};

#endif // PLANETTREEMODEL_HPP
