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

#include "gamepreferencesgeneralsettingspage.hpp"
#include "gamepreferencessettings.hpp"

#include <QGridLayout>
#include <QLabel>

namespace GamePreferences {

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void GeneralSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* generalGroup = buildGeneralGroup();

    layout->addWidget(generalGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* GeneralSettingsPage::buildGeneralGroup()
{
    QGroupBox* group = new QGroupBox("General options", this);
    QGridLayout* layout = new QGridLayout(group);

    QLabel* inGameNickLabel = new QLabel("In-game nick:", group);

    inGameNickEdit = new QLineEdit(group);
    inGameNickEdit->setMaxLength(Settings::MAX_INGAMENICK_LENGTH);

    layout->addWidget(inGameNickLabel, 0, 0, 1, 1);
    layout->addWidget(inGameNickEdit,  0, 1, 1, 1);

    return group;
}

void GeneralSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    inGameNickEdit->setText(settings.getInGameNick());
}

void GeneralSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    settings.setInGameNick(inGameNickEdit->text());
}

} // namespace GamePreferences
