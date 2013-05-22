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

#include "game.hpp"
#include "planet.hpp"
#include "planetscannersettingsdialog.hpp"

#include <QCheckBox>
#include <QCursor>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPointer>
#include <QPushButton>
#include <QSpacerItem>
#include <QSpinBox>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

PlanetScannerSettingsDialog::PlanetScannerSettingsDialog(PlanetScannerDock* parent)
    : QDialog(parent)
{
    planetsModified = false;
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Planet Scanner - Settings");
    QGridLayout* dialogLayout = new QGridLayout(this);

    /* --- Menu list --- */
    QListWidget* menuList = new QListWidget(this);
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/game.png"), "Game", menuList));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/planet.png"), "Planet", menuList));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/filter.png"), "Filter", menuList));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/table.png"), "Table", menuList));
    menuList->addItem(new QListWidgetItem(QIcon(":/icons/misc.png"), "Misc", menuList));
    menuList->setIconSize(QSize(24, 24));
    menuList->setFixedWidth(100);

    QStackedWidget* stackedWidget = new QStackedWidget(this);

    /* --- Game --- */
    QWidget* gameMenu = new QWidget(stackedWidget);
    QVBoxLayout* gameMenuLayout = new QVBoxLayout(gameMenu);
    QGroupBox* gameGroup = new QGroupBox("Starting options", gameMenu);
    QGridLayout* gameGroupLayout = new QGridLayout(gameGroup);
    QLabel* gamePathLabel = new QLabel("Path to the game:", gameGroup);
    gamePathEdit = new QLineEdit(gameGroup);
    gamePathEdit->setReadOnly(true);
    gamePathEdit->setCursor(QCursor(Qt::IBeamCursor));
    gamePathEdit->setText(parent->getGamePath());
    QPushButton* gamePathButton = new QPushButton("...", gameGroup);
    gamePathButton->setFixedWidth(30);
    connect(gamePathButton, SIGNAL(clicked()), this, SLOT(getGamePath()));
    QLabel* gameCommandlineLabel = new QLabel("Command line args.:", gameGroup);
    gameCommandlineEdit = new QLineEdit(gameGroup);
    gameCommandlineEdit->setText(parent->getGameCommandlineArguments());
    gameGroupLayout->addWidget(gamePathLabel, 0, 0, 1, 1);
    gameGroupLayout->addWidget(gamePathEdit, 0, 1, 1, 1);
    gameGroupLayout->addWidget(gamePathButton, 0, 2, 1, 1);
    gameGroupLayout->addWidget(gameCommandlineLabel, 1, 0, 1, 1);
    gameGroupLayout->addWidget(gameCommandlineEdit, 1, 1, 1, 2);
    gameGroup->setLayout(gameGroupLayout);
    gameMenuLayout->addWidget(gameGroup, 0, Qt::AlignTop);
    gameMenuLayout->addStretch(0);
    gameMenu->setLayout(gameMenuLayout);

    /* --- Planet --- */
    QWidget* planetMenu = new QWidget(stackedWidget);
    QVBoxLayout* planetMenuLayout = new QVBoxLayout(planetMenu);
    QGroupBox* planetGroup = new QGroupBox("Planet options", planetMenu);
    QGridLayout* planetGroupLayout = new QGridLayout(planetGroup);
    planetTree = new QTreeWidget(planetGroup);
    planetTree->setHeaderLabels(QStringList() << "Planet" << "Port");
    QList<Planet*> planetList = parent->getPlanetList();
    for (int i = 0; i < planetList.size(); i++) {
        const Planet* planet = planetList.at(i);
        QTreeWidgetItem* planetItem = new QTreeWidgetItem(planetTree);
        planetItem->setText(0, planet->getAddress());
        planetItem->setText(1, QString("%1").arg(planet->getPort()));
        planetItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }
    connect(planetTree, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(onPlanetTreeChanged(QTreeWidgetItem*, int)));
    planetTree->setIndentation(0);
    planetTree->setColumnWidth(0, 200);
    planetTree->setColumnWidth(1, 50);
    QPushButton* planetAddButton = new QPushButton("Add", planetGroup);
    planetRemoveButton = new QPushButton("Remove", planetGroup);
    connect(planetAddButton, SIGNAL(clicked()), this, SLOT(onPlanetAdd()));
    connect(planetRemoveButton, SIGNAL(clicked()), this, SLOT(onPlanetRemove()));
    planetGroupLayout->addWidget(planetTree, 0, 0, 1, 2);
    planetGroupLayout->addWidget(planetAddButton, 1, 0, 1, 1);
    planetGroupLayout->addWidget(planetRemoveButton, 1, 1, 1, 1);
    planetGroup->setLayout(planetGroupLayout);
    planetMenuLayout->addWidget(planetGroup, 0, Qt::AlignTop);
    planetMenuLayout->addStretch(0);
    planetMenu->setLayout(planetMenuLayout);

    /* --- Filter --- */
    QWidget* filterMenu = new QWidget(stackedWidget);
    QVBoxLayout* filterMenuLayout = new QVBoxLayout(filterMenu);
    QGroupBox* filterGamepypeGroup = new QGroupBox("Hide games by gametype", filterMenu);
    QGridLayout* filterGamepypeGroupLayout = new QGridLayout(filterGamepypeGroup);
    filterGametypeDmCheckbox = new QCheckBox(Game::getNameForGametype(Game::DM), filterGamepypeGroup);    
    filterGametypeTdmCheckbox = new QCheckBox(Game::getNameForGametype(Game::TDM), filterGamepypeGroup); 
    filterGametypeCtfCheckbox = new QCheckBox(Game::getNameForGametype(Game::CTF), filterGamepypeGroup);
    filterGametypeDomCheckbox = new QCheckBox(Game::getNameForGametype(Game::DOM), filterGamepypeGroup);
    filterGametypePracCheckbox = new QCheckBox(Game::getNameForGametype(Game::PRAC), filterGamepypeGroup);
    filterGametypeRailCheckbox = new QCheckBox(Game::getNameForGametype(Game::RAIL), filterGamepypeGroup);
    filterGametypeDmCheckbox->setChecked(parent->getGameTypeFilter()[Game::DM]);
    filterGametypeTdmCheckbox->setChecked(parent->getGameTypeFilter()[Game::TDM]);
    filterGametypeCtfCheckbox->setChecked(parent->getGameTypeFilter()[Game::CTF]);
    filterGametypeDomCheckbox->setChecked(parent->getGameTypeFilter()[Game::DOM]);
    filterGametypePracCheckbox->setChecked(parent->getGameTypeFilter()[Game::PRAC]);
    filterGametypeRailCheckbox->setChecked(parent->getGameTypeFilter()[Game::RAIL]);
    filterGamepypeGroupLayout->addWidget(filterGametypeDmCheckbox, 0, 0, 1, 1);
    filterGamepypeGroupLayout->addWidget(filterGametypeTdmCheckbox, 1, 0, 1, 1);
    filterGamepypeGroupLayout->addWidget(filterGametypeCtfCheckbox, 2, 0, 1, 1);
    filterGamepypeGroupLayout->addWidget(filterGametypeDomCheckbox, 0, 1, 1, 1);
    filterGamepypeGroupLayout->addWidget(filterGametypePracCheckbox, 1, 1, 1, 1);
    filterGamepypeGroupLayout->addWidget(filterGametypeRailCheckbox, 2, 1, 1, 1);
    filterGamepypeGroup->setLayout(filterGamepypeGroupLayout);
    QGroupBox* filterPlayersGroup = new QGroupBox("Hide games by player count", filterMenu);
    QVBoxLayout* filterPlayersGroupLayout = new QVBoxLayout(filterPlayersGroup);
    filterPlayersFullCheckbox = new QCheckBox("Full games", filterPlayersGroup);
    filterPlayersEmptyCheckbox = new QCheckBox("Empty games", filterPlayersGroup);
    filterPlayersFullCheckbox->setChecked(parent->getHideOnFullFilter());
    filterPlayersEmptyCheckbox->setChecked(parent->getHideOnEmptyFilter());
    filterPlayersGroupLayout->addWidget(filterPlayersFullCheckbox);
    filterPlayersGroupLayout->addWidget(filterPlayersEmptyCheckbox);
    filterPlayersGroup->setLayout(filterPlayersGroupLayout);
    filterMenuLayout->addWidget(filterGamepypeGroup, 0, Qt::AlignTop);
    filterMenuLayout->addWidget(filterPlayersGroup, 0, Qt::AlignTop);
    filterMenuLayout->addStretch(0);
    filterMenu->setLayout(filterMenuLayout);

    /* --- Table --- */
    QWidget* tableMenu = new QWidget(stackedWidget);
    QVBoxLayout* tableMenuLayout = new QVBoxLayout(tableMenu);
    QGroupBox* tableHideColumnsGroup = new QGroupBox("Hide columns", tableMenu);
    QVBoxLayout* tableHideColumnsGroupLayout = new QVBoxLayout(tableHideColumnsGroup);
    tableHideColumnCheckbox[0] = new QCheckBox("Hostname", tableHideColumnsGroup);
    tableHideColumnCheckbox[1] = new QCheckBox("Map", tableHideColumnsGroup);
    tableHideColumnCheckbox[2] = new QCheckBox("Gametype", tableHideColumnsGroup);
    tableHideColumnCheckbox[3] = new QCheckBox("Players", tableHideColumnsGroup);
    tableHideColumnCheckbox[4] = new QCheckBox("Address", tableHideColumnsGroup);
    for (int i = 0; i < 5; i ++) {
        tableHideColumnCheckbox[i]->setChecked(parent->isColumnHidden(i));
        tableHideColumnsGroupLayout->addWidget(tableHideColumnCheckbox[i]);
    }
    tableHideColumnsGroup->setLayout(tableHideColumnsGroupLayout);
    QGroupBox* tableResizeGroup = new QGroupBox("Resize options", tableMenu);
    QVBoxLayout* tableResizeGroupLayout = new QVBoxLayout(tableResizeGroup);
    tableResizeDisableOnRefreshCheckbox = new QCheckBox("Disable resize on refresh", tableResizeGroup);
    tableResizeDisableOnRefreshCheckbox->setChecked(parent->isResizeOnRefreshDisabled());
    tableResizeGroupLayout->addWidget(tableResizeDisableOnRefreshCheckbox);
    tableResizeGroup->setLayout(tableResizeGroupLayout);
    tableMenuLayout->addWidget(tableHideColumnsGroup, 0, Qt::AlignTop);
    tableMenuLayout->addWidget(tableResizeGroup, 0, Qt::AlignTop);
    tableMenuLayout->addStretch(0);
    tableMenu->setLayout(tableMenuLayout);

    /* --- Misc --- */
    QWidget* miscMenu = new QWidget(stackedWidget);
    QVBoxLayout* miscMenuLayout = new QVBoxLayout(miscMenu);
    QGroupBox* miscGroup = new QGroupBox("Misc options", miscMenu);
    QGridLayout* miscGroupLayout = new QGridLayout(miscGroup);
    miscRefreshCheckbox = new QCheckBox("Refresh every", miscGroup);
    miscRefreshCheckbox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));
    miscRefreshSpinbox = new QSpinBox(miscGroup);
    connect(miscRefreshCheckbox, SIGNAL(stateChanged(int)), this, SLOT(onMiscRefreshCheckboxStateChange(int)));
    onMiscRefreshCheckboxStateChange(miscRefreshCheckbox->checkState());
    miscRefreshSpinbox->setMaximum(60*60*24);
    miscRefreshSpinbox->setMinimum(5);
    if (parent->isAutoRefrshActive()) {
        miscRefreshCheckbox->setChecked(true);
        miscRefreshSpinbox->setValue(parent->getAutoRefreshInterval()/1000);
    } else {
        miscRefreshSpinbox->setValue(30);
    }
    miscRefreshSpinbox->setSuffix(" sec.");
    miscRefreshSpinbox->setFixedWidth(76);
    miscGroupLayout->addWidget(miscRefreshCheckbox, 0, 0, 1, 1);
    miscGroupLayout->addWidget(miscRefreshSpinbox, 0, 1, 1, 1, Qt::AlignLeft);
    miscGroup->setLayout(miscGroupLayout);
    miscMenuLayout->addWidget(miscGroup, 0, Qt::AlignTop);
    miscMenuLayout->addStretch(0);
    miscMenu->setLayout(miscMenuLayout);

    /*TODO:Notify*/

    stackedWidget->addWidget(gameMenu);
    stackedWidget->addWidget(planetMenu);
    stackedWidget->addWidget(filterMenu);
    stackedWidget->addWidget(tableMenu);
    stackedWidget->addWidget(miscMenu);

    stackedWidget->setCurrentIndex(0);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    dialogLayout->addWidget(menuList, 0, 0, 1, 1);
    dialogLayout->addWidget(stackedWidget, 0, 1, 1, 1);
    dialogLayout->addWidget(buttonBox, 1, 0, 1, 2);
    setLayout(dialogLayout);

    connect(menuList, SIGNAL(currentRowChanged(int)), stackedWidget, SLOT(setCurrentIndex(int)));

    setMinimumSize(440, 325);
}

