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

#ifndef PRIVATEPAGE_HPP
#define PRIVATEPAGE_HPP

#include "../../IrcClient/user.hpp"
#include "basicpage.hpp"

class PrivatePage : public BasicPage
{
    Q_OBJECT
public:
    explicit PrivatePage(QTreeWidgetItem* pageTab, QTreeWidget* tabTree);
    ~PrivatePage();

private:
    QAction* closeAction;

signals:
    void closeActionTriggered();

public slots:
    void privateMessage(const User &user, const QString &message);
    void action(const User &sender, const QString &action);

};

#endif // PRIVATEPAGE_HPP
