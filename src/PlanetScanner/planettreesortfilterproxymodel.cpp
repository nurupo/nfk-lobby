/*
    Copyright (C) 2013 by Maxim Biro <nurupo.contributions@gmail.com>

    This file is part of NFK Lobby.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the COPYING file for more details.
*/

#include "game.hpp"
#include "planettreesortfilterproxymodel.hpp"
#include "Settings/planetscannersettings.hpp"

namespace PlanetScanner {

PlanetTreeSortFilterProxyModel::PlanetTreeSortFilterProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool PlanetTreeSortFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex column2 = sourceModel()->index(sourceRow, 2, sourceParent);
    if (column2.isValid() && column2.data().toString() != "") {
        const Settings& settings = Settings::getInstance();
        QString gameType = column2.data().toString();
        if (settings.getGameTypeFilter()[gameType]) {
            return false;
        }
        QStringList players = sourceModel()->index(sourceRow, 3, sourceParent).data().toString().split('/');
        QString currentPlayers = players[0];
        QString maxPlayers = players[1];
        if (settings.getHideOnFullFilter() && currentPlayers == maxPlayers) {
            return false;
        }
        if (settings.getHideOnEmptyFilter() && currentPlayers == "0") {
            return false;
        }
    }
    return true;
}

} // namespace PlanetScanner
