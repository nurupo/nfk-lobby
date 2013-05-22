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

#ifndef GAMEPREFERENCESSETTINGSDIALOG_HPP
#define GAMEPREFERENCESSETTINGSDIALOG_HPP

#include "Settings/basicsettingsdialog.hpp"

class GamePreferencesSettingsDialog : public BasicSettingsDialog
{
public:
    explicit GamePreferencesSettingsDialog(QWidget* parent);
    static void load();

    static BasicSettingsDialogNotifier settingsNotifier;

private:
    void buildGui();
    static void addPages(QStackedWidget* stackedWidget = 0);

    static bool previouslyLoaded;
    static QString sectionName;

};


#endif // GAMEPREFERENCESSETTINGSDIALOG_HPP

