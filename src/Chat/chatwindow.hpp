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

#ifndef CHATWINDOW_HPP
#define CHATWINDOW_HPP

#include "../IrcClient/ircclient.hpp"
#include "Pages/channelpage.hpp"
#include "Pages/privatepage.hpp"
#include "Pages/serverpage.hpp"
#include "topiclabel.hpp"
#include "usertree.hpp"

#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTreeView>

namespace Chat {

class Window : public QMainWindow
{
    Q_OBJECT
public:
    Window(QWidget* parent = 0);
    ~Window();

    static IrcClient::IrcClient* ircClient;

private:
    QString getSelectedTabName() const;
    ChannelPage* createChannelPage(const QString &channelName);
    PrivatePage* createPrivatePage(const QString &pageName);
    void deleteChannelPage(const QString& channelName);
    void switchToTab(QTreeWidgetItem* tab);

    QDockWidget*        topicDock;
    TopicLabel*         topicLine;
    QDockWidget*        userDock;
    QTreeWidget*        tabTree;
    QTreeWidgetItem*    serverTab;
    QLabel*             nickLabel;
    QLineEdit*          inputLine;
    QStackedWidget*     pages;
    ServerPage*         serverPage;
    UserTree*           userTree;
    QMenu*              channelContextMenu;
    QMenu*              privateContextMenu;
    QTimer*             autojoinTimer;

    QHash<QString, ChannelPage*> channelPages;
    QHash<QString, PrivatePage*> privatePages;

    PrivatePage* getPrivatePage(QString &pageName);

private slots:
    void changeTopic(const QString& topicText);
    void joinedChannel(const IrcClient::Channel& channel);

    void startPrivate(const QString &nick);
    void sendMessage();
    void showNicksFor(const QString& channel);
    void tabSelected();
    void connected();
    void showTabTreeContextMenu(const QPoint &pos);
    void partChannelAction();
    void joinChannelAction();
    void closeChannelAction();
    void disconnectFromServer();
    void connectToServer();
    void routePrivateMessage(const IrcClient::User &sender, const QString &message, const QString &target);
    void routePrivateAction(const IrcClient::User &sender, const QString &message, const QString &target);
    void deletePrivatePage();
    //void routePart(const Channel& channel);
    void showSettingsDialog();
    void applySettings();
    void joinChannels();

};

} // namespace Chat

#endif // CHATWINDOW_HPP
