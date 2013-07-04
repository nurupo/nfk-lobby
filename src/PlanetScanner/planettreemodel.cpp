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

#include "planettreemodel.hpp"
#include <QDebug>

namespace PlanetScanner {

PlanetTreeModel::PlanetTreeModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

Qt::ItemFlags PlanetTreeModel::flags(const QModelIndex& index) const
{
   return (QStandardItemModel::flags(index) & ~Qt::ItemIsEditable);
}

PlanetTreeModel::ItemType PlanetTreeModel::getIndexType(const QModelIndex& index) const
{
    if (index.data(ItemTypeRole).canConvert<ItemType>()) {
        return index.data(ItemTypeRole).value<ItemType>();
    } else {
        qDebug() << "Can't convert ItemType of" << index.data();
        return ItemType::Planet;
    }
}

PlanetTreeModel::ItemType PlanetTreeModel::getItemType(const QStandardItem* item) const
{
    return getIndexType(item->index());
}

void PlanetTreeModel::setItemType(QStandardItem* item, ItemType type) const
{
    item->setData(QVariant::fromValue(type), ItemTypeRole);
}

} // namespace PlanetScanner
