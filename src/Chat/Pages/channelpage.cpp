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

#include "../../IrcClient/channel.hpp"
#include "../../IrcClient/usermode.hpp"
#include "../chatdock.hpp"
#include "../chattabtree.hpp"
#include "channelpage.hpp"

#include <QDockWidget>
#include <QHashIterator>
#include <QLineEdit>
#include <QTime>

namespace Chat {

ChannelPage::ChannelPage(QTreeWidgetItem* pageTab, QTreeWidget* tabTree) :
    BasicPage(pageTab, tabTree)
{
    userTreeModel = new UserTreeModel(this);
    disabled = true;

    joinAction = new QAction("Join", tabContextMenu);
    connect(joinAction, &QAction::triggered, this, &ChannelPage::joinActionTriggered);
    partAction = new QAction("Part", tabContextMenu);
    connect(partAction, &QAction::triggered, this, &ChannelPage::partActionTriggered);
    QAction* closeAction = new QAction("Close", tabContextMenu);
    connect(closeAction, &QAction::triggered, this, &ChannelPage::closeActionTriggered);
    tabContextMenu->addActions(QList<QAction*>() << joinAction << closeAction);
}

ChannelPage::~ChannelPage()
{
}

void ChannelPage::nicksReady()
{    
    QHashIterator<QString, IrcClient::UserMode*> iterator = ChatDock::ircClient->getChannel(getName()).getUserIterator();
    while (iterator.hasNext()) {
        QString key = iterator.next().key();
        addUser(key);
    }
}

void ChannelPage::topicTextResponse(const QString &topicText)
{
    print(QString("*** Topic: \"%1\"")
          .arg(topicText)
          , Information
    );
}

void ChannelPage::topicInfoResponse(const IrcClient::User &user, const QDateTime &topicDate)
{
    print(QString("*** Topic was set by %1 (%2@%3) at %4")
          .arg(user.nick)
          .arg(user.username)
          .arg(user.hostname)
          .arg(topicDate.toString(Qt::DefaultLocaleShortDate))
          , Information
    );
}

void ChannelPage::channelMessage(const IrcClient::User &user, const QString &message)
{
    print(QString("<%1> %2")
          .arg(user.nick)
          .arg(message)
          , Message
    );
}

void ChannelPage::join(const IrcClient::User &user)
{
    print(QString("*** %1 (%2@%3) has joined the channel")
          .arg(user.nick)
          .arg(user.username)
          .arg(user.hostname)
          , Information
    );

    if (!ChatDock::ircClient->getUs().nick.compare(user.nick, Qt::CaseInsensitive)) {
        enable();
    } else {
        addUser(user.nick);
    }
}

void ChannelPage::part(const IrcClient::User &user)
{
    print(QString("*** %1 (%2@%3) has left the channel")
          .arg(user.nick)
          .arg(user.username)
          .arg(user.hostname)
          , Information
    );
    if (!ChatDock::ircClient->getUs().nick.compare(user.nick, Qt::CaseInsensitive)) {
        disable();
    } else {
        removeUser(user.nick);
    }
}

void ChannelPage::quit(const IrcClient::User &user, const QString &message)
{
    print(QString("*** %1 (%2@%3) has quit (%4)")
          .arg(user.nick)
          .arg(user.username)
          .arg(user.hostname)
          .arg(message)
          , Information
    );
    if (!ChatDock::ircClient->getUs().nick.compare(user.nick, Qt::CaseInsensitive)) {
        disable();
    } else {
        removeUser(user.nick);
    }
}

void ChannelPage::nickChange(const IrcClient::User &user, const QString &newNick)
{
    print(QString("*** %1 is now known as %2")
          .arg(user.nick)
          .arg(newNick)
          , Information
    );
    renameUser(user.nick, newNick);
}

void ChannelPage::action(const IrcClient::User &sender, const QString &action)
{
    print(QString("* %1 %2")
          .arg(sender.nick)
          .arg(action)
          , Action
    );
}

void ChannelPage::notice(const IrcClient::User &sender, const QString &notice)
{
    print(QString("[%1] %2")
          .arg(sender.nick)
          .arg(notice)
          , Notice
    );
}

void ChannelPage::topicChanged(const QString &newTopicText, const IrcClient::User &user, const QDateTime &/*newTopicDate*/)
{
    print(QString("*** %1 changes topic to \"%2\"")
          .arg(user.nick)
          .arg(newTopicText)
          , Information
    );
}

void ChannelPage::kick(const IrcClient::User& sender, const QString& message, const QString& recipient)
{
    print(QString("*** %1 has kicked %2 (%3)")
          .arg(sender.nick)
          .arg(recipient)
          .arg(message)
          , Information
    );
    if (!ChatDock::ircClient->getUs().nick.compare(recipient, Qt::CaseInsensitive)) {
        disable();
    } else {
        removeUser(recipient);
    }
}

void ChannelPage::modeChanged(const IrcClient::User& sender, const QString& mode)
{
    print(QString("*** %1 sets mode %2")
          .arg(sender.nick)
          .arg(mode)
          , Information
    );

    QStringListIterator iterator(mode.split(' ', QString::SkipEmptyParts));
    const QString modes = iterator.next();

    if (modes.contains('v') || modes.contains('o')) {
        const QString recipient = iterator.next();
        const IrcClient::UserMode& mode = ChatDock::ircClient->getChannel(getName()).getUserMode(recipient);
        userTreeModel->removeUser(recipient);
        userTreeModel->addUser(recipient, mode.op, mode.voice);
    }
}

void ChannelPage::addUser(const QString &nick)
{
    const IrcClient::UserMode& mode = ChatDock::ircClient->getChannel(getName()).getUserMode(nick);
    userTreeModel->addUser(nick, mode.op, mode.voice);
}

void ChannelPage::removeUser(const QString &nick)
{
    const IrcClient::UserMode& mode = ChatDock::ircClient->getChannel(getName()).getUserMode(nick);
    userTreeModel->removeUser(nick, mode.op, mode.voice);
}

void ChannelPage::renameUser(const QString &oldNick, const QString &newNick)
{
    const IrcClient::UserMode& mode = ChatDock::ircClient->getChannel(getName()).getUserMode(oldNick);
    userTreeModel->removeUser(oldNick, mode.op, mode.voice);
    userTreeModel->addUser(newNick, mode.op, mode.voice);
}

void ChannelPage::disable()
{
    if (!disabled) {
        delete userTreeModel;
        userTreeModel = 0;
        replaceActions(partAction, joinAction);
    }
    BasicPage::disable();
}

void ChannelPage::enable()
{
    if (disabled) {
        if (!userTreeModel) {
            userTreeModel = new UserTreeModel(this);
        }
        replaceActions(joinAction, partAction);
    }
    BasicPage::enable();
}

} // namespace Chat
