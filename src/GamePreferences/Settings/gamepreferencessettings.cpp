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

#include "../../Settings/settings.hpp"
#include "../gametype.hpp"
#include "gamepreferencessettings.hpp"
#include "gamepreferencessettingsdialog.hpp"

#include <QSettings>

namespace GamePreferences {

Settings::Settings() :
    loaded(false)
{
}

Settings& Settings::getInstance()
{
    static Settings settings;
    return settings;
}

const QString& Settings::getInGameNick() const
{
    return inGameNick;
}

void Settings::setInGameNick(const QString &newNick)
{
    inGameNick = newNick;
    inGameNick.truncate(MAX_INGAMENICK_LENGTH);
}

const QVector<bool>& Settings::getDefaultPreferences() const
{
    return preferences;
}

void Settings::setDefaultPreferences(const QVector<bool> &newPreferences)
{
    preferences = newPreferences;
}

const QVector<bool>& Settings::getHiddenColumns() const
{
    return columns;
}

void Settings::setHiddenColumns(const QVector<bool> &newHiddenColumns)
{
    columns = newHiddenColumns;
}

void Settings::load()
{
    if (loaded) {
        return;
    }

    QSettings s(::Settings::FILENAME, QSettings::IniFormat);
    s.beginGroup("GamePreferences");

        s.beginGroup("General");
            setInGameNick(s.value("inGameNick", "Happy NFK Player :)").toString());
        s.endGroup();

        s.beginGroup("Preferences");
            for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
                preferences << s.value(GameType::getName(i), false).toBool();
            }
        s.endGroup();

        s.beginGroup("Table");
            columns << s.value("name", false).toBool();
            for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
                columns << s.value(GameType::getName(i), false).toBool();
            }
        s.endGroup();

    s.endGroup();

    loaded = true;
}

void Settings::save()
{
    QSettings settings(::Settings::FILENAME, QSettings::IniFormat);
    settings.beginGroup("GamePreferences");

        settings.beginGroup("General");
            settings.setValue("inGameNick", inGameNick);
        settings.endGroup();

        settings.beginGroup("Preferences");
            for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
                settings.setValue(GameType::getName(i), preferences[i]);
            }
        settings.endGroup();

        settings.beginGroup("Table");
            settings.setValue("name", columns.at(0));
            for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
                settings.setValue(GameType::getName(i), columns.at(i+1));
            }
        settings.endGroup();

    settings.endGroup();
}

void Settings::executeSettingsDialog(QWidget* parent)
{
    SettingsDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {
        save();
        emit dataChanged();
    }
}

} // namespace GamePreferences
