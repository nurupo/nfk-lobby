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

#include "gamesettingspage.hpp"
#include "planetscannersettings.hpp"

#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace PlanetScanner {

GameSettingsPage::GameSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void GameSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* gameGroup = buildGameGroup();

    layout->addWidget(gameGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* GameSettingsPage::buildGameGroup()
{
    QGroupBox* group = new QGroupBox("Starting options", this);
    QGridLayout* layout = new QGridLayout(group);

    QLabel* gamePathLabel = new QLabel("Path to the game:", group);

    gamePathEdit = new QLineEdit(group);
    gamePathEdit->setReadOnly(true);
    gamePathEdit->setCursor(QCursor(Qt::IBeamCursor));

    QPushButton* gamePathButton = new QPushButton("...", group);
    gamePathButton->setFixedWidth(30);
    connect(gamePathButton, SIGNAL(clicked()), this, SLOT(setGamePath()));

    QLabel* commandlineArgumentsLabel = new QLabel("Command line args.:", group);

    gameCommandlineArgumentsEdit = new QLineEdit(group);

    layout->addWidget(gamePathLabel,                0, 0, 1, 1);
    layout->addWidget(gamePathEdit,                 0, 1, 1, 1);
    layout->addWidget(gamePathButton,               0, 2, 1, 1);
    layout->addWidget(commandlineArgumentsLabel,    1, 0, 1, 1);
    layout->addWidget(gameCommandlineArgumentsEdit, 1, 1, 1, 2);

    return group;
}

void GameSettingsPage::setGamePath()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setNameFilter("Game (*.exe)");
    if (fd.exec() == QDialog::Accepted) {
        gamePathEdit->setText(fd.selectedFiles()[0]);
    }
}

void GameSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    gamePathEdit->setText(settings.getGamePath());
    gameCommandlineArgumentsEdit->setText(settings.getCommandlineArguments());
}

void GameSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    settings.setGamePath(gamePathEdit->text());
    settings.setCommandlineArguments(gameCommandlineArgumentsEdit->text());
}

} // namespace PlanetScanner
