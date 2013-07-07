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

#include "../Settings/settings.hpp"
#include "planetscannerwindow.hpp"
#include "Settings/planetscannersettings.hpp"

#include <QAction>
#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
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

namespace PlanetScanner {

Window::Window(QWidget* parent):
    QMainWindow(parent)
{
    setObjectName("PlanetScannerWindow");

    QToolBar* toolbar = new QToolBar(this);
    toolbar->setIconSize(QSize(24, 24));
    toolbar->setFloatable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    addToolBar(toolbar);

    QAction* refreshAction = toolbar->addAction(QIcon(":/icons/refresh.png"), "Refresh");
    connect(refreshAction, SIGNAL(triggered()), this, SLOT(refreshPlanets()));
    QAction* settingsAction = toolbar->addAction(QIcon(":/icons/settings.png"), "Settings");
    connect(settingsAction, SIGNAL(triggered()), this, SLOT(showSettingsDialog()));

    planetTreeView = new QTreeView(this);
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
    QAction* openProfileAction = new QAction("Open profile in a browser", planetTreeView);
    connect(connectAction, SIGNAL(triggered()), this, SLOT(connectSelected()));
    connect(connectAsSpectatorAction, SIGNAL(triggered()), this, SLOT(connectAsSpectatorSelected()));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copySelected()));
    connect(openProfileAction, &QAction::triggered, this, &Window::openProfileSelected);
    gameContextMenu = new QMenu(planetTreeView);
    planetContextMenu = new QMenu(planetTreeView);
    registeredPlayerContextMenu = new QMenu(planetTreeView);
    unregisteredPlayerContextMenu = new QMenu(planetTreeView);
    gameContextMenu->addActions(QList<QAction*>() << connectAction << connectAsSpectatorAction << copyAction);
    planetContextMenu->addActions(QList<QAction*>() << copyAction);
    registeredPlayerContextMenu->addActions(QList<QAction*>() << openProfileAction << copyAction);
    unregisteredPlayerContextMenu->addActions(QList<QAction*>() << copyAction);

    setCentralWidget(planetTreeView);

    game = new QProcess(this);

    statistics = new StatisticsWebSite(this);
    connect(statistics, &StatisticsWebSite::playersInfoRecieved, this, &Window::processStatisticsPlayers);

    autoRefreshTimer = new QTimer(this);
    connect(autoRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshPlanets()));

    contextMenuShown = false;

    Settings& settings = Settings::getInstance();
    connect(&settings, &Settings::dataChanged, this, &Window::applyChangedSettings);
    settings.load();
    applyChangedSettings();

    ::Settings::loadWindow(this);

    refreshPlanets();
}

Window::~Window()
{
    while(planetList.size() != 0) {
        delete planetList.takeAt(0);
    }

    ::Settings::saveWindow(this);
}

QStandardItem* Window::getPlanetTreeWidgetItem(const Planet &planet)
{
    const QString searchingText = QString("%1:%2").arg(planet.getAddress()).arg(planet.getPort());
    QList<QStandardItem*> items = planetTreeModel->findItems(searchingText, Qt::MatchExactly, 0);
    // we assume that there are no duplicates
    for (QStandardItem* item : items) {
        if (planetTreeModel->getItemType(item) == PlanetTreeModel::ItemType::Planet) {
            return item;
        }
    }
    return 0;
}

void Window::refreshPlanets()
{
    /*if (contextMenuShown) {
        return;
    }*/
    for (int i = 0; i < planetList.size(); i ++) {
        planetList.at(i)->requestGameInfo();
    }

    if (Settings::getInstance().getPullPlayers()) {
        statistics->requestPlayersInfo();
    }
}

void Window::processPlanetGames(const Planet &planet, const QList<Game> &games)
{
    QVariant selectedIndexData = planetTreeView->selectionModel()->currentIndex().data();

    QStandardItem* planetItem = getPlanetTreeWidgetItem(planet);

    removeDisappearedGames(planetItem, games);
    updateExistingGames(planetItem, games);
    addAppearedGames(planetItem, games);

    if (selectedIndexData != planetTreeView->selectionModel()->currentIndex().data()) {
        // selected item disappeared
        planetTreeView->selectionModel()->clear();
    }

    planetTreeView->expandAll();
    resizeColumnsToContents();
}

