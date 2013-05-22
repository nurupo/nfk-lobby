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
#include "gamepreferencespreferencespage.hpp"
#include "gamepreferencessettingsdialog.hpp"
#include "gamepreferencestablepage.hpp"

BasicSettingsDialogNotifier GamePreferencesSettingsDialog::settingsNotifier;
bool GamePreferencesSettingsDialog::previouslyLoaded = false;
QString GamePreferencesSettingsDialog::sectionName = "GamePreferences";

GamePreferencesSettingsDialog::GamePreferencesSettingsDialog(QWidget *parent) :
    BasicSettingsDialog(sectionName, parent)
{
    connect(this, &GamePreferencesSettingsDialog::accepted, &GamePreferencesSettingsDialog::settingsNotifier, &BasicSettingsDialogNotifier::updated);
}

void GamePreferencesSettingsDialog::addPages(QStackedWidget* stackedWidget)
{
    addPage(new GamePreferencesGeneralPage(stackedWidget, "General", ":/icons/general.png"));
    addPage(new GamePreferencesPreferencesPage(stackedWidget, "Preferences", ":/icons/game.png"));
    addPage(new GamePreferencesTablePage(stackedWidget, "Table", ":/icons/table.png"));
}

void GamePreferencesSettingsDialog::buildGui()
{
    addPages(stackedWidget);

    setWindowTitle("Game Preferences - Settings");

    listWidget->setFixedWidth(120);

    setMinimumSize(440, 325);
}

void GamePreferencesSettingsDialog::load()
{    
    if (previouslyLoaded) {
        return;
    }

    addPages();

    BasicSettingsDialog::loadAddedPages(sectionName);

    removePages();

    previouslyLoaded = true;
}
