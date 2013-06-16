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

#ifndef TABLESETTINGSPAGE_HPP
#define TABLESETTINGSPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"

#include <QCheckBox>
#include <QGroupBox>

namespace PlanetScanner {

class TableSettingsPage : public AbstractSettingsPage
{
public:
    TableSettingsPage(QWidget* parent);
    void applyChanges();
    void buildGui();
    void setGui();

private:
    QGroupBox* buildHideColumnsGroup();
    QGroupBox* buildTableResizeGroup();

    QGroupBox* hideColumnsGroup;
    QCheckBox* disableResizeOnRefreshCheckbox;

};

} // namespace PlanetScanner

#endif // TABLESETTINGSPAGE_HPP
