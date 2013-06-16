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

#ifndef USERTREEMODEL_HPP
#define USERTREEMODEL_HPP

#include "useritem.hpp"

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

namespace Chat {

class UserTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    UserTreeModel(QObject *parent = 0);
    ~UserTreeModel();

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole);
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole);
    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());
    void removeUser(const QString &nick, bool op, bool voice);
    void removeUser(const QString &nick);
    void addUser(const QString &nick, bool op, bool voice);

private:
    void updateBranch(const QModelIndex &rootBranchIndex);
    void updateBranches();
    QModelIndex getUser(const QString &nick, const QModelIndex &rootBranchIndex);
    QModelIndex getUser(const QString &nick);
    UserItem *getItem(const QModelIndex &index) const;
    const QModelIndex & getUsersBranch();
    const QModelIndex & getOperatorsBranch();
    const QModelIndex & getVoicedBranch();
    QModelIndex getRootBranch(bool op, bool voice);
    UserItem *rootItem;
    QModelIndex usersBranch;
    QModelIndex operatorsBranch;
    QModelIndex voicedBranch;

signals:
    void branchAdded(const QModelIndex &branch);

};

} // namespace Chat

#endif // USERTREEMODEL_HPP