void Window::addAppearedGames(QStandardItem* planetItem, const QList<Game> &games)
{
    for (const Game& game : games) {
        bool found = false;
        //we use address as a unique identifier
        const QString address = QString("%1:%2").arg(game.getIp()).arg(game.getPort());
        for (int row = 0; row < planetItem->rowCount(); row ++) {
            //4th column = ip:port = address
            if (planetItem->child(row, 4)->text() == address) {
                found = true;
                break;
            }
        }
        if (!found) {
            QList<QStandardItem*> gameItems;
            QStandardItem* hostname = new QStandardItem(game.getCleanedHostname());
            planetTreeModel->setItemType(hostname, PlanetTreeModel::ItemType::Game);
            gameItems << hostname;
            gameItems << new QStandardItem(game.getMap());
            gameItems << new QStandardItem(Game::getNameForGametype(game.getGametype()));
            gameItems << new QStandardItem(QString("%1/%2").arg(game.getCurrentPlayers()).arg(game.getMaxPlayers()));
            gameItems << new QStandardItem(QString("%1:%2").arg(game.getIp()).arg(game.getPort()));
            planetItem->appendRow(gameItems);
        }
    }
}

void Window::removeDisappearedGames(QStandardItem* planetItem, const QList<Game> &games)
{
    for (int row = 0; row < planetItem->rowCount(); row ++) {
        bool found = false;
        for (const Game& game : games) {
            //we use address as a unique identifier
            const QString address = QString("%1:%2").arg(game.getIp()).arg(game.getPort());
            //4th column = ip:port = address
            if (planetItem->child(row, 4)->text() == address) {
                found = true;
                break;
            }
        }
        if (!found) {
            qDeleteAll(planetItem->takeRow(row));
        }
    }
}

void Window::updateExistingGames(QStandardItem* planetItem, const QList<Game> &games)
{
    for (const Game& game : games) {
        //we use address as a unique identifier
        const QString address = QString("%1:%2").arg(game.getIp()).arg(game.getPort());
        for (int row = 0; row < planetItem->rowCount(); row ++) {
            //4th column = ip:port = address
            if (planetItem->child(row, 4)->text() == address) {
                planetItem->child(row, 0)->setText(game.getCleanedHostname());
                planetItem->child(row, 1)->setText(game.getMap());
                planetItem->child(row, 2)->setText(Game::getNameForGametype(game.getGametype()));
                planetItem->child(row, 3)->setText(QString("%1/%2").arg(game.getCurrentPlayers()).arg(game.getMaxPlayers()));
                break;
            }
        }
    }
}

void Window::processStatisticsPlayers(QHash<QString, QList<StatisticsWebSite::PlayerInfo>> playersHash)
{
    QVariant selectedIndexData = planetTreeView->selectionModel()->currentIndex().data();

    for (int i = 0; i < planetTreeModel->rowCount(); i ++) {
        QStandardItem* planetItem = planetTreeModel->item(i, 0);
        updatePlayers(planetItem, playersHash);
    }

    if (selectedIndexData != planetTreeView->selectionModel()->currentIndex().data()) {
        // selected item disappeared
        planetTreeView->selectionModel()->clear();
    }

    planetTreeView->expandAll();
    resizeColumnsToContents();
}

void Window::updatePlayers(QStandardItem* planetItem, const QHash<QString, QList<StatisticsWebSite::PlayerInfo>>& playersHash)
{
    QHashIterator<QString, QList<StatisticsWebSite::PlayerInfo>> i(playersHash);

    for (int row = 0; row < planetItem->rowCount(); row ++) {
        bool found = false;
        QString address;

        i.toFront();
        while (i.hasNext()) {
            i.next();
            address = i.key();
            //4th column = ip:port = address
            if (planetItem->child(row, 4)->text() == address) {
                found = true;
                break;
            }
        }

        QStandardItem* gameItem = planetItem->child(row, 0);
        if (found) {
            if (gameItem->rowCount() != 0) {
                removeDisappearedPlayers(gameItem, playersHash[address]);
            }
            addAppearedPlayers(gameItem, playersHash[address]);
        } else {
            if (gameItem->rowCount() != 0) {
                removeAllPlayers(gameItem);
            }
        }
    }
}

