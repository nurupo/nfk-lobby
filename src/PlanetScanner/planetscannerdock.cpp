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

#include "planetscannerdock.hpp"
#include "planetscannersettingsdialog.hpp"

#include <QAction>
#include <QClipboard>
#include <QFile>
#include <QGuiApplication>
#include <QHeaderView>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QToolBar>
#include <QTreeWidget>
#include <QTreeWidgetItemIterator>
#include <QVBoxLayout>

PlanetScannerDock::PlanetScannerDock(QWidget* parent) : QDockWidget(parent)
{
    setWindowTitle("Planet Scanner");
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

    QMainWindow* window = new QMainWindow(0);

    QToolBar* toolbar = new QToolBar(window);
    toolbar->setIconSize(QSize(24, 24));
    toolbar->setFloatable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    window->addToolBar(toolbar);

    QAction* refreshAction = toolbar->addAction(QIcon(":/icons/refresh.png"), "Refresh");
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refreshPlanets()));
    QAction* settingsAction = toolbar->addAction(QIcon(":/icons/settings.png"), "Settings");
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));

    planetsTree = new QTreeWidget(window);
    planetsTree->setMinimumHeight(10);
    planetsTree->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    planetsTree->setColumnCount(5);
    planetsTree->setHeaderLabels(QStringList() << "Hostname" << "Map" << "Gametype" << "Players" << "Address");
    planetsTree->setSortingEnabled(true);
    planetsTree->sortItems(0, Qt::AscendingOrder);
    planetsTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(planetsTree, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    QAction* connectAction = new QAction("Connect", planetsTree);
    QAction* connectAsSpectatorAction = new QAction("Connect as spectator", planetsTree);
    QAction* copyAction = new QAction("Copy", planetsTree);
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectSelected()));
    connect(connectAsSpectatorAction, SIGNAL(triggered()), this, SLOT(connectAsSpectatorSelected()));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copySelected()));
    gameContextMenu = new QMenu(planetsTree);
    planetContextMenu = new QMenu(planetsTree);
    gameContextMenu->addActions(QList<QAction*>() << connectAction << connectAsSpectatorAction << copyAction);
    planetContextMenu->addActions(QList<QAction*>() << copyAction);

    window->setCentralWidget(planetsTree);
    window->setParent(this);
    setWidget(window);

    game = new QProcess(this);

    autoRefreshTimer = new QTimer(window);
    connect(autoRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshPlanets()));

    /*for (int i = 0; i < 8; i ++)
    {
        gameTypeFilter[i] = 0;
    }
    hideOnFullFilter = 0;
    hideOnEmptyFilter = 0;
    resizeOnRefreshDisabled = 0;
    gamePath = "";
    gameCommandlineArguments = "";
    addPlanet(new Planet("nfkplanet.pro2d.ru", 10003));
    addPlanet(new Planet("planet.needforkill.com", 10003));
    */
    contextMenuShown = false;
    resizeOnRefreshDisabled = false;
    load();
}

PlanetScannerDock::~PlanetScannerDock()
{
    while(planetList.size() != 0) {
        delete planetList.takeAt(0);
    }
}

QTreeWidgetItem* PlanetScannerDock::getPlanetTreeWidgetItem(const Planet &planet)
{
    QString searchingText = QString("%1:%2").arg(planet.getAddress()).arg(planet.getPort());
    for (int i = 0; i < planetsTree->topLevelItemCount(); i ++) {
        if (!planetsTree->topLevelItem(i)->text(0).compare(searchingText, Qt::CaseInsensitive)) {
            return planetsTree->topLevelItem(i);
        }
    }
    return 0;
}

void PlanetScannerDock::refreshPlanets()
{
    if (contextMenuShown) {
        return;
    }
    for (int i = 0; i < planetList.size(); i ++) {
        planetList.at(i)->requestInfo();
    }
}

