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

#ifndef CHATSETTINGSCHANNELPAGE_HPP
#define CHATSETTINGSCHANNELPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"

#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QString>
#include <QTreeView>

class ChatSettingsChannelPage : public AbstractSettingsPage
{
    Q_OBJECT
public:
    explicit ChatSettingsChannelPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath);

    struct Channel
    {
        QString name;
        QString password;
    };

    static bool getAutojoinChannels()               {return autojoinChannels;}
    static QList<Channel> getAutojoinChannelList()  {return autojoinChannelList;}
    static bool getWaitBeforeAutojoining()          {return waitBeforeAutojoining;}
    static int getTimeToWaitBeforeAutojoining()     {return timeToWaitBeforeAutojoining;}

private:
    void save(QSettings& settings);
    void load(QSettings& settings);
    void apply();
    void setGui();
    void buildGui();

    QGroupBox* buildAutojoinGroup();
    QGroupBox* buildChannelGroup(QGroupBox* parent);
    QGroupBox* buildOptionsGroup(QGroupBox* parent);

    QGroupBox* autojoinGroup;
    QStandardItemModel* channelListModel;
    QTreeView* channelListView;
    QCheckBox* waitCheckBox;
    QSpinBox* waitSpinBox;

    static bool autojoinChannels;
    static QList<Channel> autojoinChannelList;
    static bool waitBeforeAutojoining;
    static int timeToWaitBeforeAutojoining;

private slots:
    void channelAddButtonClicked();
    void channelRemoveButtonClicked();

};

#endif // CHATSETTINGSCHANNELPAGE_HPP
