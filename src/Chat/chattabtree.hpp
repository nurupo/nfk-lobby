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

#ifndef CHATTABTREE_HPP
#define CHATTABTREE_HPP

#include <QTreeWidget>

namespace Chat {

class TabTree : public QTreeWidget
{
public:
    explicit TabTree(QWidget *parent, int startingSize);
    QSize sizeHint() const;

private:
    int startingSize;

};

} // namespace Chat

#endif // CHATTABTREE_HPP