void Window::addAppearedPlayers(QStandardItem* gameItem, const QList<StatisticsWebSite::PlayerInfo>& playersInfo)
{
    for (const StatisticsWebSite::PlayerInfo& playerInfo : playersInfo) {
        bool found = false;
        for (int row = 0; row < gameItem->rowCount(); row ++) {
            if (gameItem->child(row, 0)->text() == playerInfo.name) {
                found = true;
                break;
            }
        }
        if (!found) {
            QStandardItem* playerItem = new QStandardItem(playerInfo.name);
            planetTreeModel->setItemType(playerItem, PlanetTreeModel::ItemType::Player);
            playerItem->setData(playerInfo.playerId, PlanetTreeModel::PlayerIdRole);
            gameItem->setColumnCount(5);
            gameItem->appendRow(QList<QStandardItem*>() << playerItem);
        }
    }
}

void Window::removeDisappearedPlayers(QStandardItem* gameItem, const QList<StatisticsWebSite::PlayerInfo>& playersInfo)
{
    for (int row = 0; row < gameItem->rowCount(); row ++) {
        bool found = false;
        for (const StatisticsWebSite::PlayerInfo& playerInfo : playersInfo) {
            if (gameItem->child(row, 0)->text() == playerInfo.name) {
                found = true;
                break;
            }
        }
        if (!found) {
            qDeleteAll(gameItem->takeRow(row));
        }
    }
}

void Window::removeAllPlayers(QStandardItem* gameItem)
{
    while (gameItem->rowCount()) {
        qDeleteAll(gameItem->takeRow(0));
    }
}

void Window::removeAllPlayers()
{
    for (int i = 0; i < planetTreeModel->rowCount(); i ++) {
        QStandardItem* planetItem = planetTreeModel->item(i, 0);
        for (int row = 0; row < planetItem->rowCount(); row ++) {
            QStandardItem* gameItem = planetItem->child(row, 0);
            if (gameItem->rowCount() != 0) {
                removeAllPlayers(gameItem);
            }
        }
    }
}

void inline Window::resizeColumnsToContents()
{
    if (!Settings::getInstance().getResizeOnRefreshDisabled()) {
        planetTreeView->header()->resizeSections(QHeaderView::ResizeToContents);
    }
}

void Window::setPlanetConnectionError(const Planet &planet, QAbstractSocket::SocketError socketError)
{
    QString errorEnum;
    QDebug(&errorEnum).nospace() << socketError;
    QString error = QString("Error: [%1]").arg(errorEnum);
    QStandardItem* planetItem = getPlanetTreeWidgetItem(planet);
    QStandardItem* errorItem = planetTreeModel->item(planetItem->row(), 1);
    //no need to reset the error and resize the treeview if the error is the same
    if (errorItem->text().compare(error)) {
        errorItem->setText(error);
        if (!Settings::getInstance().getResizeOnRefreshDisabled()) {
            planetTreeView->resizeColumnToContents(1);
        }
    }
}

void Window::clearPlanetConnectionError(const Planet &planet)
{
    QStandardItem* planetItem = getPlanetTreeWidgetItem(planet);
    QStandardItem* errorItem = planetTreeModel->item(planetItem->row(), 1);
    errorItem->setText("");
    resizeColumnsToContents();
}

void Window::showSettingsDialog()
{
    Settings::getInstance().executeSettingsDialog(this);
}

void Window::addPlanet(Planet* planet)
{
    connect(planet, SIGNAL(gameInfoRecieved(const Planet &, const QList<Game> &)), this, SLOT(processPlanetGames(const Planet &, const QList<Game> &)));
    connect(planet, SIGNAL(error(const Planet &, QAbstractSocket::SocketError)), this, SLOT(setPlanetConnectionError(const Planet &, QAbstractSocket::SocketError)));
    connect(planet, SIGNAL(errorCleared(const Planet &)), this, SLOT(clearPlanetConnectionError(const Planet &)));
    planetList << planet;
    QList<QStandardItem*> planetRow;
    QStandardItem* address = new QStandardItem(QString("%1:%2").arg(planet->getAddress()).arg(planet->getPort()));
    planetTreeModel->setItemType(address, PlanetTreeModel::ItemType::Planet);
    QStandardItem* error = new QStandardItem();
    planetRow << address << error;
    planetTreeModel->invisibleRootItem()->appendRow(planetRow);
    resizeColumnsToContents();
    refreshPlanets();
}

void Window::removePlanet(Planet* planet)
{
    planetList.removeAt(planetList.indexOf(planet));
    QStandardItem* item = getPlanetTreeWidgetItem(*planet);
    planetTreeModel->removeRow(item->row());
}

