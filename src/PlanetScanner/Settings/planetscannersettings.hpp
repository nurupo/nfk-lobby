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

#ifndef PLANETSCANNERSETTINGS_HPP
#define PLANETSCANNERSETTINGS_HPP

#include "../game.hpp"

#include <QDialog>
#include <QMap>
#include <QString>

namespace PlanetScanner {

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings& getInstance();
    void load();
    void save();
    void executeSettingsDialog(QWidget* parent);

    const QString& getGamePath() const;
    void setGamePath(const QString& newGamePath);
    const QString& getCommandlineArguments() const;
    void setCommandlineArguments(const QString& newCommandlineArguments);

    //

    struct PlanetAddress {
        QString address;
        int port;
    };

    const QList<PlanetAddress>& getPlanets() const;
    void setPlanets(QList<PlanetAddress>& newPlanets);

    //

    const QMap<QString, bool>& getGameTypeFilter() const;
    void setGameTypeFilter(const QMap<QString, bool>& newGameTypeFilter);
    bool getHideOnFullFilter() const;
    void setHideOnFullFilter(bool value);
    bool getHideOnEmptyFilter() const;
    void setHideOnEmptyFilter(bool value);

    //

    const QList<bool>& getHideColumn() const;
    void setHideColumn(const QList<bool>& newHideColumn);
    bool getResizeOnRefreshDisabled() const;
    void setResizeOnRefreshDisabled(bool value);

    //

    bool getAutoRefresh() const;
    void setAutoRefresh(bool value);
    int getAutoRefreshIntervalSec() const;
    void setAutoRefreshIntervalSec(int sec);

private:
    QString gamePath;
    QString commandlineArguments;

    //

    QList<PlanetAddress> planets;

    //

    QMap<QString, bool> gameTypeFilter;
    bool hideOnFullFilter;
    bool hideOnEmptyFilter;

    //

    QList<bool> hideColumn;
    bool resizeOnRefreshDisabled;

    //

    bool autoRefresh;
    int autoRefreshIntervalSec;

    //

    bool loaded;

    Settings();
    Settings(Settings &settings) = delete;
    Settings& operator=(const Settings&) = delete;

signals:
    void dataChanged();

};

} // namespace PlanetScanner

#endif // PLANETSCANNERSETTINGS_HPP
