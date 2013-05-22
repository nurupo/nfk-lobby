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
#include "checkboxdelegate.hpp"
#include "gamepreferencesdock.hpp"
#include "Settings/gamepreferencesgeneralpage.hpp"
#include "Settings/gamepreferencespreferencespage.hpp"
#include "Settings/gamepreferencessettingsdialog.hpp"
#include "Settings/gamepreferencestablepage.hpp"

#include <QAction>
#include <QBrush>
#include <QCheckBox>
#include <QDateTime>
#include <QHeaderView>
#include <QMainWindow>
#include <QStandardItemModel>
#include <QToolBar>

GamePreferencesDock::GamePreferencesDock(QWidget *parent) :
    QDockWidget(parent)
{
    GamePreferencesSettingsDialog::load();
    connect(&GamePreferencesSettingsDialog::settingsNotifier, &BasicSettingsDialogNotifier::updated, this, &GamePreferencesDock::applySettings);

    setWindowTitle("Game Preferences");
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

    QMainWindow* window = new QMainWindow(0);

    QToolBar* toolbar = new QToolBar(window);
    toolbar->setIconSize(QSize(24, 24));
    toolbar->setFloatable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    window->addToolBar(toolbar);

    QAction* connectAction = new QAction(QIcon(":/icons/connect.png"), "Connect", toolbar);
    QAction* disconnectAction = new QAction(QIcon(":/icons/disconnect.png"), "Disconnect", toolbar);
    QAction* settingsAction = toolbar->addAction(QIcon(":/icons/settings.png"), "Settings");
    connect(connectAction, &QAction::triggered, this, &GamePreferencesDock::connectToServer);
    connect(disconnectAction, &QAction::triggered, this, &GamePreferencesDock::disconnectFromServer);
    connect(settingsAction, &QAction::triggered, this, &GamePreferencesDock::showSettingsDialog);
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
    connect(preferencesModel, &QStandardItemModel::itemChanged, this, &GamePreferencesDock::onOurGameTypeModified);
    for (int i = 1; i < preferencesModel->columnCount(); i ++) {
        preferencesView->setColumnWidth(i, 50);
        CheckBoxDelegate* checkBoxDelegate = new CheckBoxDelegate(preferencesView);
        preferencesView->setItemDelegateForColumn(i, checkBoxDelegate);
    }

    preferencesView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    preferencesView->horizontalHeader()->setSectionsMovable(true);

    ircClient = new IrcClient();
    ircClient->setVersion(QString("%1 %2 by %3 build date %4").arg(MainWindow::name).arg(MainWindow::version).arg(MainWindow::author).arg(MainWindow::buildDate));
    connect(ircClient, &IrcClient::connected,           this, &GamePreferencesDock::onConnected);
    connect(ircClient, &IrcClient::disconnected,        this, &GamePreferencesDock::onDisconnected);
    connect(ircClient, &IrcClient::join,                this, &GamePreferencesDock::onChannelJoined);
    connect(ircClient, &IrcClient::privateMessage,      this, &GamePreferencesDock::onPrivateMessageReceived);
    connect(ircClient, &IrcClient::nickAlreadyInUse,    this, &GamePreferencesDock::onNickAlreadyInUse);

    for (int i = 0; i < GameType::NUMBER_OF_GAMETYPES; i ++) {
        gamePreferences[i] = false;
    }

    applySettings();

    window->setCentralWidget(preferencesView);
    window->setParent(this);
    setWidget(window);
}

GamePreferencesDock::~GamePreferencesDock()
{
    delete ircClient;
}

void GamePreferencesDock::connectToServer()
{
    ircClient->connect("irc.wenet.ru", 6667, "", getRandomIrcNick(), "nfkl_user", "windows-1251");
    //for testing purposes
    //ircClient->connect("chat.freenode.net", 6667, "", getRandomIrcNick(), "nfkl_user", "utf-8");
}

void GamePreferencesDock::disconnectFromServer()
{
    ircClient->disconnect();
}

void GamePreferencesDock::onConnected()
{
    ircClient->joinChannel("##nfk-game-preferences-test", "lobbytest");
}

void GamePreferencesDock::onDisconnected()
{
    preferencesModel->removeRows(0, preferencesModel->rowCount());
    ircNickToIndexMap.clear();
}

void GamePreferencesDock::onChannelJoined(const Channel &channel)
{
    connect(&channel, &Channel::channelMessage, this, &GamePreferencesDock::onChannelMessageReceived);
    connect(&channel, &Channel::part, this, &GamePreferencesDock::onUserParted);
    connect(&channel, &Channel::quit, this, &GamePreferencesDock::onUserQuit);
    connect(&channel, &Channel::kick, this, &GamePreferencesDock::onUserKicked);
    connect(&channel, &Channel::nickChange, this, &GamePreferencesDock::onUserChangedNick);
    channelName = channel.getName();
    playerName = GamePreferencesGeneralPage::getInGameNick();
    addPlayer(ircClient->getUs().nick, playerName);
    sendOurName(channel.getName());
    sendGameTypeInformation(channel.getName());
}

