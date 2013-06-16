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
#include "gamesettingspage.hpp"
#include "miscsettingspage.hpp"
#include "planetsettingspage.hpp"
#include "settingsdialog.hpp"
#include "tablesettingspage.hpp"

namespace PlanetScanner {

SettingsDialog::SettingsDialog(QWidget* parent) :
    BasicSettingsDialog(parent)
{
    setWindowTitle("PlanetScanner - Settings");

    addPage(":/icons/game.png",     "Game",     new GameSettingsPage(this));
    addPage(":/icons/planet.png",   "Planet",   new PlanetSettingsPage(this));
    addPage(":/icons/filter.png",   "Filter",   new FilterSettingsPage(this));
    addPage(":/icons/table.png",    "Table",    new TableSettingsPage(this));
    addPage(":/icons/misc.png",     "Misc",     new MiscSettingsPage(this));

    listWidget->setFixedWidth(100);
    setMinimumSize(440, 325);
}

} // namespace PlanetScanner
