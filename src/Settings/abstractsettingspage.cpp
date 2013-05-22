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

#include "abstractsettingspage.hpp"

#include <QIcon>
#include <QString>

AbstractSettingsPage::AbstractSettingsPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath) :
    QWidget(parent), name(pageName), icon(iconPath)
{
}

AbstractSettingsPage::~AbstractSettingsPage()
{
}

const QIcon& AbstractSettingsPage::getIcon() const
{
    return icon;
}

const QString& AbstractSettingsPage::getName() const
{
    return name;
}
