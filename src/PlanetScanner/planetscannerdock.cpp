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
    const QString dockName = "Planet Scanner";
    setObjectName(dockName);
    setWindowTitle(dockName);
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

    planetTreeView = new QTreeView(window);
    planetTreeView->setMinimumHeight(10);
    planetTreeModel = new PlanetTreeModel(planetTreeView);
    planetTreeProxyModel = new PlanetTreeSortFilterProxyModel(planetTreeModel);
    planetTreeProxyModel->setSourceModel(planetTreeModel);
    planetTreeView->setModel(planetTreeProxyModel);
    planetTreeModel->setHorizontalHeaderLabels(QStringList() << "Hostname" << "Map" << "Gametype" << "Players" << "Address");
    planetTreeView->setSortingEnabled(true);
    planetTreeView->sortByColumn(0, Qt::AscendingOrder);
    planetTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    planetTreeView->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(planetTreeView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    QAction* connectAction = new QAction("Connect", planetTreeView);
    QAction* connectAsSpectatorAction = new QAction("Connect as spectator", planetTreeView);
    QAction* copyAction = new QAction("Copy", planetTreeView);
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectSelected()));
    connect(connectAsSpectatorAction, SIGNAL(triggered()), this, SLOT(connectAsSpectatorSelected()));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copySelected()));
    gameContextMenu = new QMenu(planetTreeView);
    planetContextMenu = new QMenu(planetTreeView);
    gameContextMenu->addActions(QList<QAction*>() << connectAction << connectAsSpectatorAction << copyAction);
    planetContextMenu->addActions(QList<QAction*>() << copyAction);

    window->setCentralWidget(planetTreeView);
    window->setParent(this);
    setWidget(window);

    game = new QProcess(this);

    autoRefreshTimer = new QTimer(window);
    connect(autoRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshPlanets()));

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

