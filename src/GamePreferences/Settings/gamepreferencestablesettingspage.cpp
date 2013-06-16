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

#include "gamepreferencessettings.hpp"
#include "gamepreferencestablesettingspage.hpp"

#include <QVBoxLayout>

namespace GamePreferences {

TableSettingsPage::TableSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void TableSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* generalGroup = buildHideColumnsGroup();

    layout->addWidget(generalGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* TableSettingsPage::buildHideColumnsGroup()
{
    QGroupBox* group = new QGroupBox("Hide columns", this);
    QVBoxLayout* layout = new QVBoxLayout(group);

    nameCheckBox = new QCheckBox("Name", group);
    layout->addWidget(nameCheckBox);

    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        gameTypesCheckBoxes[i] = new QCheckBox(GameType::getName(i), group);
        layout->addWidget(gameTypesCheckBoxes[i]);
    }

    return group;
}

void TableSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    const QVector<bool>& hideColumns = settings.getHiddenColumns();
    nameCheckBox->setChecked(hideColumns.at(0));
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        gameTypesCheckBoxes[i]->setChecked(hideColumns.at(i+1));
    }
}

void TableSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    QVector<bool> hideColumns;
    hideColumns << nameCheckBox->isChecked();
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        hideColumns << gameTypesCheckBoxes[i]->isChecked();
    }
    settings.setHiddenColumns(hideColumns);
}

} // namespace GamePreferences
