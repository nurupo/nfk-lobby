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
#include "planetsettingspage.hpp"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace PlanetScanner {

PlanetSettingsPage::PlanetSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void PlanetSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* gameGroup = buildPlanetGroup();

    layout->addWidget(gameGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* PlanetSettingsPage::buildPlanetGroup()
{
    QGroupBox* group = new QGroupBox("Planet options", this);
    QGridLayout* layout = new QGridLayout(group);

    planetTreeView = new QTreeView(group);
    planetTreeView->setIndentation(0);

    planetTreeModel = new QStandardItemModel(planetTreeView);
    planetTreeModel->setHorizontalHeaderLabels(QStringList() << "Address" << "Port");
    connect(planetTreeModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(onPlanetTreeItemChanged(QStandardItem*)));

    planetTreeView->setModel(planetTreeModel);

    planetTreeView->setColumnWidth(0, 200);
    planetTreeView->setColumnWidth(1, 50);


    QPushButton* planetAddButton = new QPushButton("Add", group);
    connect(planetAddButton, SIGNAL(clicked()), this, SLOT(onPlanetAdd()));

    QPushButton* planetRemoveButton = new QPushButton("Remove", group);
    connect(planetRemoveButton, SIGNAL(clicked()), this, SLOT(onPlanetRemove()));

    layout->addWidget(planetTreeView,       0, 0, 1, 2);
    layout->addWidget(planetAddButton,      1, 0, 1, 1);
    layout->addWidget(planetRemoveButton,   1, 1, 1, 1);

    return group;
}

void PlanetSettingsPage::onPlanetTreeItemChanged(QStandardItem* item)
{
    if (item->column() != 1) {
        return;
    }

    bool ok;
    item->text().toInt(&ok);
    if (!ok) {
        item->setText("");
    }
}

void PlanetSettingsPage::onPlanetAdd()
{
    QStandardItem* address = new QStandardItem("double click to edit");
    QStandardItem* port = new QStandardItem("10003");
    planetTreeModel->invisibleRootItem()->appendRow(QList<QStandardItem*>() << address << port);
    planetTreeView->selectionModel()->clear();
    planetTreeView->selectionModel()->setCurrentIndex(address->index(), QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
}

void PlanetSettingsPage::onPlanetRemove()
{
    if (!planetTreeView->selectionModel()->hasSelection()) {
        return;
    }

    QModelIndex currentIndex = planetTreeView->selectionModel()->currentIndex();
    qDeleteAll(planetTreeModel->takeRow(currentIndex.row()));
}

void PlanetSettingsPage::setGui()
{
    const QList<Settings::PlanetAddress> planets = Settings::getInstance().getPlanets();
    for (const Settings::PlanetAddress& planet : planets) {
        QStandardItem* address = new QStandardItem(planet.address);
        QStandardItem* port = new QStandardItem(QString::number(planet.port));
        planetTreeModel->invisibleRootItem()->appendRow(QList<QStandardItem*>() << address << port);
    }
}

void PlanetSettingsPage::applyChanges()
{
    QList<Settings::PlanetAddress> planets;
    for (int i = 0; i < planetTreeModel->rowCount(); i ++) {
        QString address = planetTreeModel->item(i, 0)->text();
        //no need to check if conversion to int went well, because it's done in onPlanetTreeItemChanged()
        int port = planetTreeModel->item(i, 1)->text().toInt();
        planets << Settings::PlanetAddress{address, port};
    }
    Settings::getInstance().setPlanets(planets);
}

} // namespace PlanetScanner
