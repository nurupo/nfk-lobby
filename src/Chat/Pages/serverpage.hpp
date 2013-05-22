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

#ifndef SERVERPAGE_HPP
#define SERVERPAGE_HPP

#include "../../IrcClient/user.hpp"
#include "basicpage.hpp"

class ServerPage : public BasicPage
{
    Q_OBJECT
public:
    explicit ServerPage(QTreeWidgetItem* pageTab, QTreeWidget* tabTree);
    virtual ~ServerPage();

private:
    QAction* connectAction;
    QAction* disconnectAction;

signals:
    void connectActionTriggered();
    void disconnectActionTriggered();

public slots:
    void connecting(const QString &address, int port);
    void serverResponse(const QString &response);
    void notice(const User &sender, const QString &notice);
    void ctcpRequest(const QString &target, const User &sender, const QString &request);
    void ctcpReply(const QString &target, const User &sender, const QString &request, const QString &message);
    void quit(const QString &message);
    void disconnected();
    void userModeChanged(const QString &target, const User &sender, const QString &mode);
};

#endif // SERVERPAGE_HPP
