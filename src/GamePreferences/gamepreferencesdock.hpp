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

#ifndef GAMEPREFERENCESDOCK_HPP
#define GAMEPREFERENCESDOCK_HPP

#include "gametype.hpp"
#include "IrcClient/channel.hpp"
#include "IrcClient/ircclient.hpp"
#include "preferencessortingmodel.hpp"

#include <QDockWidget>
#include <QHash>
#include <QPersistentModelIndex>
#include <QStandardItemModel>
#include <QTableView>
#include <QTextEdit>

class GamePreferencesDock : public QDockWidget
{
    Q_OBJECT
public:
    GamePreferencesDock(QWidget* parent);
    ~GamePreferencesDock();

private:
    IrcClient* ircClient;
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
    void onChannelJoined(const Channel &channel);
    void onChannelMessageReceived(const User& sender, const QString& message);
    void onPrivateMessageReceived(const User& sender, const QString& message);
    void onOurGameTypeModified(QStandardItem* item);
    void onUserParted(const User &user);
    void onUserQuit(const User &user);
    void onUserKicked(const User& sender, const QString& message, const QString& recipient);
    void onUserChangedNick(const User& user, const QString& newNick);
    void onNickAlreadyInUse();
    void applySettings();
    void showSettingsDialog();

};

#endif // GAMEPREFERENCESDOCK_HPP
