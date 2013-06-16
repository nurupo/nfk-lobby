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
#include "gamepreferencespreferencessettingspage.hpp"
#include "gamepreferencessettingsdialog.hpp"
#include "gamepreferencestablesettingspage.hpp"

namespace GamePreferences {

SettingsDialog::SettingsDialog(QWidget* parent) :
    BasicSettingsDialog(parent)
{
    setWindowTitle("Game Preferences - Settings");

    addPage(":/icons/general.png",  "General",      new GeneralSettingsPage(this));
    addPage(":/icons/game.png",     "Preferences",  new PreferencesSettingsPage(this));
    addPage(":/icons/table.png",    "Table",        new TableSettingsPage(this));

    listWidget->setFixedWidth(120);
    setMinimumSize(440, 325);
}

} // namespace GamePreferences
