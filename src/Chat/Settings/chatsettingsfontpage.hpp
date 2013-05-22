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

#ifndef CHATSETTINGSFONTPAGE_HPP
#define CHATSETTINGSFONTPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"

#include <QFont>
#include <QGroupBox>
#include <QLineEdit>
#include <QString>

class ChatSettingsFontPage : public AbstractSettingsPage
{
    Q_OBJECT
public:
    explicit ChatSettingsFontPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath);

    static QFont getChatListFont()                  {return chatListFont;}
    static QFont getChatWindowFont()                {return chatWindowFont;}
    static QFont getUserListFont()                  {return userListFont;}

private:
    void load(QSettings& settings);
    void save(QSettings& settings);
    void apply();
    void buildGui();
    void setGui();


    QLineEdit* chatListEdit;
    QLineEdit* chatWindowEdit;
    QLineEdit* userListEdit;

    QGroupBox* buildFontGroup();
    void setEditWithFont(QLineEdit* edit, const QFont& font);
    void setEditWithFont(QLineEdit* edit);

    static QFont chatListFont;
    static QFont chatWindowFont;
    static QFont userListFont;

private slots:
    void chatListButtonClicked();
    void chatWindowButtonClicked();
    void userListButtonClicked();

};

#endif // CHATSETTINGSFONTPAGE_HPP
