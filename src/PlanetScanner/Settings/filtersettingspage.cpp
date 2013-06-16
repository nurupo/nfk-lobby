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

#include "filtersettingspage.hpp"
#include "planetscannersettings.hpp"

#include <QMapIterator>
#include <QVBoxLayout>

namespace PlanetScanner {

FilterSettingsPage::FilterSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void FilterSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    gametypeFilterGroup = buildGametypeFilterGroup();
    QGroupBox* playerCuntFilterGroup = buildPlayerCountFilterGroup();

    layout->addWidget(gametypeFilterGroup, 0, Qt::AlignTop);
    layout->addWidget(playerCuntFilterGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* FilterSettingsPage::buildGametypeFilterGroup()
{
    QGroupBox* group = new QGroupBox("Hide games by gametype", this);
    QGridLayout* layout = new QGridLayout(group);

    QMapIterator<QString, bool> it(Settings::getInstance().getGameTypeFilter());
    int count = 0;
    int column = 0;
    while (it.hasNext()) {
        it.next();

        if (!(count % 3)) {
            column ++;
        }
        layout->addWidget(new QCheckBox(it.key(), group), count % 3, column, 1, 1);
        count ++;
    }

    return group;
}

QGroupBox* FilterSettingsPage::buildPlayerCountFilterGroup()
{
    QGroupBox* group = new QGroupBox("Hide games by player count", this);
    QVBoxLayout* layout = new QVBoxLayout(group);

    filterPlayersFullCheckbox = new QCheckBox("Full games", group);
    filterPlayersEmptyCheckbox = new QCheckBox("Empty games", group);

    layout->addWidget(filterPlayersFullCheckbox);
    layout->addWidget(filterPlayersEmptyCheckbox);

    return group;
}

void FilterSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    const QMap<QString, bool> gameTypeFilter = settings.getGameTypeFilter();
    for (int i = 0; i < gametypeFilterGroup->layout()->count(); i++) {
        QCheckBox* gameTypeCheckBox = static_cast<QCheckBox*>(gametypeFilterGroup->layout()->itemAt(i)->widget());
        gameTypeCheckBox->setChecked(gameTypeFilter[gameTypeCheckBox->text()]);
    }

    filterPlayersFullCheckbox->setChecked(settings.getHideOnFullFilter());
    filterPlayersEmptyCheckbox->setChecked(settings.getHideOnEmptyFilter());
}

void FilterSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    QMap<QString, bool> gameTypeFilter;
    for (int i = 0; i < gametypeFilterGroup->layout()->count(); i++) {
        QCheckBox* gameTypeCheckBox = static_cast<QCheckBox*>(gametypeFilterGroup->layout()->itemAt(i)->widget());
        gameTypeFilter[gameTypeCheckBox->text()] = gameTypeCheckBox->isChecked();
    }
    settings.setGameTypeFilter(gameTypeFilter);

    settings.setHideOnFullFilter(filterPlayersFullCheckbox->isChecked());
    settings.setHideOnEmptyFilter(filterPlayersEmptyCheckbox->isChecked());
}

} // namespace PlanetScanner
