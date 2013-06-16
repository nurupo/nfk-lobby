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

#include "miscsettingspage.hpp"
#include "planetscannersettings.hpp"

#include <QVBoxLayout>

namespace PlanetScanner {

MiscSettingsPage::MiscSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void MiscSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* miscGroup = buildMiscGroup();

    layout->addWidget(miscGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* MiscSettingsPage::buildMiscGroup()
{
    QGroupBox* group = new QGroupBox("Misc options", this);
    QGridLayout* layout = new QGridLayout(group);

    refreshCheckBox = new QCheckBox("Refresh every", group);
    refreshCheckBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

    refreshSpinBox = new QSpinBox(group);
    refreshSpinBox->setMaximum(60*60*24);
    refreshSpinBox->setMinimum(5);
    refreshSpinBox->setSuffix(" sec.");
    refreshSpinBox->setFixedWidth(76);

    layout->addWidget(refreshCheckBox,  0, 0, 1, 1);
    layout->addWidget(refreshSpinBox,   0, 1, 1, 1, Qt::AlignLeft);

    return group;
}

void MiscSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    refreshCheckBox->setChecked(settings.getAutoRefresh());
    refreshSpinBox->setValue(settings.getAutoRefreshIntervalSec());
}

void MiscSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    settings.setAutoRefresh(refreshCheckBox->isChecked());
    settings.setAutoRefreshIntervalSec(refreshSpinBox->value());
}

} // namespace PlanetScanner
