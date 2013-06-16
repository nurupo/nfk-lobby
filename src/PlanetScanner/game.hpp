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

#ifndef SERVER_HPP
#define SERVER_HPP

#include <QByteArray>
#include <QHash>
#include <QString>

namespace PlanetScanner {

class Game
{
public:
    explicit Game(const QByteArray &serverInfo);
    ~Game();
    enum GameType {UNK = -1, DM = 0, TDM = 2, CTF, RAIL, PRAC = 6, DOM};
    static const int MAX_GAMETYPE_ID = DOM;
    const QString & getHostname()       const { return hostname; }
    const QString & getMap()            const { return map; }
    enum GameType getGametype()         const { return gametype; }
    int getCurrentPlayers()             const { return currentPlayers; }
    int getMaxPlayers()                 const { return maxPlayers; }
    const QString & getIp()             const { return ip; }
    int getPort()                       const { return port; }
    bool isValid()                      const { return valid; }
    QString getCleanedHostname() const;
    static QString getNameForGametype(GameType gameType);
    static GameType getGametypeForName(const QString &gameType);
    static const QHash<int, QString>& getGameTypeHash();

private:
    QString hostname;
    QString map;
    GameType gametype;
    int currentPlayers;
    int maxPlayers;
    QString ip;
    int port;
    bool valid;

    static QHash<int, QString> gameNames;
    static QHash<int, QString> initializeGameNames();

    static const char Identifier = 'L';
    static const char FieldDelimeter = '\x0d';

    static enum GameType parseGameType(const QByteArray &bytes);
};

} // namespace PlanetScanner

#endif // SERVER_HPP