void GamePreferencesDock::addPlayer(const QString &ircNick, const QString &gameNick)
{
    QList<QStandardItem*> itemList;
    for (int i = 0; i < preferencesModel->columnCount(); i ++) {
        QStandardItem* item = new QStandardItem();
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(Qt::ItemIsEnabled);
        if (preferencesModel->rowCount() == 0) {
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            QColor color(215, 215, 215);
            item->setData(color, Qt::BackgroundRole);
            if (i > 0) {
                item->setData(GamePreferencesPreferencesPage::getDefaultPreferences()[i-1] ? "✔" : "", Qt::EditRole);
                gamePreferences[i-1] = GamePreferencesPreferencesPage::getDefaultPreferences()[i-1];
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

void GamePreferencesDock::updateGameType(const QString &ircNick, bool set, int gameTypeId)
{
    if ((gameTypeId < 0) || (gameTypeId > GameType::NUMBER_OF_GAMETYPES - 1)) {
        return;
    }
    preferencesModel->setData(preferencesModel->index(ircNickToIndexMap[ircNick].row(), gameTypeId + 1), set ? "✔" : "");
}

void GamePreferencesDock::updatePlayerName(const QString &ircNick, const QString newPlayerName)
{
    preferencesModel->setData(preferencesModel->index(ircNickToIndexMap[ircNick].row(), 0), newPlayerName);
}

void GamePreferencesDock::onChannelMessageReceived(const User &sender, const QString &message)
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

void GamePreferencesDock::onPrivateMessageReceived(const User &sender, const QString &message)
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

void GamePreferencesDock::onOurGameTypeModified(QStandardItem* item)
{
    if (item->row() != 0) {
        return;
    }
    const int gameTypeId = item->column() - 1;
    gamePreferences[gameTypeId] = item->data(Qt::EditRole).toString() == "✔";
    sendGameTypeChange(gamePreferences[gameTypeId], gameTypeId);
}

void GamePreferencesDock::sendGameTypeChange(bool set, int gameTypeId)
{
    ircClient->sendMessage(channelName, QString("GAMETYPEID %1 %2").arg(set ? "+" : "-").arg(gameTypeId));
}

void GamePreferencesDock::sendGameTypeInformation(const QString &target)
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

void GamePreferencesDock::updateOurPlayerName(const QString &newPlayerName)
{
    if (playerName == newPlayerName) {
        return;
    }
    playerName = newPlayerName;
    updatePlayerName(ircClient->getUs().nick, newPlayerName);
    sendOurName(channelName);
}

void GamePreferencesDock::sendOurName(const QString &target)
{
    ircClient->sendMessage(target, QString("NAME %1").arg(playerName));
}

void GamePreferencesDock::onUserParted(const User &user)
{
    removePlayer(user.nick);
}

void GamePreferencesDock::onUserQuit(const User &user)
{
    removePlayer(user.nick);
}

void GamePreferencesDock::onUserKicked(const User& /*sender*/, const QString& /*message*/, const QString& recipient)
{
    removePlayer(recipient);
}


void GamePreferencesDock::removePlayer(const QString &ircNick)
{
    if (!ircNickToIndexMap.contains(ircNick)) {
        return;
    }
    preferencesModel->removeRow(ircNickToIndexMap[ircNick].row());
    ircNickToIndexMap.remove(ircNick);
}

void GamePreferencesDock::onUserChangedNick(const User& user, const QString& newNick)
{
    if (!ircNickToIndexMap.contains(user.nick)) {
        return;
    }
    ircNickToIndexMap[newNick] = ircNickToIndexMap[user.nick];
    ircNickToIndexMap.remove(user.nick);
}

void GamePreferencesDock::applySettings()
{
    QVector<bool> hideColumns = GamePreferencesTablePage::getHiddenColumns();
    for (int i = 0; i < preferencesModel->columnCount(); i++) {
        if (hideColumns[i]) {
            preferencesView->hideColumn(i);
        } else {
            preferencesView->showColumn(i);
        }
    }
    updateOurPlayerName(GamePreferencesGeneralPage::getInGameNick());
}

void GamePreferencesDock::showSettingsDialog()
{
    GamePreferencesSettingsDialog settings(this);
    settings.showDialog();
}

void GamePreferencesDock::onNickAlreadyInUse()
{
    ircClient->sendRaw(QString("NICK %1").arg(getRandomIrcNick()));
}

QString GamePreferencesDock::getRandomIrcNick()
{
    static bool init = false;
    if (!init) {
        srand(QDateTime::currentDateTime().toTime_t());
        init = true;
    }
    return QString("nfkl_gp%1").arg(rand() % 100000);
}
