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
#include "planetscannersettings.hpp"
#include "settingsdialog.hpp"

#include <QSettings>

namespace PlanetScanner {

Settings::Settings() :
    loaded(false)
{
}

Settings& Settings::getInstance()
{
    static Settings settings;
    return settings;
}

void Settings::load()
{
    if (loaded) {
        return;
    }

    QSettings s(::Settings::FILENAME, QSettings::IniFormat);
    s.beginGroup("PlanetScanner");

        s.beginGroup("Game");
            gamePath = s.value("path", "").toString();
            commandlineArguments = s.value("commandlineArguments", "").toString();
        s.endGroup();

        int planetSize = s.beginReadArray("Planet");
            for (int i = 0; i < planetSize; i ++) {
                s.setArrayIndex(i);
                QString address = s.value("address", "").toString();
                bool ok;
                int port = s.value("port", "").toInt(&ok);
                if (!ok) {
                    continue;
                }
                planets << PlanetAddress{address, port};
            }
        s.endArray();

        s.beginGroup("Filter");
            s.beginGroup("Gametype");
                QHashIterator<int, QString> it(Game::getGameTypeHash());
                while (it.hasNext()) {
                    it.next();
                    gameTypeFilter[it.value()] = s.value(QString("hide%1").arg(it.value()), false).toBool();
                }
            s.endGroup();
            s.beginGroup("PlayerCount");
                hideOnFullFilter = s.value("hideFull", false).toBool();
                hideOnEmptyFilter = s.value("hideEmpty", false).toBool();
            s.endGroup();
        s.endGroup();

        s.beginGroup("Table");
            //int planetTreeColumns =
            s.beginReadArray("Column");
            for (int i = 0; i < 5; i++) {
                s.setArrayIndex(i);
                hideColumn << s.value("hide", false).toBool();
            }
            s.endArray();
            resizeOnRefreshDisabled = s.value("disableResize", false).toBool();
        s.endGroup();

        s.beginGroup("Misc");
            autoRefresh = s.value("autoRefresh", false).toBool();
            autoRefreshIntervalSec = s.value("autoRefreshIntervalSec", 30).toInt();
            pullPlayers = s.value("pullPlayers", true).toBool();
        s.endGroup();

    s.endGroup();

    loaded = true;
}

void Settings::save()
{
    QSettings s(::Settings::FILENAME, QSettings::IniFormat);
    s.beginGroup("PlanetScanner");

        s.beginGroup("Game");
            s.setValue("path", gamePath);
            s.setValue("commandlineArguments", commandlineArguments);
        s.endGroup();

        s.beginWriteArray("Planet");
            for (int i = 0; i < planets.size(); i ++) {
                s.setArrayIndex(i);
                s.setValue("address", planets.at(i).address);
                s.setValue("port", planets.at(i).port);
            }
        s.endArray();

        s.beginGroup("Filter");
            s.beginGroup("Gametype");
                QHashIterator<int, QString> it(Game::getGameTypeHash());
                while (it.hasNext()) {
                    it.next();
                    //no need to check if gameTypeFilter.contains(it.value()), because it does, for all values of `it`
                    //since we loaded all of them in load()
                    s.setValue(QString("hide%1").arg(it.value()), gameTypeFilter[it.value()]);
                }
            s.endGroup();
            s.beginGroup("PlayerCount");
                s.setValue("hideFull", hideOnFullFilter);
                s.setValue("hideEmpty", hideOnEmptyFilter);
            s.endGroup();
        s.endGroup();

        s.beginGroup("Table");
            s.beginWriteArray("Column");
            for (int i = 0; i < hideColumn.size(); i++) {
                s.setArrayIndex(i);
                s.setValue("hide", hideColumn.at(i));
            }
            s.endArray();
            s.setValue("disableResize", resizeOnRefreshDisabled);
        s.endGroup();

        s.beginGroup("Misc");
            s.setValue("autoRefresh", autoRefresh);
            s.setValue("autoRefreshIntervalSec", autoRefreshIntervalSec);
            s.setValue("pullPlayers", pullPlayers);
        s.endGroup();

    s.endGroup();
}

const QString& Settings::getGamePath() const
{
    return gamePath;
}

void Settings::setGamePath(const QString& newGamePath)
{
    gamePath = newGamePath;
}

const QString& Settings::getCommandlineArguments() const
{
    return commandlineArguments;
}

void Settings::setCommandlineArguments(const QString& newCommandlineArguments)
{
    commandlineArguments = newCommandlineArguments;
}

const QList<Settings::PlanetAddress>& Settings::getPlanets() const
{
    return planets;
}

void Settings::setPlanets(QList<PlanetAddress>& newPlanets)
{
    planets = newPlanets;
}

const QMap<QString, bool>& Settings::getGameTypeFilter() const
{
    return gameTypeFilter;
}

void Settings::setGameTypeFilter(const QMap<QString, bool>& newGameTypeFilter)
{
    gameTypeFilter = newGameTypeFilter;
}

bool Settings::getHideOnFullFilter() const
{
    return hideOnFullFilter;
}

void Settings::setHideOnFullFilter(bool value)
{
    hideOnFullFilter = value;
}

bool Settings::getHideOnEmptyFilter() const
{
    return hideOnEmptyFilter;
}

void Settings::setHideOnEmptyFilter(bool value)
{
    hideOnEmptyFilter = value;
}

const QList<bool>& Settings::getHideColumn() const
{
    return hideColumn;
}

void Settings::setHideColumn(const QList<bool>& newHideColumn)
{
    hideColumn = newHideColumn;
}

bool Settings::getResizeOnRefreshDisabled() const
{
    return resizeOnRefreshDisabled;
}

void Settings::setResizeOnRefreshDisabled(bool value)
{
    resizeOnRefreshDisabled = value;
}

bool Settings::getAutoRefresh() const
{
    return autoRefresh;
}

void Settings::setAutoRefresh(bool value)
{
    autoRefresh = value;
}

int Settings::getAutoRefreshIntervalSec() const
{
    return autoRefreshIntervalSec;
}

bool Settings::getPullPlayers() const
{
    return pullPlayers;
}

void Settings::setPullPlayers(bool value)
{
    pullPlayers = value;
}

void Settings::setAutoRefreshIntervalSec(int sec)
{
    autoRefreshIntervalSec = sec;
}

void Settings::executeSettingsDialog(QWidget* parent)
{
    SettingsDialog dialog(parent);
    if (dialog.exec() == QDialog::Accepted) {
        save();
        emit dataChanged();
    }
}

} // namespace PlanetScanner