void PlanetScannerDock::addGame(const Planet &planet, const QList<Game> &games)
{
    QList<QTreeWidgetItem*> newGameItems;
    QTreeWidgetItem *planetItem = getPlanetTreeWidgetItem(planet);
    while (planetItem->childCount()) {
        delete planetItem->child(0);
    }
    for (int i = 0; i < games.length(); i ++) {
        if (gameTypeFilter[games.at(i).getGametype()]) {
            continue;
        }
        if (hideOnFullFilter && games.at(i).getCurrentPlayers() == games.at(i).getMaxPlayers()) {
            continue;
        }
        if (hideOnEmptyFilter && games.at(i).getCurrentPlayers() == 0) {
            continue;
        }
        QTreeWidgetItem *gameItem = new QTreeWidgetItem(planetItem);
        gameItem->setText(0, games.at(i).getCleanedHostname());
        gameItem->setText(1, games.at(i).getMap());
        gameItem->setText(2, Game::getNameForGametype(games.at(i).getGametype()));
        gameItem->setText(3, QString("%1/%2").arg(games.at(i).getCurrentPlayers()).arg(games.at(i).getMaxPlayers()));
        gameItem->setText(4, QString("%1:%2").arg(games.at(i).getIp()).arg(games.at(i).getPort()));
        newGameItems << gameItem;
    }
    planetsTree->expandAll();
    resizeColumnsToContents();
    planetItem->addChildren(newGameItems);
}

void inline PlanetScannerDock::resizeColumnsToContents()
{
    if (!resizeOnRefreshDisabled) {
        planetsTree->header()->resizeSections(QHeaderView::ResizeToContents);
    }
}

void PlanetScannerDock::setPlanetConnectionError(const Planet &planet, QAbstractSocket::SocketError socketError)
{
    QString errorEnum;
    QDebug(&errorEnum).nospace() << socketError;
    QString error = QString("Error: [%1]").arg(errorEnum);
    QTreeWidgetItem *planetItem = getPlanetTreeWidgetItem(planet);
    //no need to reset the error and resize the treeview if the error is the same.
    //moreover, resizing treeview will just annoy the user.
    if (planetItem->text(1).compare(error)) {
        planetItem->setText(1, error);
        if (!resizeOnRefreshDisabled) {
            planetsTree->resizeColumnToContents(1);
        }
    }
}

void PlanetScannerDock::clearPlanetConnectionError(const Planet &planet)
{
    QTreeWidgetItem *planetItem = getPlanetTreeWidgetItem(planet);
    planetItem->setText(1, "");
    resizeColumnsToContents();
}

void PlanetScannerDock::showSettingsDialog()
{
    PlanetScannerSettingsDialog settings(this);
    if (settings.exec() == QDialog::Rejected) {
        return;
    }
    if (settings.isPlanetListModified()) {
        foreach (Planet* planet, planetList) {
            bool found = false;
            QTreeWidgetItemIterator it(settings.planetTree);
            while (*it) {
                if (!planet->getAddress().compare((*it)->text(0))
                        && !QString::number(planet->getPort()).compare((*it)->text(1))) {
                    delete (*it);
                    found = true;
                    break;
                }
                it++;
            }
            if (!found) {
                removePlanet(planet);
                delete planet;
            }
        }
        QTreeWidgetItemIterator it(settings.planetTree);
        while (*it) {
            /*don't add entries with non numerical values for port*/
            bool ok;
            int port = (*it)->text(1).toInt(&ok);
            if (ok) {
                addPlanet(new Planet((*it)->text(0), port));
            }
            it++;
        }
    }
    gameTypeFilter[Game::DM] = settings.filterGametypeDmCheckbox->isChecked();
    gameTypeFilter[Game::TDM] = settings.filterGametypeTdmCheckbox->isChecked();
    gameTypeFilter[Game::CTF] = settings.filterGametypeCtfCheckbox->isChecked();
    gameTypeFilter[Game::RAIL] = settings.filterGametypeRailCheckbox->isChecked();
    gameTypeFilter[Game::PRAC] = settings.filterGametypePracCheckbox->isChecked();
    gameTypeFilter[Game::DOM] = settings.filterGametypeDomCheckbox->isChecked();
    hideOnFullFilter = settings.filterPlayersFullCheckbox->isChecked();
    hideOnEmptyFilter = settings.filterPlayersEmptyCheckbox->isChecked();
    if (settings.miscRefreshCheckbox->isChecked()) {
        if (settings.miscRefreshSpinbox->value()*1000 != getAutoRefreshInterval()) {
            autoRefreshTimer->start(settings.miscRefreshSpinbox->value()*1000);
        }
    } else {
        if (autoRefreshTimer->isActive()) {
            autoRefreshTimer->stop();
        }
    }
    for (int i = 0; i < 5; i ++) {
        bool wasHidden = planetsTree->isColumnHidden(i);
        planetsTree->setColumnHidden(i, settings.tableHideColumnCheckbox[i]->isChecked());
        if (!settings.tableHideColumnCheckbox[i]->isChecked()
                && wasHidden
                && i != 0) {
            planetsTree->resizeColumnToContents(i-1);
        }
    }
    resizeOnRefreshDisabled = settings.tableResizeDisableOnRefreshCheckbox->isChecked();
    gamePath = settings.gamePathEdit->text();
    gameCommandlineArguments = settings.gameCommandlineEdit->text();
    refreshPlanets();
    save();
}

