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

#include "useritem.hpp"
#include "usertreemodel.hpp"

#include <QCoreApplication>
#include <QDebug>
#include <QStringList>

UserTreeModel::UserTreeModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    rootItem = new UserItem("Users");
}

UserTreeModel::~UserTreeModel()
{
    qDebug() << "UserTreeModel deleted";
    delete rootItem;
}

int UserTreeModel::columnCount(const QModelIndex &/*index*/) const
{
    return rootItem->columnCount();
}

QVariant UserTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    UserItem* item = getItem(index);

    return QVariant(item->data());
}

Qt::ItemFlags UserTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    if (index == usersBranch || index == voicedBranch || index == operatorsBranch) {
        return Qt::ItemIsEnabled;
    }

    return Qt::ItemIsSelectable | QAbstractItemModel::flags(index);
}

UserItem *UserTreeModel::getItem(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return rootItem;
    }

    UserItem *item = static_cast<UserItem*>(index.internalPointer());
    if (!item) {
        return rootItem;
    }

    return item;
}

QVariant UserTreeModel::headerData(int /*section*/, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }

    return QVariant(rootItem->data());
}

QModelIndex UserTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0) {
        return QModelIndex();
    }

    UserItem *parentItem = getItem(parent);

    UserItem *childItem = parentItem->child(row);
    if (!childItem) {
        return QModelIndex();
    }

    return createIndex(row, column, childItem);
}

const QModelIndex& UserTreeModel::getUsersBranch()
{
    if (!usersBranch.isValid()) {
        int row = rowCount();
        insertRow(row);
        usersBranch = index(row, 0);
        setData(usersBranch, QVariant("Users"));
        emit branchAdded(usersBranch);
    }
    return usersBranch;
}

const QModelIndex& UserTreeModel::getOperatorsBranch()
{
    if (!operatorsBranch.isValid()) {
        int row = 0;
        insertRow(row);
        operatorsBranch = index(row, 0);
        setData(operatorsBranch, QVariant("Operators"));
        if (usersBranch.isValid()) {
            usersBranch = index(usersBranch.row()+1, 0);
        }
        if (voicedBranch.isValid()) {
            voicedBranch = index(voicedBranch.row()+1, 0);
        }
        emit branchAdded(operatorsBranch);
    }
    return operatorsBranch;
}

const QModelIndex& UserTreeModel::getVoicedBranch()
{
    if (!voicedBranch.isValid()) {
        int row = 0;
        if (operatorsBranch.isValid()) {
            row = 1;
        }
        insertRow(row);
        voicedBranch = index(row, 0);
        setData(voicedBranch, QVariant("Voiced"));
        if (usersBranch.isValid()) {
            usersBranch = index(usersBranch.row()+1, 0);
        }
        emit branchAdded(voicedBranch);
    }
    return voicedBranch;
}

bool UserTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    UserItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows);
    endInsertRows();

    return success;
}

bool UserTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    UserItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

QModelIndex UserTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return QModelIndex();
    }

    UserItem *childItem = getItem(index);
    UserItem *parentItem = childItem->parent();

    if (parentItem == rootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int UserTreeModel::rowCount(const QModelIndex &parent) const
{
    UserItem *parentItem = getItem(parent);

    return parentItem->childCount();
}

bool UserTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::DisplayRole) {
        return false;
    }

    UserItem *item = getItem(index);
    item->setData(QString(value.toString()));

    emit dataChanged(index, index);

    return true;
}

bool UserTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal) {
        return false;
    }

    rootItem->setData(value.toString());

    emit headerDataChanged(orientation, section, section);

    return true;
}

void UserTreeModel::removeUser(const QString &nick, bool op, bool voice)
{
    QModelIndex removedUser = getUser(nick, getRootBranch(op, voice));

    if (!removedUser.isValid()) {
        qDebug() << "error: removeUser -> couldn't find \"" << nick << "\"";
    }
    removeRow(removedUser.row(), removedUser.parent());
    updateBranch(getRootBranch(op, voice));
}

void UserTreeModel::removeUser(const QString &nick)
{
    QModelIndex removedUser = getUser(nick);

    if (!removedUser.isValid()) {
        qDebug() << "error: removeUser -> couldn't find \"" << nick << "\"";
    }
    QModelIndex parent = removedUser.parent();

    removeRow(removedUser.row(), removedUser.parent());
    updateBranch(parent);
}

