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

#ifndef STATISTICSWEBSITE_HPP
#define STATISTICSWEBSITE_HPP

#include <QHash>
#include <QNetworkAccessManager>
#include <QNetworkRequest>

namespace PlanetScanner {

class StatisticsWebSite : public QObject
{
    Q_OBJECT
public:
    StatisticsWebSite(QObject* parent);
    void requestPlayersInfo();

    struct PlayerInfo {
        QString country;
        QString model;
        QString name;
        QString nick;
        int place;
        int playerId;
        int points;
    };

private:
    QNetworkAccessManager* manager;
    QNetworkRequest* request;

private slots:
    void processPlayersInfo();

signals:
    void playersInfoRecieved(QHash<QString, QList<PlayerInfo>>);

};

} // namespace PlanetScanner

#endif // STATISTICSWEBSITE_HPP
