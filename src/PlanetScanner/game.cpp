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

#include "game.hpp"

#include <QRegularExpression>

namespace PlanetScanner {

//FIXME: use initializer-list that will be introduced in Qt 5.1.0 https://bugreports.qt-project.org/browse/QTBUG-31094
//QHash<int, QString> Game::gameNames {{0, "DM"}, {2, "TDM"}, {3, "CTF"}, {4, "RAIL"}, {6, "PRAC"}, {7, "DOM"}};
QHash<int, QString> Game::gameNames = Game::initializeGameNames();

QHash<int, QString> Game::initializeGameNames()
{
    QHash<int, QString> hash;
    hash[0] = "DM";
    hash[2] = "TDM";
    hash[3] = "CTF";
    hash[4] = "RAIL";
    hash[6] = "PRAC";
    hash[7] = "DOM";
    return hash;
}

const QHash<int, QString>& Game::getGameTypeHash()
{
    return gameNames;
}

Game::Game(const QByteArray &serverInfo)
{
    if (serverInfo.size() < 2 || serverInfo.at(1) != Game::Identifier) {
        valid = false;
        return;
    }
    QList<QByteArray> infoList = serverInfo.split(Game::FieldDelimeter);
    if (infoList.size() != 8) {
        valid = false;
        return;
    }
    bool ok;
    ip = QString(infoList.at(0).right(infoList.at(0).size()-2));
    hostname = QString(infoList.at(1));
    map = QString(infoList.at(2));
    gametype = parseGameType(infoList.at(3));
    currentPlayers = infoList.at(4).toInt(&ok);
    if (!ok) {
        valid = false;
        return;
    }
    maxPlayers = infoList.at(5).toInt(&ok);
    if (!ok) {
        valid = false;
        return;
    }
    port = infoList.at(6).toInt(&ok);
    if (!ok) {
        valid = false;
        return;
    }
    /*TODO:check is validation is checked in planet_scanner_dock*/
    valid = true;
}

Game::~Game()
{
}

Game::GameType Game::parseGameType(const QByteArray &bytes)
{
    bool ok;
    int type = bytes.toInt(&ok);
    if (!ok) {
        return Game::UNK;
    }
    GameType gameType = static_cast<GameType>(type);
    switch (gameType) {
        case Game::DM:
        case Game::TDM:
        case Game::CTF:
        case Game::RAIL:
        case Game::PRAC:
        case Game::DOM:     break;
        default:            return Game::UNK;
    }
    return gameType;
}

QString Game::getNameForGametype(const enum Game::GameType type)
{
    /*https://bitbucket.org/coolant/nfk-planet-scanner/src/1eeb34f604b2a2242680113eada427b26803f1f8/main.pas?at=default#cl-610*/
    if (gameNames.contains(type)) {
        return gameNames[type];
    } else {
        return "UNK";
    }
}

Game::GameType Game::getGametypeForName(const QString &gameType)
{
    QHashIterator<int, QString> it(gameNames);
    while (it.hasNext()) {
        it.next();
        if (it.value() == gameType) {
            return static_cast<GameType>(it.key());
        }
    }
    return Game::UNK;
}

QString Game::getCleanedHostname() const
{
    QString cleanedHostname(getHostname());
    return cleanedHostname.replace(QRegularExpression("\\^[^\\^]"), "");
}

} // namespace PlanetScanner
