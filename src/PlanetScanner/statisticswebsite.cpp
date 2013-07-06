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

#include "statisticswebsite.hpp"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>

StatisticsWebSite::StatisticsWebSite(QObject *parent) :
    QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    request = new QNetworkRequest(QUrl("http://nfk.pro2d.ru/api.php?action=gsl"));
}

void StatisticsWebSite::requestPlayersInfo()
{
    QNetworkReply* reply = manager->get(*request);
    connect(reply, &QNetworkReply::finished, this, &StatisticsWebSite::processPlayersInfo);
}

void StatisticsWebSite::processPlayersInfo()
{
    QNetworkReply* reply = static_cast<QNetworkReply*>(QObject::sender());
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    if (!document.isArray()) {
        return;
    }

    QHash<QString, QList<PlayerInfo>> playersHash;

    const QJsonArray array = document.array();

    for (int i = 0; i < array.size(); i ++) {
        const QJsonValue gameValue = array.at(i);
        if (!gameValue.isObject()) {
            continue;
        }
        const QJsonObject gameObject = gameValue.toObject();
        if (!gameObject.contains("ip") || !gameObject.contains("players")) {
            continue;
        }
        const QJsonValue ipValue = gameObject["ip"];
        const QJsonValue playersValue = gameObject["players"];
        if (!ipValue.isString() || !playersValue.isArray()) {
            continue;
        }
        QString ip = ipValue.toString();
        const QJsonArray playersArray = playersValue.toArray();
        QList<PlayerInfo> playersList;
        for (int j = 0; j < playersArray.size(); j ++) {
            const QJsonValue playerValue = playersArray.at(j);
            if (!playerValue.isObject()) {
                continue;
            }
            const QJsonObject playerObject = playerValue.toObject();
            PlayerInfo playerInfo;
            const QJsonValue countryValue = playerObject["country"];
            if (!countryValue.isUndefined() && !countryValue.isNull()) {
                playerInfo.country = countryValue.toString();
            }
            const QJsonValue modelValue = playerObject["model"];
            if (!modelValue.isUndefined() && !modelValue.isNull()) {
                playerInfo.model = modelValue.toString();
            }
            const QJsonValue nameValue = playerObject["name"];
            if (!nameValue.isUndefined() && !nameValue.isNull()) {
                playerInfo.name = nameValue.toString();
            }
            const QJsonValue nickValue = playerObject["nick"];
            if (!nickValue.isUndefined() && !nickValue.isNull()) {
                playerInfo.nick = nickValue.toString();
            }
            const QJsonValue placeValue = playerObject["place"];
            if (!placeValue.isUndefined() && !placeValue.isNull()) {
                playerInfo.place = placeValue.toDouble();
            }
            const QJsonValue playerIdValue = playerObject["playerID"];
            if (!playerIdValue.isUndefined() && !playerIdValue.isNull()) {
                playerInfo.playerId = playerIdValue.toDouble();
            }
            const QJsonValue pointsValue = playerObject["points"];
            if (!pointsValue.isUndefined() && !pointsValue.isNull()) {
                playerInfo.points = pointsValue.toDouble();
            }
            playersList << playerInfo;
        }
        playersHash[ip] = playersList;
    }

    if (playersHash.size() != 0) {
        emit playersInfoRecieved(playersHash);
    }

    reply->deleteLater();
}
