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

#include "../mainwindow.hpp"
#include "../Settings/settings.hpp"
#include "gamepreferenceswindow.hpp"
#include "Settings/gamepreferencessettings.hpp"

#include <QAction>
#include <QBrush>
#include <QCheckBox>
#include <QDateTime>
#include <QHeaderView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QToolBar>
#include <QVector>

namespace GamePreferences {

Window::Window(QWidget *parent) :
    QMainWindow(parent)
{
    setObjectName("GamePreferencesWindow");

    Settings::getInstance().load();
    connect(&Settings::getInstance(), &Settings::dataChanged, this, &Window::applySettings);

    QToolBar* toolbar = new QToolBar(this);
    toolbar->setIconSize(QSize(24, 24));
    toolbar->setFloatable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    addToolBar(toolbar);

    QAction* connectAction = new QAction(QIcon(":/icons/connect.png"), "Connect", toolbar);
    QAction* disconnectAction = new QAction(QIcon(":/icons/disconnect.png"), "Disconnect", toolbar);
    QAction* settingsAction = toolbar->addAction(QIcon(":/icons/settings.png"), "Settings");
    connect(connectAction, &QAction::triggered, this, &Window::connectToServer);
    connect(disconnectAction, &QAction::triggered, this, &Window::disconnectFromServer);
    connect(settingsAction, &QAction::triggered, this, &Window::showSettingsDialog);
    toolbar->addActions(QList<QAction*>() << connectAction << disconnectAction << settingsAction);

    preferencesView = new QTableView(this);
    preferencesView->setSortingEnabled(true);
    preferencesModel = new QStandardItemModel(preferencesView);
    sortModel = new PreferencesSortingModel(preferencesView);
    sortModel->setSourceModel(preferencesModel);
    QStringList headers;
    headers.append("Name");
    headers.append(GameType::getNames());
    preferencesModel->setHorizontalHeaderLabels(headers);
    preferencesView->setModel(sortModel);
    connect(preferencesView, &QTableView::clicked, this, &Window::onItemClicked);
    for (int i = 1; i < preferencesModel->columnCount(); i ++) {
        preferencesView->setColumnWidth(i, 50);
    }
    preferencesView->setColumnWidth(0, 150);

    ircClient = new IrcClient::IrcClient();
    ircClient->setVersion(QString("%1 %2 by %3 build date %4").arg(MainWindow::name).arg(MainWindow::version).arg(MainWindow::author).arg(MainWindow::buildDate));
    connect(ircClient, &IrcClient::IrcClient::connected,           this, &Window::onConnected);
    connect(ircClient, &IrcClient::IrcClient::disconnected,        this, &Window::onDisconnected);
    connect(ircClient, &IrcClient::IrcClient::join,                this, &Window::onChannelJoined);
    connect(ircClient, &IrcClient::IrcClient::privateMessage,      this, &Window::onPrivateMessageReceived);
    connect(ircClient, &IrcClient::IrcClient::nickAlreadyInUse,    this, &Window::onNickAlreadyInUse);

    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        gamePreferences[i] = false;
    }

    setCentralWidget(preferencesView);

    applySettings();
    ::Settings::loadWindow(this);
}

Window::~Window()
{
    delete ircClient;

    ::Settings::saveWindow(this);
}

void Window::connectToServer()
{
    ircClient->connect("irc.wenet.ru", 6667, "", getRandomIrcNick(), "nfkl_user", "windows-1251");
    //for testing purposes
    //ircClient->connect("chat.freenode.net", 6667, "", getRandomIrcNick(), "nfkl_user", "utf-8");
}

void Window::disconnectFromServer()
{
    ircClient->disconnect();
}

void Window::onConnected()
{
    ircClient->joinChannel("##nfk-game-preferences-test", "lobbytest");
}

void Window::onDisconnected()
{
    preferencesModel->removeRows(0, preferencesModel->rowCount());
    ircNickToIndexMap.clear();
}

void Window::onChannelJoined(const IrcClient::Channel &channel)
{
    connect(&channel, &IrcClient::Channel::channelMessage, this, &Window::onChannelMessageReceived);
    connect(&channel, &IrcClient::Channel::part, this, &Window::onUserParted);
    connect(&channel, &IrcClient::Channel::quit, this, &Window::onUserQuit);
    connect(&channel, &IrcClient::Channel::kick, this, &Window::onUserKicked);
    connect(&channel, &IrcClient::Channel::nickChange, this, &Window::onUserChangedNick);
    channelName = channel.getName();
    playerName = Settings::getInstance().getInGameNick();
    addPlayer(ircClient->getUs().nick, playerName);
    sendOurName(channel.getName());
    sendGameTypeInformation(channel.getName());
}

void Window::addPlayer(const QString &ircNick, const QString &gameNick)
{
    QList<QStandardItem*> itemList;
    for (int i = 0; i < preferencesModel->columnCount(); i ++) {
        QStandardItem* item = new QStandardItem();
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsEnabled);
        if (preferencesModel->rowCount() == 0) {
            QColor color(215, 215, 215);
            item->setData(color, Qt::BackgroundRole);
            if (i > 0) {
                const QVector<bool>& defaultPreferences = Settings::getInstance().getDefaultPreferences();
                item->setText(defaultPreferences.at(i-1) ? "✔" : "");
                gamePreferences[i-1] = defaultPreferences.at(i-1);
            }
        }
        if (i > 0) {
            item->setData(QFont(item->font().family(), 12), Qt::FontRole);
        }
        itemList << item;
    }
    itemList[0]->setText(gameNick);
    itemList[0]->setFlags(Qt::ItemIsEnabled);
    itemList[0]->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    preferencesModel->appendRow(itemList);
    ircNickToIndexMap[ircNick] = QPersistentModelIndex(preferencesModel->index(preferencesModel->rowCount() - 1, 0));
}

