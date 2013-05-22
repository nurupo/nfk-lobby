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

#include "preferencessortingmodel.hpp"

PreferencesSortingModel::PreferencesSortingModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
}

bool PreferencesSortingModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    if (left.row() == 0) {
        return sortOrder() == Qt::AscendingOrder;
    } else if (right.row() == 0) {
        return sortOrder() != Qt::AscendingOrder;
    }

    return QSortFilterProxyModel::lessThan(left, right);
}
