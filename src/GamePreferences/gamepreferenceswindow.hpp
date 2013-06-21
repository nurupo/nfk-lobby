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

#ifndef GAMEPREFERENCESWINDOW_HPP
#define GAMEPREFERENCESWINDOW_HPP

#include "gametype.hpp"
#include "IrcClient/channel.hpp"
#include "IrcClient/ircclient.hpp"
#include "preferencessortingmodel.hpp"

#include <QHash>
#include <QMainWindow>
#include <QPersistentModelIndex>
#include <QStandardItemModel>
#include <QTableView>
#include <QTextEdit>

namespace GamePreferences {

class Window : public QMainWindow
{
    Q_OBJECT
public:
    Window(QWidget* parent = 0);
    ~Window();

private:
    IrcClient::IrcClient* ircClient;
    QTableView* preferencesView;
    QStandardItemModel* preferencesModel;
    PreferencesSortingModel* sortModel;
    QHash<QString, QPersistentModelIndex> ircNickToIndexMap;

    bool gamePreferences[GameType::NUMBER_OF_GAMETYPES];
    QString playerName;

    QString channelName;

    void addPlayer(const QString &ircNick, const QString &gameNick);
    void updateGameType(const QString &ircNick, bool set, int gameTypeId);
    void updatePlayerName(const QString &ircNick, const QString newPlayerName);
    void updateOurPlayerName(const QString &newPlayerName);
    void sendGameTypeChange(bool set, int gameTypeId);
    void sendGameTypeInformation(const QString &target);
    void sendOurName(const QString &target);
    void removePlayer(const QString &ircNick);
    QString getRandomIrcNick();

private slots:
    void connectToServer();
    void disconnectFromServer();
    void onConnected();
    void onDisconnected();
    void onChannelJoined(const IrcClient::Channel &channel);
    void onChannelMessageReceived(const IrcClient::User& sender, const QString& message);
    void onPrivateMessageReceived(const IrcClient::User& sender, const QString& message);
    void onOurGameTypeModified(QStandardItem* item);
    void onUserParted(const IrcClient::User &user);
    void onUserQuit(const IrcClient::User &user);
    void onUserKicked(const IrcClient::User& sender, const QString& message, const QString& recipient);
    void onUserChangedNick(const IrcClient::User& user, const QString& newNick);
    void onNickAlreadyInUse();
    void applySettings();
    void showSettingsDialog();

};

} // namespace GamePreferences

#endif // GAMEPREFERENCESWINDOW_HPP
