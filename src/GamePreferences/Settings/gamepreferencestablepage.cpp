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

#include "gamepreferencestablepage.hpp"

#include <QVBoxLayout>

QVector<bool> GamePreferencesTablePage::gameTypes;
bool GamePreferencesTablePage::name = false;

GamePreferencesTablePage::GamePreferencesTablePage(QStackedWidget *parent, const QString &pageName, const QString &iconPath) :
    AbstractSettingsPage(parent, pageName, iconPath)
{
}

void GamePreferencesTablePage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* generalGroup = buildHideColumnsGroup();

    layout->addWidget(generalGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* GamePreferencesTablePage::buildHideColumnsGroup()
{
    QGroupBox* group = new QGroupBox("Hide columns", this);
    QVBoxLayout* groupLayout = new QVBoxLayout(group);

    nameCheckBox = new QCheckBox("Name", group);
    groupLayout->addWidget(nameCheckBox);

    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        gameTypesCheckBoxes[i] = new QCheckBox(GameType::getName(i), group);
        groupLayout->addWidget(gameTypesCheckBoxes[i]);
    }

    return group;
}

void GamePreferencesTablePage::setGui()
{
    nameCheckBox->setChecked(name);
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        gameTypesCheckBoxes[i]->setChecked(gameTypes[i]);
    }
}

void GamePreferencesTablePage::apply()
{
    name = nameCheckBox->isChecked();
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        gameTypes[i] = gameTypesCheckBoxes[i]->isChecked();
    }
}

void GamePreferencesTablePage::load(QSettings& settings)
{
    name = settings.value("name", false).toBool();
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        gameTypes << settings.value(GameType::getName(i), false).toBool();
    }
}

void GamePreferencesTablePage::save(QSettings& settings)
{
    settings.setValue("name", name);
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        settings.setValue(GameType::getName(i), gameTypes[i]);
    }
}

QVector<bool> GamePreferencesTablePage::getHiddenColumns()
{
    QVector<bool> hiddenColumns;
    hiddenColumns << name << gameTypes;
    return hiddenColumns;
}