void UserTreeModel::updateBranch(const QModelIndex &rootBranchIndex)
{
    UserItem* rootBranchItem = getItem(rootBranchIndex);
    if (rootBranchItem->childCount() == 0) {
        removeRow(rootBranchIndex.row(), rootBranchIndex.parent());
        if (rootBranchIndex == operatorsBranch) {
            operatorsBranch = QModelIndex();
            if (usersBranch.isValid()) {
                usersBranch = index(usersBranch.row()-1, 0);
            }
            if (voicedBranch.isValid()) {
                voicedBranch = index(voicedBranch.row()-1, 0);
            }
        } else if (rootBranchIndex == voicedBranch) {
            voicedBranch = QModelIndex();
            if (usersBranch.isValid()) {
                usersBranch = index(usersBranch.row()-1, 0);
            }
        } else {
            usersBranch = QModelIndex();
        }
    } else {
        setData(index(rootBranchIndex.row(), 0), QVariant(QString("%1 (%2)").arg(rootBranchItem->data().split(" ").at(0)).arg(rootBranchItem->childCount())));
    }
}

QModelIndex UserTreeModel::getUser(const QString &nick)
{
    const int size = 3;
    QModelIndex rootBranches[size] = {operatorsBranch, voicedBranch, usersBranch};
    for (int i = 0; i < size; i ++) {
        if (rootBranches[i].isValid()) {
            const QModelIndex index = getUser(nick, rootBranches[i]);
            if (index != QModelIndex()) {
                return index;
            }
        }
    }
    return QModelIndex();
}

QModelIndex UserTreeModel::getUser(const QString &nick, const QModelIndex &rootBranchIndex)
{
    UserItem* rootBranchItem = getItem(rootBranchIndex);
    int min = 0;
    int max = rootBranchItem->childCount()-1;
    while (max >= min) {
        int mid = min + ((max - min) / 2 );
        int comparising = QString::localeAwareCompare(nick, rootBranchItem->child(mid)->data());
        if (comparising == 0) {
            return createIndex(mid, 0, rootBranchItem->child(mid));
        } else if (comparising < 0) {
            max = mid - 1;
        } else if (comparising > 0) {
            min = mid + 1;
        }
    }
    return QModelIndex();
}


QModelIndex UserTreeModel::getRootBranch(bool op, bool voice)
{
    QModelIndex rootBranchIndex;
    if (op) {
        rootBranchIndex = getOperatorsBranch();
    } else if (voice) {
        rootBranchIndex = getVoicedBranch();
    } else {
        rootBranchIndex = getUsersBranch();
    }
    return rootBranchIndex;
}

void UserTreeModel::addUser(const QString &nick, bool op, bool voice)
{
    QModelIndex newUser;
    QModelIndex rootBranchIndex = getRootBranch(op, voice);
    UserItem* rootBranchItem = getItem(rootBranchIndex);

    if (rootBranchItem->childCount() == 0) {
        //case for no users in the branch
        insertRow(0, rootBranchIndex);
        newUser = index(0, 0, rootBranchIndex);
    } else if (rootBranchItem->childCount() == 1) {
        //case for one user in the branch
        if (QString::localeAwareCompare(nick, rootBranchItem->child(0)->data()) < 0) {
            insertRow(0, rootBranchIndex);
            newUser = index(0, 0, rootBranchIndex);
        } else {
            insertRow(1, rootBranchIndex);
            newUser = index(1, 0, rootBranchIndex);
        }
    } else {
        //case for >=2 users in the branch
        //uses binary search
        QModelIndex previousItemIndex;
        int state = 0;
        int min = 0;
        int max = rootBranchItem->childCount()-1;
        while (max > min) {
            int mid = min + ((max - min) / 2 ) + 1;
            int previous = QString::localeAwareCompare(nick, rootBranchItem->child(mid-1)->data());
            int current = QString::localeAwareCompare(nick, rootBranchItem->child(mid)->data());
            if (previous  >= 0 && current <= 0) {
                previousItemIndex = index(mid, 0, createIndex(0, 0, rootBranchItem));
                state = 0;
                break;
            } else if (previous < 0 && current < 0) {
                max = mid - 1;
                state = -1;
            } else if (previous > 0 && current > 0) {
                min = mid;
                state = 1;
            }
        }
        if (state == 0) {
            insertRow(previousItemIndex.row(), rootBranchIndex);
            newUser = index(previousItemIndex.row(), 0, rootBranchIndex);
        } else if (state == 1){
            int row = rootBranchItem->childCount();
            insertRow(row, rootBranchIndex);
            newUser = index(row, 0, rootBranchIndex);
        } else if (state == -1) {
            insertRow(0, rootBranchIndex);
            newUser = index(0, 0, rootBranchIndex);
        }
    }
    setData(newUser, QVariant(nick));
    updateBranch(newUser.parent());
}