QStandardItem* PlanetScannerDock::getPlanetTreeWidgetItem(const Planet &planet)
{
    const QString searchingText = QString("%1:%2").arg(planet.getAddress()).arg(planet.getPort());
    QList<QStandardItem*> items = planetTreeModel->findItems(searchingText, Qt::MatchExactly, 0);
    // we assume that there are no duplicates
    for (QStandardItem* item : items) {
        if (item->parent() == 0) {
            return item;
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
    //saving selction
    QModelIndex currentIndex = planetTreeView->selectionModel()->currentIndex();

    QStandardItem* planetItem = getPlanetTreeWidgetItem(planet);
    while (planetItem->rowCount()) {
        qDeleteAll(planetItem->takeRow(0));
    }
    for (const Game &game : games) {
        QList<QStandardItem*> gameItems;
        gameItems << new QStandardItem(game.getCleanedHostname());
        gameItems << new QStandardItem(game.getMap());
        gameItems << new QStandardItem(Game::getNameForGametype(game.getGametype()));
        gameItems << new QStandardItem(QString("%1/%2").arg(game.getCurrentPlayers()).arg(game.getMaxPlayers()));
        gameItems << new QStandardItem(QString("%1:%2").arg(game.getIp()).arg(game.getPort()));
        planetItem->appendRow(gameItems);
    }

    //restoring selection
    planetTreeView->selectionModel()->clear();
    planetTreeView->selectionModel()->setCurrentIndex(currentIndex, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);

    planetTreeView->expandAll();
    resizeColumnsToContents();
}

void inline PlanetScannerDock::resizeColumnsToContents()
{
    if (!resizeOnRefreshDisabled) {
        planetTreeView->header()->resizeSections(QHeaderView::ResizeToContents);
    }
}

void PlanetScannerDock::setPlanetConnectionError(const Planet &planet, QAbstractSocket::SocketError socketError)
{
    QString errorEnum;
    QDebug(&errorEnum).nospace() << socketError;
    QString error = QString("Error: [%1]").arg(errorEnum);
    QStandardItem* planetItem = getPlanetTreeWidgetItem(planet);
    QStandardItem* errorItem = planetTreeModel->item(planetItem->row(), 1);
    //no need to reset the error and resize the treeview if the error is the same
    if (errorItem->text().compare(error)) {
        errorItem->setText(error);
        if (!resizeOnRefreshDisabled) {
            planetTreeView->resizeColumnToContents(1);
        }
    }
}

void PlanetScannerDock::clearPlanetConnectionError(const Planet &planet)
{
    QStandardItem* planetItem = getPlanetTreeWidgetItem(planet);
    QStandardItem* errorItem = planetTreeModel->item(planetItem->row(), 1);
    errorItem->setText("");
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
            //don't add entries with non numerical values for port
            bool ok;
            int port = (*it)->text(1).toInt(&ok);
            if (ok) {
                addPlanet(new Planet((*it)->text(0), port));
            }
            it++;
        }
    }
    planetTreeProxyModel->setGameTypeFilter(Game::DM, settings.filterGametypeDmCheckbox->isChecked());
    planetTreeProxyModel->setGameTypeFilter(Game::TDM, settings.filterGametypeTdmCheckbox->isChecked());
    planetTreeProxyModel->setGameTypeFilter(Game::CTF, settings.filterGametypeCtfCheckbox->isChecked());
    planetTreeProxyModel->setGameTypeFilter(Game::RAIL, settings.filterGametypeRailCheckbox->isChecked());
    planetTreeProxyModel->setGameTypeFilter(Game::PRAC, settings.filterGametypePracCheckbox->isChecked());
    planetTreeProxyModel->setGameTypeFilter(Game::DOM, settings.filterGametypeDomCheckbox->isChecked());
    planetTreeProxyModel->setHideOnFullFilter(settings.filterPlayersFullCheckbox->isChecked());
    planetTreeProxyModel->setHideOnEmptyFilter(settings.filterPlayersEmptyCheckbox->isChecked());
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
        bool wasHidden = planetTreeView->isColumnHidden(i);
        planetTreeView->setColumnHidden(i, settings.tableHideColumnCheckbox[i]->isChecked());
        if (!settings.tableHideColumnCheckbox[i]->isChecked()
                && wasHidden
                && i != 0) {
            planetTreeView->resizeColumnToContents(i-1);
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
    QList<QStandardItem*> planetRow;
    planetRow << new QStandardItem(QString("%1:%2").arg(planet->getAddress()).arg(planet->getPort()));
    planetRow << new QStandardItem();
    planetRow << new QStandardItem();
    planetRow << new QStandardItem();
    planetRow << new QStandardItem();
    planetTreeModel->invisibleRootItem()->appendRow(planetRow);
    resizeColumnsToContents();
}

void PlanetScannerDock::removePlanet(Planet* planet)
{
    planetList.removeAt(planetList.indexOf(planet));
    delete getPlanetTreeWidgetItem(*planet);
}

void PlanetScannerDock::startGame(const QString &additionalCommandlineArguments)
{
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.at(2).data().toString() == "") {
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
    QStringList fullAddress = selectedIndexes.at(0).parent().data().toString().split(':');
    QString newAddress = fullAddress[0];
    QString newPort = fullAddress[1];
    nfksetup.setValue("address", newAddress);
    nfksetup.setValue("port", newPort);
    game->start(gamePath, arguments.split(" "));
    game->waitForStarted(10 * 1000);
    nfksetup.setValue("address", oldAddress);
    nfksetup.setValue("port", oldPort);
}

void PlanetScannerDock::connectSelected()
{
    startGame("+connect " + planetTreeView->selectionModel()->selectedIndexes().at(4).data().toString());
}

void PlanetScannerDock::connectAsSpectatorSelected()
{
    QString configPath = getBasenfkPath()+ "spec.cfg";
    QFile config(configPath);
    if (!config.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
        error("Couldn't open:\n\"" + configPath + "\"");
        return;
    }
    QString configContent = "spectator 1\necho spectator on\nconnect " + planetTreeView->selectionModel()->selectedIndexes().at(4).data().toString();
    config.write(configContent.toLatin1());
    config.flush();
    startGame("+exec spec ");
}

QString PlanetScannerDock::getBasenfkPath()
{
    return gamePath.left(gamePath.lastIndexOf('/')) + "/basenfk/";
}

void PlanetScannerDock::copySelected()
{
    QString copy;
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    for (int i = 0; i < planetTreeModel->columnCount(); i++) {
        if (planetTreeView->isColumnHidden(i)) {
            continue;
        }
        QString column = selectedIndexes.at(i).data().toString();
        if (!column.compare("")) {
            continue;
        }
        copy += column;
        if (i < planetTreeModel->columnCount()-1) {
            if (selectedIndexes.at(i+1).data().toString().compare("") && !planetTreeView->isColumnHidden(i+1)) {
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
        set.setValue("hide", planetTreeProxyModel->getGameTypeFilter()[i]);
    }
    set.endArray();
    set.beginGroup("PlayerCount");
    set.setValue("hideFull", planetTreeProxyModel->getHideOnFullFilter());
    set.setValue("hideEmpty", planetTreeProxyModel->getHideOnEmptyFilter());
    set.endGroup();
    set.endGroup();
    set.beginGroup("Table");
    set.beginWriteArray("Column");
    for (int i = 0; i < planetTreeModel->columnCount(); i++) {
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
        planetTreeProxyModel->setGameTypeFilter(i, set.value("hide", false).toBool());
    }
    set.endArray();
    set.beginGroup("PlayerCount");
    planetTreeProxyModel->setHideOnFullFilter(set.value("hideFull", false).toBool());
    planetTreeProxyModel->setHideOnEmptyFilter(set.value("hideEmpty", false).toBool());
    set.endGroup();
    set.endGroup();
    set.beginGroup("Table");
    int planetTreeColumns = set.beginReadArray("Column");
    for (int i = 0; i < planetTreeColumns; i++) {
        set.setArrayIndex(i);
        if (set.value("hide", false).toBool()) {
            planetTreeView->hideColumn(i);
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
    QPoint globalPos = planetTreeView->viewport()->mapToGlobal(pos);
    globalPos.setX(globalPos.x() + 1);
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() != 1 * 5) {
        return;
    }
    QModelIndex selectedIndex = selectedIndexes[2];
    contextMenuShown = true;
    if (!selectedIndex.data().toString().compare("")) {
        planetContextMenu->exec(globalPos);
    } else {
        gameContextMenu->exec(globalPos);
    }
    contextMenuShown = false;
}
