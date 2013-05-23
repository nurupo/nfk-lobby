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

#include "chatdock.hpp"
#include "chattabtree.hpp"
#include "mainwindow.hpp"
#include "Settings/chatsettingschannelpage.hpp"
#include "Settings/chatsettingsdialog.hpp"
#include "Settings/chatsettingsfontpage.hpp"
#include "Settings/chatsettingsgeneralpage.hpp"
#include "Settings/chatsettingsserverpage.hpp"

#include "usertree.hpp"
#include <QAction>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QLineEdit>
#include <QMainWindow>
#include <QRegularExpression>
#include <QStackedWidget>
#include <QTextEdit>
#include <QTimer>
#include <QToolBar>
#include <QTreeWidget>

IrcClient* ChatDock::ircClient = new IrcClient();

ChatDock::ChatDock(QWidget* parent) :
    QDockWidget(parent)
{
    ChatSettingsDialog::load();
    connect(&ChatSettingsDialog::settingsNotifier, &BasicSettingsDialogNotifier::updated, this, &ChatDock::applySettings);

    const QString dockName = "Chat";
    setObjectName(dockName);
    setWindowTitle(dockName);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

    QMainWindow* window = new QMainWindow(0);

    QToolBar* toolbar = new QToolBar(window);
    toolbar->setIconSize(QSize(24, 24));
    toolbar->setFloatable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    window->addToolBar(toolbar);

    //QAction* refreshAction = toolbar->addAction(QIcon(":/icons/refresh.png"), "refresh");
    //connect(refreshAction, SIGNAL(triggered()), this, SLOT(refreshPlanets()));

    QDockWidget* inputDock = new QDockWidget(window);
    inputDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    inputDock->setTitleBarWidget(new QWidget(inputDock));
    inputDock->setContextMenuPolicy(Qt::PreventContextMenu);
    window->addDockWidget(Qt::BottomDockWidgetArea, inputDock);
    QWidget* inputDockWidget = new QWidget(inputDock);
    QHBoxLayout* inputDockWidgetLayout = new QHBoxLayout(inputDockWidget);
    nickLabel = new QLabel(inputDockWidget);
    nickLabel->hide();
    inputLine = new QLineEdit(inputDockWidget);

    connect(inputLine, &QLineEdit::returnPressed, this, &ChatDock::sendMessage);
    inputDockWidgetLayout->addWidget(nickLabel);
    inputDockWidgetLayout->addWidget(inputLine);
    inputDockWidgetLayout->setContentsMargins(2, 2, 2, 6);
    inputDockWidget->setLayout(inputDockWidgetLayout);
    inputDock->setFixedHeight(inputDock->height());
    inputDock->setWidget(inputDockWidget);

    QDockWidget* tabDock = new QDockWidget(window);
    tabDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    tabDock->setTitleBarWidget(new QWidget(tabDock));
    tabDock->setContextMenuPolicy(Qt::PreventContextMenu);
    window->addDockWidget(Qt::LeftDockWidgetArea, tabDock);
    window->setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    window->setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    tabTree = new ChatTabTree(tabDock, 100);
    tabTree->setHeaderLabel("Chats");
    tabTree->setIndentation(8);
    tabTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabTree->setMinimumWidth(1);
    tabTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    tabDock->setWidget(tabTree);
    tabTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabTree, &QTreeWidget::itemSelectionChanged, this, &ChatDock::tabSelected);
    connect(tabTree, &QTreeWidget::customContextMenuRequested, this, &ChatDock::showTabTreeContextMenu);

    QAction* connectAction = new QAction(QIcon(":/icons/connect.png"), "Connect", toolbar);
    QAction* disconnectAction = new QAction(QIcon(":/icons/disconnect.png"), "Disconnect", toolbar);
    QAction* settingsAction = toolbar->addAction(QIcon(":/icons/settings.png"), "Settings");
    connect(connectAction, &QAction::triggered, this, &ChatDock::connectToServer);
    connect(disconnectAction, &QAction::triggered, this, &ChatDock::disconnectFromServer);
    connect(settingsAction, &QAction::triggered, this, &ChatDock::showSettingsDialog);
    toolbar->addActions(QList<QAction*>() << connectAction << disconnectAction << settingsAction);

    serverTab = new QTreeWidgetItem(tabTree, QStringList() << "IRC Server");
    tabTree->addTopLevelItem(serverTab);

    userDock = new QDockWidget(window);
    userDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    userDock->setTitleBarWidget(new QWidget(userDock));
    userDock->setContextMenuPolicy(Qt::PreventContextMenu);
    window->addDockWidget(Qt::RightDockWidgetArea, userDock);
    window->setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    window->setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    userTree = new UserTree(userDock, 100);
    userTree->setItemsExpandable(false);
    userTree->setIndentation(8);
    userTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    userTree->setMinimumWidth(1);
    userTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    userDock->setWidget(userTree);
    connect(userTree, &UserTree::privateActionTriggered, this, &ChatDock::startPrivate);

    topicDock = new QDockWidget(window);
    topicDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    topicDock->setTitleBarWidget(new QWidget(topicDock));
    topicDock->setContextMenuPolicy(Qt::PreventContextMenu);
    window->addDockWidget(Qt::TopDockWidgetArea, topicDock);
    topicLine = new TopicLabel(topicDock);
    topicDock->setWidget(topicLine);

    QMainWindow* pagesWindow = new QMainWindow(0);
    pages = new QStackedWidget(pagesWindow);

    serverPage = new ServerPage(serverTab, tabTree);
    connect(serverPage, &ServerPage::connectActionTriggered,    this, &ChatDock::connectToServer);
    connect(serverPage, &ServerPage::disconnectActionTriggered, this, &ChatDock::disconnectFromServer);

    pagesWindow->setCentralWidget(pages);
    pages->addWidget(serverPage);

    window->setCentralWidget(pagesWindow);
    window->setParent(this);
    setWidget(window);

    tabTree->setItemSelected(serverTab, true);

    autojoinTimer = new QTimer(this);
    autojoinTimer->setSingleShot(true);
    connect(autojoinTimer, &QTimer::timeout, this, &ChatDock::joinChannels);
    applySettings();

    ircClient->setVersion(QString("%1 %2 by %3; build date: %4").arg(MainWindow::name).arg(MainWindow::version).arg(MainWindow::author).arg(MainWindow::buildDate));

    connect(ircClient, &IrcClient::notice,             serverPage, &ServerPage::notice);
    connect(ircClient, &IrcClient::serverResponse,     serverPage, &ServerPage::serverResponse);
    connect(ircClient, &IrcClient::ctcpRequest,        serverPage, &ServerPage::ctcpRequest);
    connect(ircClient, &IrcClient::ctcpReply,          serverPage, &ServerPage::ctcpReply);
    connect(ircClient, &IrcClient::quit,               serverPage, &ServerPage::quit);
    connect(ircClient, &IrcClient::connecting,         serverPage, &ServerPage::connecting);
    connect(ircClient, &IrcClient::disconnected,       serverPage, &ServerPage::disconnected);
    connect(ircClient, &IrcClient::userModeChanged,    serverPage, &ServerPage::userModeChanged);
    connect(ircClient, &IrcClient::join,               this,       &ChatDock::joinedChannel);
    connect(ircClient, &IrcClient::connected,          this,       &ChatDock::connected);
    connect(ircClient, &IrcClient::privateMessage,     this,       &ChatDock::routePrivateMessage);
    connect(ircClient, &IrcClient::action,             this,       &ChatDock::routePrivateAction);
    connect(ircClient, &IrcClient::nickChange,         nickLabel,  &QLabel::setText);
    connect(ircClient, &IrcClient::connected,          nickLabel,  &QLabel::show);
    connect(ircClient, &IrcClient::disconnected,       nickLabel,  &QLabel::hide);
    if (ChatSettingsGeneralPage::getAutoConnect()) {
        connectToServer();
    }
}

