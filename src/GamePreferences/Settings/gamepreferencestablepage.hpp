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

#ifndef GAMEPREFERENCESTABLEPAGE_HPP
#define GAMEPREFERENCESTABLEPAGE_HPP

#include "../gametype.hpp"
#include "Settings/abstractsettingspage.hpp"

#include <QCheckBox>
#include <QGroupBox>
#include <QString>
#include <QVector>

class GamePreferencesTablePage : public AbstractSettingsPage
{
public:
    explicit GamePreferencesTablePage(QStackedWidget *parent, const QString &pageName, const QString &iconPath);

    static QVector<bool> getHiddenColumns();

private:
    void save(QSettings& settings);
    void load(QSettings& settings);
    void apply();
    void setGui();
    void buildGui();

    QGroupBox* buildHideColumnsGroup();
    QCheckBox* nameCheckBox;
    QCheckBox* gameTypesCheckBoxes[GameType::NUMBER_OF_GAMETYPES];

    static bool name;
    static QVector<bool> gameTypes;


};

#endif // GAMEPREFERENCESTABLEPAGE_HPP
