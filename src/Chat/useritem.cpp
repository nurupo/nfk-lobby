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

#include <QStringList>

namespace Chat {

UserItem::UserItem(const QString &data, UserItem *parent)
{
    parentItem = parent;
    itemData = data;
}

UserItem::~UserItem()
{
    qDeleteAll(childItems);
}

UserItem *UserItem::child(int number)
{
    return childItems.value(number);
}

int UserItem::childCount() const
{
    return childItems.size();
}

int UserItem::childNumber() const
{
    if (!parentItem) {
        return 0;
    }

    return parentItem->childItems.indexOf(const_cast<UserItem*>(this));;
}

int UserItem::columnCount() const
{
    return 1;
}

QString UserItem::data() const
{
    return itemData;
}

bool UserItem::insertChildren(int position, int count)
{
    if (position < 0 || position > childItems.size()) {
        return false;
    }

    for (int i = 0; i < count; i ++) {
        UserItem *item = new UserItem(QString(), this);
        childItems.insert(position, item);
    }

    return true;
}

bool UserItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size()) {
        return false;
    }

    for (int i = 0; i < count; i ++) {
        delete childItems.takeAt(position);
    }

    return true;
}

UserItem* UserItem::parent()
{
    return parentItem;
}

void UserItem::setData(const QString &data)
{
    itemData = data;
}

} // namespace Chat
