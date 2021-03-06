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

#include "aboutdialog.hpp"
#include "Chat/chatdock.hpp"
#include "GamePreferences/gamepreferencesdock.hpp"
#include "mainwindow.hpp"
#include "PlanetScanner/planetscannerdock.hpp"
#include "Settings/settings.hpp"

#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QToolBar>
#include <QTreeWidget>
#include <QVBoxLayout>

//NOTE: should I move these constants somewhere else?
//Looks like it's better to make them be defines in some header file,
//instead of tying them to the MainWindow
const QString MainWindow::name = "NFK Lobby";
const QString MainWindow::version = "0.2.2";
const QString MainWindow::buildDate = __DATE__;
const QString MainWindow::author = "Maxim Biro";
const QString MainWindow::years = "2013-2014";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(700, 350, 1000, 400);
    setWindowTitle(name);
    setObjectName("MainWindow");
    setDockNestingEnabled(true);

    PlanetScanner::Dock* scannerDock = new PlanetScanner::Dock(this);
    addDockWidget(Qt::TopDockWidgetArea, scannerDock);

    Chat::Dock* chatDock = new Chat::Dock(this);
    addDockWidget(Qt::TopDockWidgetArea, chatDock);

    GamePreferences::Dock* preferencesDock = new GamePreferences::Dock(this);
    preferencesDock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);
    addDockWidget(Qt::TopDockWidgetArea, preferencesDock);

    tabifyDockWidget(scannerDock, chatDock);
    tabifyDockWidget(chatDock, preferencesDock);

    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::South);
    chatDock->raise();

    QMenuBar* menu = new QMenuBar(this);
    setMenuBar(menu);
    setContextMenuPolicy(Qt::PreventContextMenu);

    QAction* tabsTopAction      = new QAction("Top", this);
    QAction* tabsLeftAction     = new QAction("Left", this);
    QAction* tabsRightAction    = new QAction("Right", this);
    QAction* tabsBottomAction   = new QAction("Bottom", this);

    connect(tabsTopAction,      &QAction::triggered, this, &MainWindow::tabsTopActionTriggered);
    connect(tabsLeftAction,     &QAction::triggered, this, &MainWindow::tabsLeftActionTriggered);
    connect(tabsRightAction,    &QAction::triggered, this, &MainWindow::tabsRightActionTriggered);
    connect(tabsBottomAction,   &QAction::triggered, this, &MainWindow::tabsBottomActionTriggered);

    QMenu* windowMenu = menu->addMenu("Window");
    windowMenu->addMenu("Tab position")->addActions(QList<QAction*>() << tabsTopAction << tabsLeftAction << tabsRightAction << tabsBottomAction);
    windowMenu->addMenu("Show dock")->addActions(QList<QAction*>() << scannerDock->toggleViewAction() << chatDock->toggleViewAction() << preferencesDock->toggleViewAction());

    QMenu* aboutMenu = menu->addMenu("About");
    QAction* aboutQtAction = new QAction("About Qt", this);
    QAction* aboutAppAction = new QAction(QString("About %1").arg(name), this);

    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(aboutAppAction, &QAction::triggered, this, &MainWindow::aboutAppActionTriggered);

    aboutMenu->addActions(QList<QAction*>() << aboutQtAction << aboutAppAction);

    ::Settings::loadWindow(this);
}

MainWindow::~MainWindow()
{
    Settings::saveWindow(this);
}

void MainWindow::tabsTopActionTriggered()
{
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::North);
}

void MainWindow::tabsLeftActionTriggered()
{
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::West);
}

void MainWindow::tabsRightActionTriggered()
{
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::East);
}

void MainWindow::tabsBottomActionTriggered()
{
    setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::South);
}

void MainWindow::aboutAppActionTriggered()
{
    AboutDialog dialog(this);
    dialog.exec();
}
