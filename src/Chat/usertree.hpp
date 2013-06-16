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

#ifndef USERTREE_HPP
#define USERTREE_HPP

#include <QMenu>
#include <QTreeView>

namespace Chat {

class UserTree : public QTreeView
{
    Q_OBJECT
public:
    explicit UserTree(QWidget* parent, int startingSize);
    QSize sizeHint() const;

private:
    int startingSize;
    QMenu* contextMenu;
    QAction* privateAction;

signals:
    void privateActionTriggered(QString indexText);


public slots:
    void showContextMenu(const QPoint &pos);
};

} // namespace Chat

#endif // USERTREE_HPP
