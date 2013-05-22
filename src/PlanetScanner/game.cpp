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
    switch (type) {
        case Game::DM:      return "DM";    break;
        case Game::TDM:     return "TDM";   break;
        case Game::CTF:     return "CTF";   break;
        case Game::RAIL:    return "RAIL";  break;
        case Game::PRAC:    return "PRAC";  break;
        case Game::DOM:     return "DOM";   break;
        default:            return "UNK";
    }
}

QString Game::getCleanedHostname() const
{
    QString cleanedHostname(getHostname());
    return cleanedHostname.replace(QRegularExpression("\\^[^\\^]"), "");
}
