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

#include "planet.hpp"

namespace PlanetScanner {

Planet::Planet(const QString& address, const int port) : address(address), port(port), sock(new QTcpSocket())
{
    errorState = false;
    QObject::connect(sock, SIGNAL(connected()), this, SLOT(requestVersion()));
    QObject::connect(sock, SIGNAL(readyRead()), this, SLOT(parseServerResponse()));
    QObject::connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect();
}

Planet::~Planet()
{
    sock->close();
    delete sock;
}

void Planet::requestGameInfo()
{
    if (sock->state() == QAbstractSocket::UnconnectedState) {
        connect();
    } else {
        sock->write("?G\x0d\x0a");
    }
}

void Planet::connect()
{
    sock->connectToHost(address, port);
}

void Planet::disconnect()
{
    sock->disconnectFromHost();
}

void Planet::requestVersion()
{
    sock->write("?V077\x0d\x0a");
}

void Planet::error(QAbstractSocket::SocketError socketError)
{
    errorState = true;
    emit error(*this, socketError);
}

void Planet::parseServerResponse()
{
    QByteArray response = sock->readLine(129);

    if (errorState) {
        errorState = false;
        emit errorCleared(*this);
    }
    if (response.size() < 2) {
        /*too short response*/
        return;
    }
    if (response.at(0) == 'V' || response.at(1) == 'V') {
        if (response.endsWith("V077\x0a")) {
            requestGameInfo();
        } else {
            /*unsupported version of NFK Planet*/
        }
    } else if (response.at(1) == 'L') {
        QList<Game> games;
        Game game(response);
        if (game.isValid()) {
            games.append(game);
        }
        while (sock->canReadLine()) {
            response = sock->readLine(129);
            if (response.at(1) == 'E') {
                break;
            }
            game = Game(response);
            if (game.isValid()) {
                games << game;
            }
        }
        emit gameInfoRecieved(*this, games);
    } else if (response.at(1) == 'E') {
        emit gameInfoRecieved(*this, QList<Game>());
    } else {
        /*unknown response*/
    }
}

} // namespace PlanetScanner
