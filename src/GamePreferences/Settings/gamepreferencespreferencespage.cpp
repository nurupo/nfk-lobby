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

#include "gamepreferencespreferencespage.hpp"

#include <QGridLayout>

QVector<bool> GamePreferencesPreferencesPage::preferences;

GamePreferencesPreferencesPage::GamePreferencesPreferencesPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath) :
    AbstractSettingsPage(parent, pageName, iconPath)
{
}

void GamePreferencesPreferencesPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* generalGroup = buildDefaultPreferencesGroup();

    layout->addWidget(generalGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* GamePreferencesPreferencesPage::buildDefaultPreferencesGroup()
{
    QGroupBox* group = new QGroupBox("Default game preferences", this);
    QGridLayout* groupLayout = new QGridLayout(group);

    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        defaultPreferenceCheckBox[i] = new QCheckBox(GameType::getName(i), group);
    }

    groupLayout->addWidget(defaultPreferenceCheckBox[0],    0, 0, 1, 1);
    groupLayout->addWidget(defaultPreferenceCheckBox[1],    0, 1, 1, 1);
    groupLayout->addWidget(defaultPreferenceCheckBox[2],    0, 2, 1, 1);
    groupLayout->addWidget(defaultPreferenceCheckBox[3],    0, 3, 1, 1);
    groupLayout->addWidget(defaultPreferenceCheckBox[4],    1, 0, 1, 2, Qt::AlignCenter);
    groupLayout->addWidget(defaultPreferenceCheckBox[5],    1, 1, 1, 2, Qt::AlignCenter);
    groupLayout->addWidget(defaultPreferenceCheckBox[6],    1, 2, 1, 2, Qt::AlignCenter);

    return group;
}

void GamePreferencesPreferencesPage::setGui()
{
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        defaultPreferenceCheckBox[i]->setChecked(preferences[i]);
    }
}

void GamePreferencesPreferencesPage::apply()
{
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        preferences[i] = defaultPreferenceCheckBox[i]->isChecked();
    }
}

void GamePreferencesPreferencesPage::load(QSettings& settings)
{
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        preferences << settings.value(GameType::getName(i), false).toBool();
    }
}

void GamePreferencesPreferencesPage::save(QSettings& settings)
{
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        settings.setValue(GameType::getName(i), preferences[i]);
    }
}
