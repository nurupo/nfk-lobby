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

#ifndef CHATSETTINGS_HPP
#define CHATSETTINGS_HPP

#include <QWidget>

namespace Chat {

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings& getInstance();
    void load();
    void save();
    void executeSettingsDialog(QWidget* parent);


    //

    int getServerId() const;
    void setServerId(int value);
    bool getAutoConnect() const;
    void setAutoConnect(bool value);
    const QString& getNick() const;
    void setNick(const QString& newNick);
    const QString& getUsername() const;
    void setUsername(const QString newUsername);
    const QString& getQuitMessage() const;
    void setQuitMessage(const QString& newQuitMessage);
    bool getAutoIdentify() const;
    void setAutoIdentify(bool value);
    const QString& getPassword() const;
    void setPassword(const QString& newPassword);

    //

    struct Server
    {
        QString name;
        QString address;
        int port;
        QString password;
        QString encoding;
    };

    const QList<Server>& getServerList() const;
    void setServerList(const QList<Server>& newServerList);

    //

    struct Channel
    {
        QString name;
        QString password;
    };

    bool getAutojoinChannels() const;
    void setAutojoinChannels(bool value);
    const QList<Channel>& getAutojoinChannelList() const;
    void setAutojoinChannelList(const QList<Channel>& newChannelList);
    bool getWaitBeforeAutojoining() const;
    void setWaitBeforeAutojoining(bool value);
    int getTimeToWaitBeforeAutojoining() const;
    void setTimeToWaitBeforeAutojoining(int sec);

    //

    const QFont& getChatListFont() const;
    void setChatListFont(const QFont& newFont);
    const QFont& getChatWindowFont() const;
    void setChatWindowFont(const QFont& newFont);
    const QFont& getUserListFont() const;
    void setUserListFont(const QFont& newFont);

private:
    int serverId;
    bool autoConnect;
    QString nick;
    QString username;
    QString quitMessage;
    bool autoIdentify;
    QString password;

    //

    QList<Server> serverList;
    //QList<Server> modifiedServerList;

    //

    bool autojoinChannels;
    QList<Channel> autojoinChannelList;
    bool waitBeforeAutojoining;
    int timeToWaitBeforeAutojoining;

    //

    QFont chatListFont;
    QFont chatWindowFont;
    QFont userListFont;


    bool loaded;

    Settings();
    Settings(Settings &settings) = delete;
    Settings& operator=(const Settings&) = delete;

signals:
    void dataChanged();
};

} // namespace Chat

#endif // CHATSETTINGS_HPP
