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

#ifndef CHATSETTINGSSERVERPAGE_HPP
#define CHATSETTINGSSERVERPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"

#include <QGroupBox>
#include <QStandardItemModel>
#include <QString>
#include <QTreeView>

class ChatSettingsServerPage : public AbstractSettingsPage
{
    Q_OBJECT
public:
    explicit ChatSettingsServerPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath);

    void save(QSettings& settings);
    void apply();
    void buildGui();
    void setGui();

    struct Server
    {
        QString name;
        QString address;
        int port;
        QString password;
        QString encoding;
    };

    static QList<Server> getServerList()            {return serverList;}
    static QStandardItemModel* getServerListModel() {return serverListModel;}

private:
    void load(QSettings& settings);

    QGroupBox* buildServerGroup();
    static void buildServerModel();

    static QList<Server> serverList;
    QList<Server> modifiedServerList;
    static QStandardItemModel* serverListModel;
    QTreeView* serverListView;

private slots:
    void serverAddButtonClicked();
    void serverRemoveButtonClicked();
    void serverEditButtonClicked();
};

#endif // CHATSETTINGSSERVERPAGE_HPP