ChatDock::~ChatDock()
{
    QMutableHashIterator<QString, ChannelPage*> iterator (channelPages);
    while (iterator.hasNext()) {
        delete iterator.next().value();
    }
    delete ircClient;
}

void ChatDock::joinedChannel(const Channel& channel)
{
    ChannelPage* channelPage;

    if (channelPages.contains(channel.getName())) {
        channelPage = channelPages[channel.getName()];
        channelPage->enable();
    } else {
        channelPage = createChannelPage(channel.getName());
    }

    connect(&channel, &Channel::topicTextResponse,  channelPage, &ChannelPage::topicTextResponse);
    connect(&channel, &Channel::topicInfoResponse,  channelPage, &ChannelPage::topicInfoResponse);
    connect(&channel, &Channel::nicksReady,         channelPage, &ChannelPage::nicksReady);
    connect(&channel, &Channel::channelMessage,     channelPage, &ChannelPage::channelMessage);
    connect(&channel, &Channel::action,             channelPage, &ChannelPage::action);
    connect(&channel, &Channel::join,               channelPage, &ChannelPage::join);
    connect(&channel, &Channel::nickChange,         channelPage, &ChannelPage::nickChange);
    connect(&channel, &Channel::notice,             channelPage, &ChannelPage::notice);
    connect(&channel, &Channel::part,               channelPage, &ChannelPage::part);
    connect(&channel, &Channel::quit,               channelPage, &ChannelPage::quit);
    connect(&channel, &Channel::topicChanged,       channelPage, &ChannelPage::topicChanged);
    connect(&channel, &Channel::modeChanged,        channelPage, &ChannelPage::modeChanged);
    connect(&channel, &Channel::kick,               channelPage, &ChannelPage::kick);
    connect(&channel, &Channel::topicChanged,       this,        &ChatDock::changeTopic);
    connect(&channel, &Channel::topicTextResponse,  this,        &ChatDock::changeTopic);
    connect(&channelPage->getUserTreeModel(), &UserTreeModel::branchAdded, userTree, &UserTree::expand);

    switchToTab(channelPage->getTab());
}

