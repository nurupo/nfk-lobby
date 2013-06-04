#ifndef PLANETTREESORTFILTERPROXYMODEL_HPP
#define PLANETTREESORTFILTERPROXYMODEL_HPP

#include <QSortFilterProxyModel>

class PlanetTreeSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    PlanetTreeSortFilterProxyModel(QObject* parent);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    const bool* getGameTypeFilter() const {return gameTypeFilter;}
    bool getHideOnFullFilter() const {return hideOnFullFilter;}
    bool getHideOnEmptyFilter() const {return hideOnEmptyFilter;}
    void setGameTypeFilter(int gameType, bool value) {gameTypeFilter[gameType] = value;}
    void setHideOnFullFilter(bool value) {hideOnFullFilter = value;}
    void setHideOnEmptyFilter(bool value) {hideOnEmptyFilter = value;}

private:
    bool gameTypeFilter[8];
    bool hideOnFullFilter;
    bool hideOnEmptyFilter;
};

#endif // PLANETTREESORTFILTERPROXYMODEL_HPP
