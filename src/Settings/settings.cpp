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

#include "settings.hpp"

#include <QSettings>

const QString Settings::FILENAME = "settings.ini";

Settings::Settings()
{
}

void Settings::saveWindow(const QMainWindow* window)
{
    QSettings settings(FILENAME, QSettings::IniFormat);
    settings.beginGroup(window->objectName());
    settings.setValue("geometry", window->saveGeometry());
    settings.setValue("state", window->saveState());
    settings.endGroup();
}

void Settings::loadWindow(QMainWindow* window)
{
    QSettings settings(FILENAME, QSettings::IniFormat);
    settings.beginGroup(window->objectName());
    window->restoreGeometry(settings.value("geometry").toByteArray());
    window->restoreState(settings.value("state").toByteArray());
    settings.endGroup();
}