void Window::startGame(const QString &additionalCommandlineArguments)
{
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 0) {
        return;
    }
    if (planetTreeModel->getIndexType(selectedIndexes.at(0)) != PlanetTreeModel::ItemType::Game) {
        error("Wrong selection.");
        return;
    }
    QString arguments = Settings::getInstance().getCommandlineArguments();
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
    game->start(Settings::getInstance().getGamePath(), arguments.split(" "));
    game->waitForStarted(10 * 1000);
    nfksetup.setValue("address", oldAddress);
    nfksetup.setValue("port", oldPort);
}

void Window::connectSelected()
{
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 0) {
        return;
    }
    startGame("+connect " + selectedIndexes.at(4).data().toString());
}

void Window::connectAsSpectatorSelected()
{
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 0) {
        return;
    }
    QString configPath = getBasenfkPath()+ "spec.cfg";
    QFile config(configPath);
    if (!config.open(QIODevice::Truncate | QIODevice::WriteOnly | QIODevice::Text)) {
        error("Couldn't open:\n\"" + configPath + "\"");
        return;
    }
    QString configContent = "spectator 1\necho spectator on\nconnect " + selectedIndexes.at(4).data().toString();
    config.write(configContent.toLatin1());
    config.flush();
    startGame("+exec spec ");
}

QString Window::getBasenfkPath()
{
    QString gamePath = Settings::getInstance().getGamePath();
    return gamePath.left(gamePath.lastIndexOf('/')) + "/basenfk/";
}

void Window::copySelected()
{
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 0) {
        return;
    }
    QString copy;
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

void Window::openProfileSelected()
{
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 0) {
        return;
    }
    double playerId = selectedIndexes.at(0).data(PlanetTreeModel::PlayerIdRole).toDouble();
    QDesktopServices::openUrl(StatisticsWebSite::getProfileUrl(playerId));
}

void Window::error(const QString &errorText)
{
    QMessageBox::critical(this, QString("Error"), errorText, QMessageBox::Ok);
}

void Window::showContextMenu(const QPoint &pos)
{
    QPoint globalPos = planetTreeView->viewport()->mapToGlobal(pos);
    globalPos.setX(globalPos.x() + 1);
    QModelIndexList selectedIndexes = planetTreeView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 0) {
        return;
    }
    contextMenuShown = true;
    PlanetTreeModel::ItemType itemType = planetTreeModel->getIndexType(selectedIndexes.at(0));
    if (itemType == PlanetTreeModel::ItemType::Planet) {
        planetContextMenu->exec(globalPos);
    } else if (itemType == PlanetTreeModel::ItemType::Game) {
        gameContextMenu->exec(globalPos);
    } else if (itemType == PlanetTreeModel::ItemType::Player) {
        if (selectedIndexes.at(0).data(PlanetTreeModel::PlayerIdRole).toInt() == 0) {
            unregisteredPlayerContextMenu->exec(globalPos);
        } else {
            registeredPlayerContextMenu->exec(globalPos);
        }
    }
    contextMenuShown = false;
}

void Window::applyChangedSettings()
{
    const Settings& settings = Settings::getInstance();
    QList<Settings::PlanetAddress> planets = settings.getPlanets();
    //removing planets from planetList that are not presentes in settings
    //and removing planets in settings that are already presented in the PlanetList
    for (Planet* p1 : planetList) {
        int i;
        bool found = false;
        for (i = 0; i < planets.size(); i ++) {
            found = p1->getAddress() == planets.at(i).address && p1->getPort() == planets.at(i).port;
            if (found) {
                break;
            }
        }
        if (found) {
            planets.removeAt(i);
        } else {
            removePlanet(p1);
            delete p1;
        }
    }
    //planets now contains all new planets
    for (const Settings::PlanetAddress& p2 : planets) {
        addPlanet(new Planet(p2.address, p2.port));
    }

    const QList<bool>& hideColumn = settings.getHideColumn();
    for (int i = 0; i < hideColumn.size(); i ++) {
        bool wasHidden = planetTreeView->isColumnHidden(i);
        planetTreeView->setColumnHidden(i, hideColumn.at(i));
        if (!hideColumn.at(i) && wasHidden && i != 0) {
            planetTreeView->resizeColumnToContents(i-1);
        }
    }

    if (settings.getAutoRefresh()) {
        autoRefreshTimer->start(settings.getAutoRefreshIntervalSec() * 1000);
    } else {
        autoRefreshTimer->stop();
    }

    if (!settings.getPullPlayers()) {
        removeAllPlayers();
    }

    planetTreeProxyModel->invalidate();
}

} // namespace PlanetScanner
