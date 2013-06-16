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

#include "privatepage.hpp"

namespace Chat {

PrivatePage::PrivatePage(QTreeWidgetItem* pageTab, QTreeWidget* tabTree) :
    BasicPage(pageTab, tabTree)
{
    disabled = false;

    closeAction = new QAction("Close", tabContextMenu);
    connect(closeAction, &QAction::triggered, this, &PrivatePage::closeActionTriggered);
    tabContextMenu->addActions(QList<QAction*>() << closeAction);
}

PrivatePage::~PrivatePage()
{
}

void PrivatePage::privateMessage(const IrcClient::User &user, const QString &message)
{
    print(QString("<%1> %2")
          .arg(user.nick)
          .arg(message)
          , Message
    );
}

void PrivatePage::action(const IrcClient::User &sender, const QString &action)
{
    print(QString("* %1 %2")
          .arg(sender.nick)
          .arg(action)
          , Action
    );
}

} // namespace Chat