void ChatDock::switchToTab(QTreeWidgetItem* tab)
{
    QList<QTreeWidgetItem*> selectedItems = tabTree->selectedItems();
    for (int i = 0; i < selectedItems.size(); i++) {
        selectedItems[i]->setSelected(false);
    }
    tab->setSelected(true);
}

void ChatDock::startPrivate(const QString &nick)
{
    PrivatePage* privatePage = createPrivatePage(nick);
    switchToTab(privatePage->getTab());
}

/*This method should be only invoked from joinedChannel because it sets needed connects*/
ChannelPage* ChatDock::createChannelPage(const QString& channelName)
{
    ChannelPage* channelPage;
    QTreeWidgetItem* channelTab;
    channelTab = new QTreeWidgetItem(serverTab);
    channelTab->setText(0, channelName);
    serverTab->addChild(channelTab);
    channelPage = new ChannelPage(channelTab, tabTree);
    connect(ircClient, &IrcClient::disconnected, channelPage, &ChannelPage::disable);
    connect(channelPage, &ChannelPage::partActionTriggered, this, &ChatDock::partChannelAction);
    connect(channelPage, &ChannelPage::joinActionTriggered, this, &ChatDock::joinChannelAction);
    connect(channelPage, &ChannelPage::closeActionTriggered, this, &ChatDock::closeChannelAction);
    pages->addWidget(channelPage);
    if (!channelPages.size()) {
        tabTree->expandAll();
    }
    channelPages[channelName] = channelPage;
    return channelPage;
}

/*TODO: add Close action to the Channel tabContextMenu and trigger removeChannel*/

void ChatDock::deleteChannelPage(const QString& channelName)
{
    ChannelPage* channelPage = channelPages.take(channelName);
    QTreeWidgetItem* channelTab = channelPage->getTab();
    channelPages.remove(channelName);
    serverTab->removeChild(channelTab);
    delete channelTab;
    pages->removeWidget(channelPage);
    delete channelPage;
}


