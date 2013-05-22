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

#ifndef GAMEPREFERENCESGENERALPAGE_HPP
#define GAMEPREFERENCESGENERALPAGE_HPP

#include "Settings/abstractsettingspage.hpp"

#include <QGroupBox>
#include <QLineEdit>
#include <QString>

class GamePreferencesGeneralPage : public AbstractSettingsPage
{
public:
    explicit GamePreferencesGeneralPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath);

    static QString getInGameNick()  {return inGameNick;}

private:
    void save(QSettings& settings);
    void load(QSettings& settings);
    void apply();
    void setGui();
    void buildGui();

    QGroupBox* buildGeneralGroup();

    static const int MAX_INGAMENICK_LENGTH = 30;
    QLineEdit* inGameNickEdit;
    static QString inGameNick;


};

#endif // GAMEPREFERENCESGENERALPAGE_HPP
