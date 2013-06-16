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

#ifndef GAMEPREFERENCESSETTINGS_HPP
#define GAMEPREFERENCESSETTINGS_HPP

#include <QWidget>

namespace GamePreferences {

class Settings : public QObject
{
    Q_OBJECT
public:
    static Settings& getInstance();
    void load();
    void save();
    void executeSettingsDialog(QWidget* parent);

    //

    // max nick length allowed by NFK
    static const int MAX_INGAMENICK_LENGTH = 30;

    const QString& getInGameNick() const;
    void setInGameNick(const QString& newNick);

    //

    const QVector<bool>& getDefaultPreferences() const;
    void setDefaultPreferences(const QVector<bool>& newPreferences);

    //

    const QVector<bool>& getHiddenColumns() const;
    void setHiddenColumns(const QVector<bool>& newHiddenColumns);

private:
    QString inGameNick;

    //

    QVector<bool> preferences;

    //

    QVector<bool> columns;

    //

    bool loaded;

    Settings();
    Settings(Settings &settings) = delete;
    Settings& operator=(const Settings&) = delete;

signals:
    void dataChanged();

};

} // namespace GamePreferences

#endif // GAMEPREFERENCESSETTINGS_HPP
