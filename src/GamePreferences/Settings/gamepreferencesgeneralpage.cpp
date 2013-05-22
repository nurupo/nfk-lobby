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

#include "gamepreferencesgeneralpage.hpp"

#include <QGridLayout>
#include <QLabel>

QString GamePreferencesGeneralPage::inGameNick;

GamePreferencesGeneralPage::GamePreferencesGeneralPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath) :
    AbstractSettingsPage(parent, pageName, iconPath)
{
}

void GamePreferencesGeneralPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* generalGroup = buildGeneralGroup();

    layout->addWidget(generalGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* GamePreferencesGeneralPage::buildGeneralGroup()
{
    QGroupBox* group = new QGroupBox("General options", this);
    QGridLayout* groupLayout = new QGridLayout(group);

    QLabel* inGameNickLabel = new QLabel("In-game nick:", group);

    inGameNickEdit = new QLineEdit(group);
    inGameNickEdit->setMaxLength(MAX_INGAMENICK_LENGTH);

    groupLayout->addWidget(inGameNickLabel, 0, 0, 1, 1);
    groupLayout->addWidget(inGameNickEdit,  0, 1, 1, 1);

    return group;
}

void GamePreferencesGeneralPage::setGui()
{
    inGameNickEdit->setText(inGameNick);
}

void GamePreferencesGeneralPage::apply()
{
    inGameNick = inGameNickEdit->text();
}

void GamePreferencesGeneralPage::load(QSettings& settings)
{
    inGameNick = settings.value("inGameNick", "Happy NFK Player :)").toString();
    inGameNick.truncate(MAX_INGAMENICK_LENGTH);
}

void GamePreferencesGeneralPage::save(QSettings& settings)
{
    settings.setValue("inGameNick", inGameNick);
}
