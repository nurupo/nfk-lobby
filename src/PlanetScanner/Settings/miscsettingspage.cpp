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

#include "../statisticswebsite.hpp"
#include "miscsettingspage.hpp"
#include "planetscannersettings.hpp"

#include <QLabel>
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
    QVBoxLayout* layout = new QVBoxLayout(group);

    QHBoxLayout* refreshLayout = new QHBoxLayout(group);

    refreshCheckBox = new QCheckBox("Refresh every", group);
    refreshCheckBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

    refreshSpinBox = new QSpinBox(group);
    refreshSpinBox->setMaximum(60*60*24);
    refreshSpinBox->setMinimum(5);
    refreshSpinBox->setSuffix(" sec.");
    refreshSpinBox->setFixedWidth(76);

    refreshLayout->addWidget(refreshCheckBox);
    refreshLayout->addWidget(refreshSpinBox, 0, Qt::AlignLeft);

    QHBoxLayout* playersLayout = new QHBoxLayout(group);
    playersLayout->setSpacing(0);

    pullPlayersCheckBox = new QCheckBox("Pull players from", group);

    QLabel* webSiteUrlLabel = new QLabel(QString("<a href=\"%1\">%1</a>").arg(StatisticsWebSite::baseUrl()), group);
    webSiteUrlLabel->setTextFormat(Qt::RichText);
    webSiteUrlLabel->setTextInteractionFlags(webSiteUrlLabel->textInteractionFlags() | Qt::LinksAccessibleByMouse);
    webSiteUrlLabel->setOpenExternalLinks(true);

    playersLayout->addWidget(pullPlayersCheckBox);
    playersLayout->addWidget(webSiteUrlLabel, 10, Qt::AlignLeft);

    layout->addLayout(refreshLayout);
    layout->addLayout(playersLayout);

    return group;
}

void MiscSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    refreshCheckBox->setChecked(settings.getAutoRefresh());
    refreshSpinBox->setValue(settings.getAutoRefreshIntervalSec());
    pullPlayersCheckBox->setChecked(settings.getPullPlayers());
}

void MiscSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    settings.setAutoRefresh(refreshCheckBox->isChecked());
    settings.setAutoRefreshIntervalSec(refreshSpinBox->value());
    settings.setPullPlayers(pullPlayersCheckBox->isChecked());
}

} // namespace PlanetScanner
