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

#ifndef PLANET_HPP
#define PLANET_HPP

#include "game.hpp"

#include <QAbstractSocket>
#include <QTcpSocket>

namespace PlanetScanner {

class Planet : public QObject
{
     Q_OBJECT
public:
    explicit Planet(const QString &address, const int port);
    ~Planet();
    const QString & getAddress() const { return address; }
    int getPort() const { return port; }

private:
    void connect();
    void disconnect();

    const QString address;
    const int port;
    QTcpSocket* const sock;
    bool errorState;

signals:
    void gameInfoRecieved(const Planet &planet, const QList<Game> &status);
    void connected(const Planet &planet);
    void error(const Planet &planet, QAbstractSocket::SocketError socketError);
    void errorCleared(const Planet &planet);

private slots:
    void parseServerResponse();
    void requestVersion();
    void error(QAbstractSocket::SocketError socketError);

public slots:
    void requestInfo();
};

} // namespace PlanetScanner

#endif // PLANET_HPP