void PlanetScannerDock::addPlanet(Planet* planet)
{
    connect(planet, SIGNAL(gameInfoRecieved(const Planet &, const QList<Game> &)), this, SLOT(addGame(const Planet &, const QList<Game> &)));
    connect(planet, SIGNAL(error(const Planet &, QAbstractSocket::SocketError)), this, SLOT(setPlanetConnectionError(const Planet &, QAbstractSocket::SocketError)));
    connect(planet, SIGNAL(errorCeared(const Planet &)), this, SLOT(clearPlanetConnectionError(const Planet &)));
    planetList << planet;
    QTreeWidgetItem *planetItem = new QTreeWidgetItem(planetsTree);
    planetItem->setText(0, QString("%1:%2").arg(planet->getAddress()).arg(planet->getPort()));
    resizeColumnsToContents();
}

void PlanetScannerDock::removePlanet(Planet* planet)
{
    planetList.removeAt(planetList.indexOf(planet));
    delete getPlanetTreeWidgetItem(*planet);
}

void PlanetScannerDock::startGame(const QString &additionalCommandlineArguments)
{
    if (planetsTree->selectedItems()[0]->parent() == 0) {
        error("Selected planet instead of a game.");
        return;
    }
    QString arguments(gameCommandlineArguments);
    arguments += " " + additionalCommandlineArguments;
    if (game->state() != QProcess::NotRunning) {
        game->kill();
        game->waitForFinished();
    }
    QString basenfkPath = getBasenfkPath();
    QDir basenfk(basenfkPath);
    if (!basenfk.exists()) {
        error("Couldn't find:\n\"" + basenfkPath + "\"");
        return;
    }
    QFile file(basenfkPath + "nfksetup.ini");
    if (!file.exists()) {
        error("Couldn't find:\n\"" + basenfkPath + "nfksetup.ini" + "\"");
        return;
    }
    QSettings nfksetup(basenfkPath + "nfksetup.ini", QSettings::IniFormat);
    nfksetup.beginGroup("nfkplanet");
    QString oldAddress = nfksetup.value("address").toString();
    QString oldPort = nfksetup.value("port").toString();
    QString newAddress = planetsTree->selectedItems()[0]->parent()->text(0).split(':')[0];
    QString newPort = planetsTree->selectedItems()[0]->parent()->text(0).split(':')[1];
    nfksetup.setValue("address", newAddress);
    nfksetup.setValue("port", newPort);
    game->start(gamePath, arguments.split(" "));
    game->waitForStarted(10000);
    nfksetup.setValue("address", oldAddress);
    nfksetup.setValue("port", oldPort);
}

void PlanetScannerDock::connectSelected()
{
    startGame("+connect " + planetsTree->selectedItems()[0]->text(4));
}

void PlanetScannerDock::connectAsSpectatorSelected()
{
    QString configPath = getBasenfkPath()+ "spec.cfg";
    QFile config(configPath);
    if (!config.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
        error("Couldn't open:\n\"" + configPath + "\"");
        return;
    }
    QString configContent = "spectator 1\n\recho spectator on\n\rconnect " + planetsTree->selectedItems()[0]->text(4);
    config.write(configContent.toLatin1());
    startGame("+exec spec ");
}

QString PlanetScannerDock::getBasenfkPath()
{
    return gamePath.left(gamePath.lastIndexOf('/')) + "/basenfk/";
}

void PlanetScannerDock::copySelected()
{
    QString copy;
    for (int i = 0; i < planetsTree->columnCount(); i++) {
        if (planetsTree->isColumnHidden(i)) {
            continue;
        }
        QString column = planetsTree->selectedItems()[0]->text(i);
        if (!column.compare("")) {
            continue;
        }
        copy += column;
        if (i < planetsTree->columnCount()-1) {
            if (planetsTree->selectedItems()[0]->text(i+1).compare("") && !planetsTree->isColumnHidden(i+1)) {
                copy += "|";
            }
        }
    }
    QGuiApplication::clipboard()->setText(copy);
}

