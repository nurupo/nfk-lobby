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

#include "../chatdock.hpp"
#include "serverpage.hpp"

namespace Chat {

ServerPage::ServerPage(QTreeWidgetItem* pageTab, QTreeWidget* tabTree) :
    BasicPage(pageTab, tabTree)
{
    connectAction = new QAction("Connect", tabContextMenu);
    connect(connectAction, &QAction::triggered, this, &ServerPage::connectActionTriggered);
    tabContextMenu->addActions(QList<QAction*>() << connectAction);
    disconnectAction = new QAction("Disconnect", tabContextMenu);
    connect(disconnectAction, &QAction::triggered, this, &ServerPage::disconnectActionTriggered);
}

ServerPage::~ServerPage()
{
}

void ServerPage::serverResponse(const QString &response)
{
    print(QString("* %1")
          .arg(response)
          , ServerResponse
    );
}

void ServerPage::notice(const IrcClient::User &sender, const QString &notice)
{
    print(QString("[%1] %2")
          .arg(sender.nick)
          .arg(notice)
          , Notice
    );
}

void ServerPage::ctcpRequest(const QString &target, const IrcClient::User &sender, const QString &request)
{
    if (sender.nick == Dock::ircClient->getUs().nick) {
        print(QString("Sent CTCP-%1 request to %2")
              .arg(request)
              .arg(target)
              , Ctcp
        );
    } else {
        print(QString("Recieved CTCP-%1 request from %2 (%3@%4)")
              .arg(request)
              .arg(sender.nick)
              .arg(sender.username)
              .arg(sender.hostname)
              , Ctcp
        );
    }
}

void ServerPage::ctcpReply(const QString &target, const IrcClient::User &sender, const QString &request, const QString &message)
{
    if (sender.nick == Dock::ircClient->getUs().nick) {
        print(QString("Sent CTCP-%1 reply to %2: %3")
              .arg(request)
              .arg(target)
              .arg(message)
              , Ctcp
        );
    } else {
        print(QString("Recieved CTCP-%1 reply from %2 (%3@%4): %5")
              .arg(request)
              .arg(sender.nick)
              .arg(sender.username)
              .arg(sender.hostname)
              .arg(message)
              , Ctcp
        );
    }
}

void ServerPage::userModeChanged(const QString &target, const IrcClient::User &sender, const QString &mode)
{
    print(QString("*** %1 sets mode %2 to %3")
          .arg(sender.nick)
          .arg(mode)
          .arg(target)
          , Information
    );
}

void ServerPage::connecting(const QString &address, int port)
{
    print(QString("* Connecting to %1:%2...")
          .arg(address)
          .arg(port)
          , ServerResponse
    );
    BasicPage::enable();
    replaceActions(connectAction, disconnectAction);
}

void ServerPage::quit(const QString &message)
{
    const IrcClient::User& us = Dock::ircClient->getUs();
    print(QString("*** %1 (%2@%3) has quit (%4)")
          .arg(us.nick)
          .arg(us.username)
          .arg(us.hostname)
          .arg(message)
          , Information
    );
}

void ServerPage::disconnected()
{
    print(QString("* Disconnected"), ServerResponse);
    BasicPage::disable();
    replaceActions(disconnectAction, connectAction);
}

} // namespace Chat
