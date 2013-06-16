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

#ifndef UserItem_HPP
#define UserItem_HPP

#include <QList>
#include <QVariant>
#include <QVector>

namespace Chat {

class UserItem
{
public:
    explicit UserItem(const QString &data, UserItem *parent = 0);
    ~UserItem();

    UserItem* child(int number);
    int childCount() const;
    int columnCount() const;
    QString data() const;
    bool insertChildren(int position, int count);
    UserItem* parent();
    bool removeChildren(int position, int count);
    int childNumber() const;
    void setData(const QString &data);

private:
    QList<UserItem*> childItems;
    QString itemData;
    UserItem* parentItem;
};

} // namespace Chat

#endif // UserItem_HPP