void Window::updateGameType(const QString &ircNick, bool set, int gameTypeId)
{
    if ((gameTypeId < 0) || (gameTypeId > GameType::NUMBER_OF_GAMETYPES - 1)) {
        return;
    }
    preferencesModel->setData(preferencesModel->index(ircNickToIndexMap[ircNick].row(), gameTypeId + 1), set ? "✔" : "");
}

void Window::updatePlayerName(const QString &ircNick, const QString newPlayerName)
{
    preferencesModel->setData(preferencesModel->index(ircNickToIndexMap[ircNick].row(), 0), newPlayerName);
}

void Window::onChannelMessageReceived(const IrcClient::User &sender, const QString &message)
{
    if (sender.nick == ircClient->getUs().nick) {
        return;
    }

    QStringListIterator iterator(message.split(' ', QString::SkipEmptyParts));
    const QString command = iterator.next();

    if (command == "NAME") {
        const QString playerName = message.mid(5);
        if (ircNickToIndexMap.contains(sender.nick)) {
            updatePlayerName(sender.nick, playerName);
        } else {
            addPlayer(sender.nick, playerName);
            sendOurName(sender.nick);
            sendGameTypeInformation(sender.nick);
        }
    } else if (command == "GAMETYPEID") {
        const bool set = iterator.next() == "+";
        while (iterator.hasNext()) {
            bool ok = false;
            const int gameTypeId = iterator.next().toInt(&ok);
            if (ok) {
                updateGameType(sender.nick, set, gameTypeId);
            }
        }
    }
}

void Window::onPrivateMessageReceived(const IrcClient::User &sender, const QString &message)
{
    if (sender.nick == ircClient->getUs().nick) {
        return;
    }

    QStringListIterator iterator(message.split(' ', QString::SkipEmptyParts));
    const QString command = iterator.next();

    if (command == "NAME") {
        const QString playerName = message.mid(5);
        addPlayer(sender.nick, playerName);
    } else if (command == "GAMETYPEID") {
        const bool set = iterator.next() == "+";
        while (iterator.hasNext()) {
            bool ok = false;
            const int gameTypeId = iterator.next().toInt(&ok);
            if (ok) {
                updateGameType(sender.nick, set, gameTypeId);
            }
        }
    }
}

void Window::sendGameTypeChange(bool set, int gameTypeId)
{
    ircClient->sendMessage(channelName, QString("GAMETYPEID %1 %2").arg(set ? "+" : "-").arg(gameTypeId));
}

void Window::sendGameTypeInformation(const QString &target)
{
    QString gameTypeIds;
    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        if (gamePreferences[i]) {
            gameTypeIds += QString("%1 ").arg(i);
        }
    }
    if (gameTypeIds.length() != 0) {
        ircClient->sendMessage(target, QString("GAMETYPEID + %1").arg(gameTypeIds.trimmed()));
    }
}

void Window::updateOurPlayerName(const QString &newPlayerName)
{
    if (playerName == newPlayerName) {
        return;
    }
    playerName = newPlayerName;
    updatePlayerName(ircClient->getUs().nick, newPlayerName);
    sendOurName(channelName);
}

void Window::sendOurName(const QString &target)
{
    ircClient->sendMessage(target, QString("NAME %1").arg(playerName));
}

void Window::onUserParted(const IrcClient::User &user)
{
    removePlayer(user.nick);
}

void Window::onUserQuit(const IrcClient::User &user)
{
    removePlayer(user.nick);
}

void Window::onUserKicked(const IrcClient::User& /*sender*/, const QString& /*message*/, const QString& recipient)
{
    removePlayer(recipient);
}

void Window::removePlayer(const QString &ircNick)
{
    if (!ircNickToIndexMap.contains(ircNick)) {
        return;
    }
    preferencesModel->removeRow(ircNickToIndexMap[ircNick].row());
    ircNickToIndexMap.remove(ircNick);
}

void Window::onUserChangedNick(const IrcClient::User& user, const QString& newNick)
{
    if (!ircNickToIndexMap.contains(user.nick)) {
        return;
    }
    ircNickToIndexMap[newNick] = ircNickToIndexMap[user.nick];
    ircNickToIndexMap.remove(user.nick);
}

void Window::applySettings()
{
    const Settings& settings = Settings::getInstance();
    const QVector<bool>& hideColumns = settings.getHiddenColumns();
    for (int i = 0; i < preferencesModel->columnCount(); i++) {
        if (hideColumns[i]) {
            preferencesView->hideColumn(i);
        } else {
            preferencesView->showColumn(i);
        }
    }
    updateOurPlayerName(settings.getInGameNick());
}

void Window::showSettingsDialog()
{
    Settings::getInstance().executeSettingsDialog(this);
}

void Window::onNickAlreadyInUse()
{
    ircClient->sendRaw(QString("NICK %1").arg(getRandomIrcNick()));
}

QString Window::getRandomIrcNick()
{
    static bool init = false;
    if (!init) {
        srand(QDateTime::currentDateTime().toTime_t());
        init = true;
    }
    return QString("nfkl_gp%1").arg(rand() % 100000);
}

void Window::onItemClicked(const QModelIndex& index)
{
    if (index.row() == 0 && index.column() > 0) {
        QStandardItem* item = preferencesModel->itemFromIndex(sortModel->mapToSource(index));
        bool set = item->text() == "✔";
        if (set) {
            item->setText("");
        } else {
            item->setText("✔");
        }
        const int gameTypeId = item->column() - 1;
        gamePreferences[gameTypeId] = !set;
        sendGameTypeChange(gamePreferences[gameTypeId], gameTypeId);
    }
}

} // namespace GamePreferences
