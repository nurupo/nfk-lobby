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

#ifndef IRCCLIENT_HPP
#define IRCCLIENT_HPP

#include "channel.hpp"

#include <QObject>
#include <QTcpSocket>
#include <QTextCodec>

class IrcClient : public QObject
{
    Q_OBJECT
public:
    IrcClient();
    ~IrcClient();

    void connect(const QString &address, int port, const QString &password, const QString &nick, const QString &username, const QString &encoding);
    void disconnect();

    const User & getUs() const {return us;}

    bool hasChannel(const QString& name) const {return channels.contains(name);}
    const Channel& getChannel(const QString& name) const {return *channels.value(name);}
    int getChannelCount() const {return channels.size();}
    QHashIterator<QString, Channel*> getChannelIterator() const {return QHashIterator<QString, Channel*>(channels);}

    void setCodec(const QString &encoding);
    void setQuitMessage(const QString &message) {quitMessage = message;}
    void setVersion(const QString &version) {this->version = version;}

    void sendNotice(const QString &nick, const QString &string);
    void sendCtcpReply(const QString &nick, const QString &string);
    void sendMessage(const QString &target, const QString &string);
    void sendCtcpRequest(const QString &target, const QString &string);
    void sendRaw(const QString &string);
    void sendAction(const QString &target, const QString &string);
    void sendQuit();
    void sendPart(const QString &channel);
    void joinChannel(const QString &name, const QString &password = QString());
    void identify(const QString &password);

private:
    void processServerLine(const QString &message);
    void processServerResponse(const QString &code, const QString &response);
    void processUserLine(const QString &line);
    void processMode(const QString &target, const User &sender, const QString &mode);
    bool isChannel(const QString &channelName) const;
    void renameUser(const User &user, const QString &newNick);
    void removeUser(const QString &nick);
    void quitUser(const User &user, const QString &message);
    void callWho(const QString &target);

    void removeChannels();

    QString address;
    int port;
    QString password;
    User us;
    QTextCodec* codec;
    QTcpSocket sock;
    bool isConnected;
    bool hasQuit;
    QHash<QString, Channel*> channels;
    static const char Ctcp = '\1';
    static const QString ChannelPrefixes;
    QString version;
    QString Finger;
    QString quitMessage;
    QHash<QString, bool> calledWho;
    bool setUs;
    
signals:
    void connecting(const QString &address, int port);
    void unknown(const QString &line);
    void ctcpRequest(const QString &target, const User &sender, const QString &request);
    void ctcpReply(const QString &target, const User &sender, const QString &request, const QString &message);
    void privateMessage(const User &sender, const QString &message, const QString &target);
    void action(const User &sender, const QString &action, const QString &target);
    void join(const Channel& channel);
    void part(const Channel& channel);
    void notice(const User &sender, const QString &notice);
    void serverResponse(const QString &response);
    void nickChange(const QString &newNick);
    void connected();
    void disconnecting();
    void disconnected();
    void quit(const QString &message);
    void userModeChanged(const QString &target, const User &sender, const QString &mode);
    void nickAlreadyInUse();

private slots:
    void connectionEstablished();
    void readServerMessages();
    
};

#endif // IRCCLIENT_HPP
