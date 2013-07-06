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

#ifndef PLANETSCANNERWINDOW_HPP
#define PLANETSCANNERWINDOW_HPP

#include "game.hpp"
#include "planet.hpp"
#include "planettreemodel.hpp"
#include "planettreesortfilterproxymodel.hpp"
#include "statisticswebsite.hpp"

#include <QMainWindow>
#include <QMenu>
#include <QProcess>
#include <QStandardItemModel>
#include <QTimer>
#include <QTreeView>

namespace PlanetScanner {

class Window : public QMainWindow
{
    Q_OBJECT
public:
    Window(QWidget* parent = 0);
    ~Window();

private:
    QProcess* game;
    QMenu* gameContextMenu;
    QMenu* planetContextMenu;
    bool contextMenuShown;
    QTimer* autoRefreshTimer;
    QList<Planet*> planetList;
    QTreeView* planetTreeView;
    PlanetTreeModel* planetTreeModel;
    PlanetTreeSortFilterProxyModel* planetTreeProxyModel;
    StatisticsWebSite* statistics;

    void addPlanet(Planet* planet);
    void removePlanet(Planet* planet);
    QStandardItem* getPlanetTreeWidgetItem(const Planet &planet);
    void resizeColumnsToContents();
    void startGame(const QString &additionalCommandlineArguments);
    QString getBasenfkPath();
    void error(const QString &errorText);
    void addAppearedGames(QStandardItem* planetItem, const QList<Game> &games);
    void removeDisappearedGames(QStandardItem* planetItem, const QList<Game> &games);
    void updateExistingGames(QStandardItem* planetItem, const QList<Game> &games);
    void updatePlayers(QStandardItem* planetItem, const QHash<QString, QList<StatisticsWebSite::PlayerInfo>>& playersHash);
    void addAppearedPlayers(QStandardItem* gameItem, const QList<StatisticsWebSite::PlayerInfo>& playersInfo);
    void removeDisappearedPlayers(QStandardItem* gameItem, const QList<StatisticsWebSite::PlayerInfo>& playersInfo);
    void removeAllPlayers(QStandardItem* gameItem);

private slots:
    void refreshPlanets();
    void showSettingsDialog();
    void processPlanetGames(const Planet &planet, const QList<Game> &games);
    void processStatisticsPlayers(QHash<QString, QList<StatisticsWebSite::PlayerInfo>> playersHash);
    void setPlanetConnectionError(const Planet &planet, QAbstractSocket::SocketError socketError);
    void clearPlanetConnectionError(const Planet &planet);
    void connectSelected();
    void connectAsSpectatorSelected();
    void copySelected();
    void showContextMenu(const QPoint &pos);
    void applyChangedSettings();
};

} // namespace PlanetScanner

#endif // PLANETSCANNERWINDOW_HPP
