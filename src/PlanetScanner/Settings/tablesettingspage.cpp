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

#include "planetscannersettings.hpp"
#include "tablesettingspage.hpp"

#include <QVBoxLayout>

namespace PlanetScanner {

TableSettingsPage::TableSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void TableSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    hideColumnsGroup = buildHideColumnsGroup();
    QGroupBox* tableResizeGroup = buildTableResizeGroup();

    layout->addWidget(hideColumnsGroup, 0, Qt::AlignTop);
    layout->addWidget(tableResizeGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* TableSettingsPage::buildHideColumnsGroup()
{
    QGroupBox* group = new QGroupBox("Hide columns", this);
    QVBoxLayout* layout = new QVBoxLayout(group);

    layout->addWidget(new QCheckBox("Hostname", group));
    layout->addWidget(new QCheckBox("Map", group));
    layout->addWidget(new QCheckBox("Gametype", group));
    layout->addWidget(new QCheckBox("Players", group));
    layout->addWidget(new QCheckBox("Address", group));

    return group;
}

QGroupBox* TableSettingsPage::buildTableResizeGroup()
{
    QGroupBox* group = new QGroupBox("Resize options", this);
    QVBoxLayout* layout = new QVBoxLayout(group);

    disableResizeOnRefreshCheckbox = new QCheckBox("Disable resize on refresh", group);

    layout->addWidget(disableResizeOnRefreshCheckbox);

    return group;
}

void TableSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    const QList<bool>& hideColumn = settings.getHideColumn();
    for (int i = 0; i < hideColumnsGroup->layout()->count(); i ++) {
        QCheckBox* hideColumnCheckBox = static_cast<QCheckBox*>(hideColumnsGroup->layout()->itemAt(i)->widget());
        hideColumnCheckBox->setChecked(hideColumn.at(i));
    }

    disableResizeOnRefreshCheckbox->setChecked(settings.getResizeOnRefreshDisabled());
}

void TableSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    QList<bool> hideColumn;
    for (int i = 0; i < hideColumnsGroup->layout()->count(); i ++) {
        QCheckBox* hideColumnCheckBox = static_cast<QCheckBox*>(hideColumnsGroup->layout()->itemAt(i)->widget());
        hideColumn << hideColumnCheckBox->isChecked();
    }
    settings.setHideColumn(hideColumn);

    settings.setResizeOnRefreshDisabled(disableResizeOnRefreshCheckbox->isChecked());
}

} // namespace PlanetScanner
