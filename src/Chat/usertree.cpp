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

#include "usertree.hpp"

namespace Chat {

UserTree::UserTree(QWidget* parent, int startingSize) :
    QTreeView(parent)
{
    this->startingSize = startingSize;
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &UserTree::customContextMenuRequested, this, &UserTree::showContextMenu);
    contextMenu = new QMenu(this);
    privateAction = new QAction("Query", contextMenu);
    contextMenu->addActions(QList<QAction*>() << privateAction);
}

QSize UserTree::sizeHint() const
{
    return QSize(startingSize, startingSize);
}

void UserTree::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = viewport()->mapToGlobal(pos);
    globalPos.setX(globalPos.x() + 1);
    QList<QModelIndex> selected = selectedIndexes();
    if (selected.size() != 1) {
        return;
    }
    QModelIndex index = selected[0];
    if (!index.isValid()) {
        return;
    }
    QAction* selectedAction = contextMenu->exec(globalPos);
    if (selectedAction == privateAction) {
        emit privateActionTriggered(index.data().toString());
    }

}

} // namespace Chat
