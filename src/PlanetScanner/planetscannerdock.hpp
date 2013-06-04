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

#ifndef PLANETSCANNERDOCK_HPP
#define PLANETSCANNERDOCK_HPP

#include "game.hpp"
#include "planet.hpp"
#include "planettreesortfilterproxymodel.hpp"
#include "planettreemodel.hpp"

#include <QDockWidget>
#include <QStandardItemModel>
#include <QTimer>
#include <QTreeWidget>

class PlanetScannerDock : public QDockWidget
{
    Q_OBJECT
public:
    PlanetScannerDock(QWidget* parent);
    ~PlanetScannerDock();
    QList<Planet*> & getPlanetList() {return planetList;}
    bool isAutoRefrshActive() const {return autoRefreshTimer->isActive();}
    int getAutoRefreshInterval() const {return autoRefreshTimer->interval();}
    const PlanetTreeSortFilterProxyModel* getPlanetTreeProxyModel() const {return planetTreeProxyModel;}
    bool isColumnHidden(int i) const {return planetTreeView->isColumnHidden(i);}
    bool isResizeOnRefreshDisabled() const {return resizeOnRefreshDisabled;}
    const QString & getGamePath() const {return gamePath;}
    const QString & getGameCommandlineArguments() const {return gameCommandlineArguments;}
    void addPlanet(Planet* planet);

private:
    QProcess* game;
    QMenu* gameContextMenu;
    QMenu* planetContextMenu;
    bool contextMenuShown;
    QTimer* autoRefreshTimer;
    bool resizeOnRefreshDisabled;
    QString gamePath;
    QString gameCommandlineArguments;
    QList<Planet*> planetList;
    QTreeView* planetTreeView;
    PlanetTreeModel* planetTreeModel;
    PlanetTreeSortFilterProxyModel* planetTreeProxyModel;
    void removePlanet(Planet* planet);
    QStandardItem* getPlanetTreeWidgetItem(const Planet &planet);
    void resizeColumnsToContents();
    void startGame(const QString &additionalCommandlineArguments);
    QString getBasenfkPath();
    void save();
    void load();
    void error(const QString &errorText);

private slots:
    void refreshPlanets();
    void showSettingsDialog();
    void addGame(const Planet &planet, const QList<Game> &games);
    void setPlanetConnectionError(const Planet &planet, QAbstractSocket::SocketError socketError);
    void clearPlanetConnectionError(const Planet &planet);
    void connectSelected();
    void connectAsSpectatorSelected();
    void copySelected();
    void showContextMenu(const QPoint &pos);
};

#endif // PLANETSCANNERDOCK_HPP
