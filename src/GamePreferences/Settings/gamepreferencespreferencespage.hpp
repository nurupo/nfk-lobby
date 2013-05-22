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

#ifndef GAMEPREFERENCESPREFERENCESPAGE_HPP
#define GAMEPREFERENCESPREFERENCESPAGE_HPP

#include "../gametype.hpp"
#include "Settings/abstractsettingspage.hpp"

#include <QCheckBox>
#include <QGroupBox>
#include <QString>
#include <QVector>

class GamePreferencesPreferencesPage : public AbstractSettingsPage
{
public:
    explicit GamePreferencesPreferencesPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath);

    static QVector<bool> getDefaultPreferences()  {return preferences;}

private:
    void save(QSettings& settings);
    void load(QSettings& settings);
    void apply();
    void setGui();
    void buildGui();

    QGroupBox* buildDefaultPreferencesGroup();
    QCheckBox* defaultPreferenceCheckBox[GameType::NUMBER_OF_GAMETYPES];

    static QVector<bool> preferences;


};

#endif // GAMEPREFERENCESPREFERENCESPAGE_HPP