void ChatDock::tabSelected()
{
    QList<QTreeWidgetItem*> selectedItems = tabTree->selectedItems();

    if (selectedItems.size() != 1) {
        return;
    }

    if (!selectedItems.at(0)->parent()) {
        /*server tab*/
        userDock->hide();
        topicDock->hide();
        pages->setCurrentIndex(0);
        serverPage->clearTabIndication();
        inputLine->setFocus();
        return;
    }

    QString tabName = getSelectedTabName();
    if (channelPages.contains(tabName)) {
        /*channel tab*/
        showNicksFor(tabName);
        ChannelPage* channelPage = channelPages[tabName];
        if (channelPage->isDisabled()) {
            topicLine->setText(QString());
        } else {
            QString topic(ircClient->getChannel(tabName).getTopicText());
            BasicPage::eraseControlCharacters(topic);
            topicLine->setText(topic);
        }
        userDock->show();
        topicDock->show();
        channelPages[tabName]->clearTabIndication();
    } else if (privatePages.contains(tabName)){
        /*user tab*/
        userDock->hide();
        topicDock->hide();
        privatePages[tabName]->clearTabIndication();
    }

    for (int i = 1; i < pages->count(); i ++) {
        const BasicPage* page = static_cast<BasicPage*>(pages->widget(i));
        if (!tabName.compare(page->getName())) {
            pages->setCurrentIndex(i);
            break;
        }
    }
    inputLine->setFocus();
}

void ChatDock::showNicksFor(const QString& channel)
{
    userTree->setModel(&channelPages[channel]->getUserTreeModel());
    userTree->expandAll();
}

void ChatDock::changeTopic(const QString& topicText)
{
    const Channel* channel = static_cast<Channel*>(sender());
    QString tabName = getSelectedTabName();
    if (tabName == channel->getName()) {
        topicLine->setText(topicText);
    }
}

void ChatDock::sendMessage()
{
    QString tabName = getSelectedTabName();
    if (tabName.isEmpty()) {
        /*TODO: Error message saying "no tab selected"*/
        return;
    }

    QString message = inputLine->text();
    inputLine->clear();
    if (!message.length()) {
        return;
    }

    bool serverTab = !tabTree->selectedItems().at(0)->parent();

    if (message.startsWith("//") && !serverTab) {
        ircClient->sendMessage(tabName, message.mid(1));
        return;
    }
    if (message.startsWith("/me ", Qt::CaseInsensitive) && !serverTab) {
        ircClient->sendAction(tabName, message.mid(4));
        return;
    }
    QRegularExpressionMatch matchCtcp = QRegularExpression("/ctcp (\\S+) (.+)", QRegularExpression::CaseInsensitiveOption).match(message);
    if (matchCtcp.hasMatch()) {
        ircClient->sendCtcpRequest(matchCtcp.captured(1), matchCtcp.captured(2));
        return;
    }
    if (message.startsWith("/ns ", Qt::CaseInsensitive)) {
        message.replace(QRegularExpression("^/ns"), "/nickserv");
    } else if (message.startsWith("/cs ", Qt::CaseInsensitive)) {
        message.replace(QRegularExpression("^/cs"), "/chanserv");
    }
    if (message.startsWith("/")) {
        message.replace(QRegularExpression("^/"), "");
        ircClient->sendRaw(message);
    } else if (!serverTab){
        ircClient->sendMessage(tabName, message);
    }
}

QString ChatDock::getSelectedTabName() const
{
    QTreeWidgetItem* tabItem = tabTree->selectedItems().at(0);
    return tabItem ? tabItem->text(0) : QString();
}

void ChatDock::connected()
{
    if (ChatSettingsGeneralPage::getAutoIdentify()) {
        ircClient->identify(ChatSettingsGeneralPage::getPassword());
    }
    if (ChatSettingsChannelPage::getAutojoinChannels()) {
        if (ChatSettingsChannelPage::getWaitBeforeAutojoining()) {
            autojoinTimer->start();
        } else {
            joinChannels();
        }
    }
}

void ChatDock::showTabTreeContextMenu(const QPoint &pos)
{
    QPoint globalPos = tabTree->viewport()->mapToGlobal(pos);
    globalPos.setX(globalPos.x() + 1);
    QList<QTreeWidgetItem*> selectedItems = tabTree->selectedItems();
    if (selectedItems.size() != 1) {
        return;
    }
    QTreeWidgetItem* selectedItem = selectedItems[0];
    QMenu* menu = 0;
    if (!selectedItem->parent()) {
        menu = serverPage->getTabContextMenu();
    } else {
        const QString tabName = getSelectedTabName();
        if (channelPages.contains(tabName)) {
            menu = channelPages[tabName]->getTabContextMenu();
        } else if (privatePages.contains(tabName)) {
            menu = privatePages[tabName]->getTabContextMenu();
        }
    }
    menu->exec(globalPos);
}

