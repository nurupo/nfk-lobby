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

#ifndef PLANETTREEMODEL_HPP
#define PLANETTREEMODEL_HPP

#include <QStandardItemModel>

namespace PlanetScanner {

class PlanetTreeModel : public QStandardItemModel
{
public:
    PlanetTreeModel(QObject* parent);
    Qt::ItemFlags flags(const QModelIndex& index) const;

    enum {ItemTypeRole = Qt::UserRole, PlayerIdRole};
    enum class ItemType {Planet = 0, Game, Player};

    ItemType getIndexType(const QModelIndex& index) const;
    ItemType getItemType(const QStandardItem* item) const;
    void setItemType(QStandardItem* item, ItemType type) const;

};

} // namespace PlanetScanner

Q_DECLARE_METATYPE(PlanetScanner::PlanetTreeModel::ItemType)

#endif // PLANETTREEMODEL_HPP
