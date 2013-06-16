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

#include "gamepreferencespreferencessettingspage.hpp"
#include "gamepreferencessettings.hpp"

#include <QGridLayout>

namespace GamePreferences {

PreferencesSettingsPage::PreferencesSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void PreferencesSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* generalGroup = buildDefaultPreferencesGroup();

    layout->addWidget(generalGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* PreferencesSettingsPage::buildDefaultPreferencesGroup()
{
    QGroupBox* group = new QGroupBox("Default game preferences", this);
    QGridLayout* layout = new QGridLayout(group);

    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        defaultPreferenceCheckBox[i] = new QCheckBox(GameType::getName(i), group);
    }

    layout->addWidget(defaultPreferenceCheckBox[0],    0, 0, 1, 1);
    layout->addWidget(defaultPreferenceCheckBox[1],    0, 1, 1, 1);
    layout->addWidget(defaultPreferenceCheckBox[2],    0, 2, 1, 1);
    layout->addWidget(defaultPreferenceCheckBox[3],    0, 3, 1, 1);
    layout->addWidget(defaultPreferenceCheckBox[4],    1, 0, 1, 2, Qt::AlignCenter);
    layout->addWidget(defaultPreferenceCheckBox[5],    1, 1, 1, 2, Qt::AlignCenter);
    layout->addWidget(defaultPreferenceCheckBox[6],    1, 2, 1, 2, Qt::AlignCenter);

    return group;
}

void PreferencesSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    const QVector<bool>& defaultPreferences = settings.getDefaultPreferences();
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        defaultPreferenceCheckBox[i]->setChecked(defaultPreferences.at(i));
    }
}

void PreferencesSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    QVector<bool> defaultPreferences;
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        defaultPreferences << defaultPreferenceCheckBox[i]->isChecked();
    }
    settings.setDefaultPreferences(defaultPreferences);
}

} // namespace GamePreferences