void ChatDock::connectToServer()
{
    //ircSrv->connect("irc.wenet.ru", 6667, "", "wolferiac", "username", "windows-1251");
    const int& serverId = ChatSettingsGeneralPage::getServerId();
    const QList<ChatSettingsServerPage::Server> servers = ChatSettingsServerPage::getServerList();
    if (serverId >= 0 && serverId < servers.size()) {
        ircClient->connect(servers[serverId].address, servers[serverId].port, servers[serverId].password, ChatSettingsGeneralPage::getNick(), ChatSettingsGeneralPage::getUsername(), servers[serverId].encoding);
    }
}

void ChatDock::joinChannels()
{
    const QList<ChatSettingsChannelPage::Channel>& channels = ChatSettingsChannelPage::getAutojoinChannelList();
    for (int i = 0; i < channels.size(); i ++) {
        ircClient->joinChannel(channels[i].name, channels[i].password);
    }
}

void ChatDock::disconnectFromServer()
{
    ircClient->disconnect();
}

void ChatDock::partChannelAction()
{
    QString channel = getSelectedTabName();
    /*rename sendPart to partChannel*/
    ircClient->sendPart(channel);
}

void ChatDock::joinChannelAction()
{
    QString channel = getSelectedTabName();
    ircClient->joinChannel(channel);
}

void ChatDock::routePrivateMessage(const User &sender, const QString &message, const QString &target)
{
    QString pageName;
    if (sender.nick == ircClient->getUs().nick) {
        pageName = target;
    } else {
        pageName = sender.nick;
    }

    getPrivatePage(pageName)->privateMessage(sender, message);
}

void ChatDock::routePrivateAction(const User &sender, const QString &message, const QString &target)
{
    QString pageName;
    if (sender.nick == ircClient->getUs().nick) {
        pageName = target;
    } else {
        pageName = sender.nick;
    }

    getPrivatePage(pageName)->action(sender, message);
}

PrivatePage* ChatDock::getPrivatePage(QString &pageName)
{
    PrivatePage* privatePage;
    if (!privatePages.contains(pageName)) {
        privatePage = createPrivatePage(pageName);
    } else {
        privatePage = privatePages[pageName];
    }
    return privatePage;
}

PrivatePage* ChatDock::createPrivatePage(const QString &pageName)
{
    if (!privatePages.contains(pageName)) {
        PrivatePage* privatePage;
        QTreeWidgetItem* privateTab = new QTreeWidgetItem(serverTab);
        privateTab->setText(0, pageName);
        serverTab->addChild(privateTab);
        privatePage = new PrivatePage(privateTab, tabTree);
        connect(ircClient, &IrcClient::disconnected,   privatePage, &PrivatePage::disable);
        connect(ircClient, &IrcClient::connected,      privatePage, &PrivatePage::enable);
        connect(privatePage, &PrivatePage::closeActionTriggered, this, &ChatDock::deletePrivatePage);
        pages->addWidget(privatePage);
        if (!privatePages.size()) {
            tabTree->expandAll();
        }
        privatePages[pageName] = privatePage;
        return privatePage;
    } else {
        return 0;
    }
}

void ChatDock::deletePrivatePage()
{
    PrivatePage* privatePage = privatePages.take(getSelectedTabName());
    QTreeWidgetItem* privateTab = privatePage->getTab();
    serverTab->removeChild(privateTab);
    delete privateTab;
    pages->removeWidget(privatePage);
    delete privatePage;
}

void ChatDock::showSettingsDialog()
{
    ChatSettingsDialog settings(this);
    settings.showDialog();
}

void ChatDock::applySettings()
{
    tabTree->setFont(ChatSettingsFontPage::getChatListFont());
    userTree->setFont(ChatSettingsFontPage::getUserListFont());
    ircClient->setQuitMessage(ChatSettingsGeneralPage::getQuitMessage());
    autojoinTimer->setInterval(ChatSettingsChannelPage::getTimeToWaitBeforeAutojoining() * 1000);
}

void ChatDock::closeChannelAction()
{
    QString channel = getSelectedTabName();
    /*rename sendPart to partChannel*/
    ircClient->sendPart(channel);
    deleteChannelPage(channel);
}
