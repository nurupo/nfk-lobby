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

#ifndef CHATSETTINGSGENERALPAGE_HPP
#define CHATSETTINGSGENERALPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"

#include <QAbstractItemModel>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QString>

class ChatSettingsGeneralPage : public AbstractSettingsPage
{
public:
    explicit ChatSettingsGeneralPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath);

    static int getServerId()                        {return serverId;}
    static QString getNick()                        {return nick;}
    static QString getUsername()                    {return username;}
    static QString getQuitMessage()                 {return quitMessage;}
    static bool getAutoIdentify()                   {return autoIdentify;}
    static QString getPassword()                    {return password;}

private:
    void save(QSettings& settings);
    void load(QSettings& settings);
    void apply();
    void setGui();
    void buildGui();

    QGroupBox* buildGeneralGroup();
    QGroupBox* buildAutoIdentifyGroup();

    QComboBox* serverSelectionComboBox;
    QLineEdit* nickEdit;
    QLineEdit* usernameEdit;
    QLineEdit* quitMessageEdit;

    QGroupBox* autoIdentifygroup;
    QLineEdit* passwordEdit;

    static int serverId;
    static QString nick;
    static QString username;
    static QString quitMessage;
    static bool autoIdentify;
    static QString password;
};

#endif // CHATSETTINGSGENERALPAGE_HPP