PlanetScannerSettingsDialog::~PlanetScannerSettingsDialog()
{
}

void PlanetScannerSettingsDialog::getGamePath()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setNameFilter("Game (*.exe)");
    if (fd.exec() == QDialog::Accepted) {
        gamePathEdit->setText(fd.selectedFiles()[0]);
    }
}

void PlanetScannerSettingsDialog::onMiscRefreshCheckboxStateChange(const int state)
{
    miscRefreshSpinbox->setEnabled(state == Qt::Checked);
}

void PlanetScannerSettingsDialog::onPlanetTreeChanged(QTreeWidgetItem* item, int column)
{
    planetsModified = true;
    if (column != 1) {
        return;
    }
    bool ok;
    item->text(1).toInt(&ok);
    if (!ok) {
        item->setText(1, "");
    }

}
void PlanetScannerSettingsDialog::onPlanetAdd()
{
    QString address = "click to edit";
    QString port = "10003";
    QTreeWidgetItem* addedItem = new QTreeWidgetItem(planetTree, QStringList() << address << port);
    addedItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    planetTree->setCurrentItem(addedItem);
    planetsModified = true;
}

void PlanetScannerSettingsDialog::onPlanetRemove()
{
    QTreeWidgetItem* currentItem = planetTree->currentItem();
    if (currentItem != 0) {
        delete currentItem;
        planetsModified = true;
    }
}
