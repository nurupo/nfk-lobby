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

#ifndef CHANNELPAGE_HPP
#define CHANNELPAGE_HPP

#include "../../IrcClient/user.hpp"
#include "../../IrcClient/usermode.hpp"
#include "../usertreemodel.hpp"
#include "basicpage.hpp"

#include <QMainWindow>
#include <QTextEdit>

namespace Chat {

class ChannelPage : public BasicPage
{
    Q_OBJECT
public:
    explicit ChannelPage(QTreeWidgetItem* pageTab, QTreeWidget* tabTree);
    virtual ~ChannelPage();
    UserTreeModel& getUserTreeModel() {return *userTreeModel;}

private:
    UserTreeModel* userTreeModel;
    QAction* joinAction;
    QAction* partAction;
    void addUser(const QString &nick);
    void removeUser(const QString &nick);
    void renameUser(const QString &oldNick, const QString &newNick);

signals:
    void joinActionTriggered();
    void partActionTriggered();
    void closeActionTriggered();

public slots:
    void disable();
    void enable();

    void nicksReady();
    void topicTextResponse(const QString &topicText);
    void topicInfoResponse(const IrcClient::User &user, const QDateTime &topicDate);
    void channelMessage(const IrcClient::User &sender, const QString &message);
    void action(const IrcClient::User &sender, const QString &action);
    void join(const IrcClient::User &user);
    void part(const IrcClient::User &user);
    void nickChange(const IrcClient::User &user, const QString &newNick);
    void notice(const IrcClient::User &sender, const QString &notice);
    void quit(const IrcClient::User &user, const QString &message);
    void topicChanged(const QString &newTopicText, const IrcClient::User &user, const QDateTime &newTopicDate);
    void modeChanged(const IrcClient::User &sender, const QString &mode);
    void kick(const IrcClient::User& sender, const QString& message, const QString& recipient);
};

} // namespace Chat

#endif // CHANNELPAGE_HPP