void PlanetScannerDock::save()
{
    QSettings set("settings.ini", QSettings::IniFormat);
    set.beginGroup("PlanetScanner");
    set.beginGroup("Game");
    set.setValue("path", gamePath);
    set.setValue("commandlineArguments", gameCommandlineArguments);
    set.endGroup();
    set.beginWriteArray("Planet");
    for (int i = 0; i < planetList.size(); i ++) {
        set.setArrayIndex(i);
        set.setValue("address", planetList.at(i)->getAddress());
        set.setValue("port", planetList.at(i)->getPort());
    }
    set.endArray();
    set.beginGroup("Filter");
    set.beginWriteArray("Gametype");
    for (int i = 0; i < 8; i ++) {
        set.setArrayIndex(i);
        set.setValue("hide", gameTypeFilter[i]);
    }
    set.endArray();
    set.beginGroup("PlayerCount");
    set.setValue("hideFull", hideOnFullFilter);
    set.setValue("hideEmpty", hideOnEmptyFilter);
    set.endGroup();
    set.endGroup();
    set.beginGroup("Table");
    set.beginWriteArray("Column");
    for (int i = 0; i < planetsTree->columnCount(); i++) {
        set.setArrayIndex(i);
        set.setValue("hide", isColumnHidden(i));
    }
    set.endArray();
    set.setValue("disableResize", resizeOnRefreshDisabled);
    set.endGroup();
    set.beginGroup("Misc");
    set.setValue("autoRefresh", autoRefreshTimer->isActive());
    set.setValue("autoRefreshInterval", autoRefreshTimer->interval()/1000);
    set.endGroup();
    set.endGroup();
}

void PlanetScannerDock::load()
{
    QSettings set("settings.ini", QSettings::IniFormat);
    set.beginGroup("PlanetScanner");
    set.beginGroup("Game");
    gamePath = set.value("path", "").toString();
    gameCommandlineArguments = set.value("commandlineArguments", "").toString();
    set.endGroup();
    int planetSize = set.beginReadArray("Planet");
    for (int i = 0; i < planetSize; i ++) {
        set.setArrayIndex(i);
        QString address = set.value("address", "").toString();
        bool ok;
        int port = set.value("port", "").toInt(&ok);
        if (!ok) {
            continue;
        }
        addPlanet(new Planet(address, port));
    }
    set.endArray();
    set.beginGroup("Filter");
    set.beginReadArray("Gametype");
    for (int i = 0; i < 8; i ++) {
        set.setArrayIndex(i);
        gameTypeFilter[i] = set.value("hide", false).toBool();
    }
    set.endArray();
    set.beginGroup("PlayerCount");
    hideOnFullFilter = set.value("hideFull", false).toBool();
    hideOnEmptyFilter = set.value("hideEmpty", false).toBool();
    set.endGroup();
    set.endGroup();
    set.beginGroup("Table");
    int planetTreeColumns = set.beginReadArray("Column");
    for (int i = 0; i < planetTreeColumns; i++) {
        set.setArrayIndex(i);
        if (set.value("hide", false).toBool()) {
            planetsTree->hideColumn(i);
        }
    }
    set.endArray();
    resizeOnRefreshDisabled = set.value("disableResize", false).toBool();
    set.endGroup();
    set.beginGroup("Misc");
    if (set.value("autoRefresh", false).toBool()) {
        autoRefreshTimer->start(set.value("autoRefreshInterval", 30).toInt()*1000);
    }
    set.endGroup();
    set.endGroup();
}

void PlanetScannerDock::error(const QString &errorText)
{
    QMessageBox::critical(this, QString("Error"), errorText, QMessageBox::Ok);
}

void PlanetScannerDock::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = planetsTree->viewport()->mapToGlobal(pos);
    globalPos.setX(globalPos.x() + 1);
    QList<QTreeWidgetItem*> selectedItems = planetsTree->selectedItems();
    if (selectedItems.size() != 1) {
        return;
    }
    QTreeWidgetItem* selectedItem = selectedItems[0];
    contextMenuShown = true;
    if (!selectedItem->text(4).compare("")) {
        planetContextMenu->exec(globalPos);
    } else {
        gameContextMenu->exec(globalPos);
    }
    contextMenuShown = false;
}
