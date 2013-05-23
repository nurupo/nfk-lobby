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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static QString name;
    static QString version;
    static QString buildDate;
    static QString author;
    static QString years;

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void tabsTopActionTriggered();
    void tabsLeftActionTriggered();
    void tabsRightActionTriggered();
    void tabsBottomActionTriggered();
    void aboutAppActionTriggered();
    void applySettings();
};

#endif // MAINWINDOW_HPP
