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

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "user.hpp"
#include "usermode.hpp"

#include <QDateTime>
#include <QString>

namespace IrcClient {

class Channel : public QObject
{
    Q_OBJECT
public:
    explicit Channel(const QString& channelName);
    ~Channel();
    Channel(const Channel &channel);

    const QDateTime& getTopicDate() const {return topicDate;}
    const QString& getName() const {return name;}
    const QString& getTopicText() const {return topicText;}
    const User& getTopicSetter() const {return topicSetter;}

    bool hasUser(const QString& nick) const {return users.contains(nick);}
    const UserMode& getUserMode(const QString& nick) const {return *users.value(nick);}
    int getUserCount() const {return users.size();}
    QHashIterator<QString, UserMode*> getUserIterator() const {return QHashIterator<QString, UserMode*>(users);}
    void setUserMode(const QString &nick, const UserMode &newUserMode);

    UserMode& getUserMode(const QString& nick) {return *users.value(nick);}

    void addUser(const QString &nick, bool op = false , bool voice = false);

    void removeUser(const QString& nick);
    void renameUser(const QString &oldNick, const QString &newNick);
    void setTopicDate(QDateTime topicDate) {this->topicDate = topicDate;}
    void setTopicSetter(const User& topicSetter) {this->topicSetter = topicSetter;}
    void setTopicText(QString topicText) {this->topicText = topicText;}

private:
    QString name;
    QString topicText;
    User topicSetter;
    QDateTime topicDate;
    QHash<QString, UserMode*> users;

signals:
    void action(const User& sender, const QString& action);
    void channelMessage(const User& sender, const QString& message);
    void join(const User& user);
    void part(const User& user);
    void nickChange(const User& user, const QString& newNick);
    void notice(const User& sender, const QString& notice);
    void quit(const User& user, const QString& message);
    void topicChanged(const QString& topicText, const User& topicSetter, const QDateTime& topicDate);
    void topicTextResponse(const QString& topicText);
    void topicInfoResponse(const User& topicSetter, const QDateTime& topicDate);
    void modeChanged(const User& sender, const QString& mode);
    void nicksReady();
    void kick(const User& sender, const QString& message, const QString& recipient);

};

} // namespace IrcClient

#endif // CHANNEL_HPP
