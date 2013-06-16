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

#include "chatsettings.hpp"
#include "chatsettingsdialog.hpp"

#include <QDateTime>
#include <QSettings>

namespace Chat {

Settings::Settings() :
    loaded(false)
{
}

Settings& Settings::getInstance()
{
    static Settings settings;
    return settings;
}

int Settings::getServerId() const
{
    return serverId;
}

void Settings::setServerId(int value)
{
    serverId = value;
}

bool Settings::getAutoConnect() const
{
    return autoConnect;
}

void Settings::setAutoConnect(bool value)
{
    autoConnect = value;
}

const QString& Settings::getNick() const
{
    return nick;
}

void Settings::setNick(const QString& newNick)
{
    nick = newNick;
}


const QString& Settings::getUsername() const
{
    return username;
}

void Settings::setUsername(const QString newUsername)
{
    username = newUsername;
}

const QString& Settings::getQuitMessage() const
{
    return quitMessage;
}

void Settings::setQuitMessage(const QString& newQuitMessage)
{
    quitMessage = newQuitMessage;
}

bool Settings::getAutoIdentify() const
{
    return autoIdentify;
}

void Settings::setAutoIdentify(bool value)
{
    autoIdentify = value;
}

const QString& Settings::getPassword() const
{
    return password;
}

void Settings::setPassword(const QString& newPassword)
{
    password = newPassword;
}

const QList<Settings::Server>& Settings::getServerList() const
{
    return serverList;
}

void Settings::setServerList(const QList<Server>& newServerList)
{
    serverList = newServerList;
}

bool Settings::getAutojoinChannels() const
{
    return autojoinChannels;
}

void Settings::setAutojoinChannels(bool value)
{
    autojoinChannels = value;
}

const QList<Settings::Channel>& Settings::getAutojoinChannelList() const
{
    return autojoinChannelList;
}

void Settings::setAutojoinChannelList(const QList<Channel>& newChannelList)
{
    autojoinChannelList = newChannelList;
}

bool Settings::getWaitBeforeAutojoining() const
{
    return waitBeforeAutojoining;
}

void Settings::setWaitBeforeAutojoining(bool value)
{
    waitBeforeAutojoining = value;
}

int Settings::getTimeToWaitBeforeAutojoining() const
{
    return timeToWaitBeforeAutojoining;
}

void Settings::setTimeToWaitBeforeAutojoining(int sec)
{
    timeToWaitBeforeAutojoining = sec;
}

const QFont& Settings::getChatListFont() const
{
    return chatListFont;
}

void Settings::setChatListFont(const QFont& newFont)
{
    chatListFont = newFont;
}

const QFont& Settings::getChatWindowFont() const
{
    return chatWindowFont;
}

void Settings::setChatWindowFont(const QFont& newFont)
{
    chatWindowFont = newFont;
}

const QFont& Settings::getUserListFont() const
{
    return userListFont;
}

void Settings::setUserListFont(const QFont& newFont)
{
    userListFont = newFont;
}

void Settings::load()
{
    if (loaded) {
        return;
    }

    QSettings s("settings.ini", QSettings::IniFormat);
    s.beginGroup("Chat");

        s.beginGroup("General");
            serverId = s.value("serverId", 0).toInt();
            autoConnect = s.value("autoConnect", false).toBool();
            srand(QDateTime::currentDateTime().toTime_t());
            nick = s.value("nick", QString("nfkl_user%1").arg(rand() % 10000)).toString();
            username = s.value("username", "nfkl_user").toString();
            quitMessage = s.value("quitMessage").toString();
            autoIdentify = s.value("autoIdentify", false).toBool();
            password = s.value("password").toString();
        s.endGroup();

        s.beginGroup("Server");
            int serverListSize = s.beginReadArray("serverList");
            for (int i = 0; i < serverListSize; i ++) {
                s.setArrayIndex(i);
                Server server;
                server.name = s.value("name").toString();
                server.address = s.value("address").toString();
                server.port = s.value("port").toInt();
                server.password = s.value("password").toString();
                server.encoding = s.value("encoding").toString();
                serverList << server;
            }
            s.endArray();
        s.endGroup();

        s.beginGroup("Channel");
            autojoinChannels = s.value("autojoinChannels", false).toBool();
            int autojoinChannelListSize = s.beginReadArray("autojoinChannelList");
            for (int i = 0; i < autojoinChannelListSize; i ++) {
                s.setArrayIndex(i);
                Channel channel;
                channel.name = s.value("name").toString();
                channel.password = s.value("password").toString();
                autojoinChannelList << channel;
            }
            s.endArray();
            waitBeforeAutojoining = s.value("waitBeforeAutojoining", false).toBool();
            timeToWaitBeforeAutojoining = s.value("timeToWaitBeforeAutojoining", 10).toInt();
        s.endGroup();

        s.beginGroup("Font");
            chatListFont = s.value("chatListFont", QFont("Sans Serif", 10)).value<QFont>();
            chatWindowFont = s.value("chatWindowFont", QFont("Curier", 8)).value<QFont>();
            userListFont = s.value("userListFont", QFont("Sans Serif", 10)).value<QFont>();
        s.endGroup();

    s.endGroup();

    loaded = true;
}

void Settings::save()
{
    QSettings s("settings.ini", QSettings::IniFormat);
    s.beginGroup("Chat");

        s.beginGroup("General");
            s.setValue("serverId", serverId);
            s.setValue("autoConnect", autoConnect);
            s.setValue("nick", nick);
            s.setValue("username", username);
            s.setValue("quitMessage", quitMessage);
            s.setValue("autoIdentify", autoIdentify);
            s.setValue("password", password);
        s.endGroup();

        s.beginGroup("Server");
            s.beginWriteArray("serverList");
            for (int i = 0; i < serverList.size(); i ++) {
                s.setArrayIndex(i);
                s.setValue("name", serverList[i].name);
                s.setValue("address", serverList[i].address);
                s.setValue("port", serverList[i].port);
                s.setValue("password", serverList[i].password);
                s.setValue("encoding", serverList[i].encoding);
            }
            s.endArray();
        s.endGroup();

        s.beginGroup("Channel");
            s.setValue("autojoinChannels", autojoinChannels);
            s.beginWriteArray("autojoinChannelList");
            for (int i = 0; i < autojoinChannelList.size(); i ++) {
                s.setArrayIndex(i);
                s.setValue("name", autojoinChannelList[i].name);
                s.setValue("password", autojoinChannelList[i].password);
            }
            s.endArray();
            s.setValue("waitBeforeAutojoining", waitBeforeAutojoining);
            s.setValue("timeToWaitBeforeAutojoining", timeToWaitBeforeAutojoining);
        s.endGroup();

        s.beginGroup("Font");
            s.setValue("chatListFont", chatListFont);
            s.setValue("chatWindowFont", chatWindowFont);
            s.setValue("userListFont", userListFont);
        s.endGroup();

    s.endGroup();
}

void Settings::executeSettingsDialog(QWidget* parent)
{
    SettingsDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {
        save();
        emit dataChanged();
    }
}

} // namespace Chat
