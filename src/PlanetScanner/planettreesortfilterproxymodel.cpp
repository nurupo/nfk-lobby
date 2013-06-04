#include "game.hpp"
#include "planettreesortfilterproxymodel.hpp"

PlanetTreeSortFilterProxyModel::PlanetTreeSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool PlanetTreeSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex column2 = sourceModel()->index(sourceRow, 2, sourceParent);
    if (column2.isValid() && column2.data().toString() != "") {
        QString gameType = column2.data().toString();
        if (gameTypeFilter[Game::getGametypeForName(gameType)]) {
            return false;
        }
        QStringList players = sourceModel()->index(sourceRow, 3, sourceParent).data().toString().split('/');
        QString currentPlayers = players[0];
        QString maxPlayers = players[1];
        if (hideOnFullFilter && currentPlayers == maxPlayers) {
            return false;
        }
        if (hideOnEmptyFilter && currentPlayers == "0") {
            return false;
        }
    }
    return true;
}
