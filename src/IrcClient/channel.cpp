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

#include "channel.hpp"

namespace IrcClient {

Channel::Channel(const QString& channelName) :
    QObject(0)
{
    name = channelName;
}

Channel::~Channel()
{
    QMutableHashIterator<QString, UserMode*> iterator (users);
    while (iterator.hasNext()) {
        delete iterator.next().value();
    }
}

void Channel::renameUser(const QString &oldNick, const QString &newNick)
{
    if (users.contains(oldNick)) {
        UserMode* renamer = users.take(oldNick);
        users[newNick] = renamer;
    }
}

void Channel::removeUser(const QString& nick)
{
    if (users.contains(nick)) {
        delete users.take(nick);
    }
}

void Channel::addUser(const QString& nick, bool op, bool voice)
{
    UserMode* user = new UserMode(op, voice);
    users[nick] = user;
}

void Channel::setUserMode(const QString& nick, const UserMode &newUserMode)
{
    UserMode* userMode = users[nick];
    userMode->op = newUserMode.op;
    userMode->voice = newUserMode.voice;
}

} // namespace IrcClient
