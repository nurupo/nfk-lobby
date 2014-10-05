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

#include "../Settings/settings.hpp"
#include "chattabtree.hpp"
#include "chatwindow.hpp"
#include "mainwindow.hpp"
#include "Settings/chatsettings.hpp"
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

namespace Chat {

IrcClient::IrcClient* Window::ircClient = new IrcClient::IrcClient();

Window::Window(QWidget* parent) :
    QMainWindow(parent)
{
    setObjectName("ChatWindow");

    Settings::getInstance().load();
    connect(&Settings::getInstance(), &Settings::dataChanged, this, &Window::applySettings);

    QToolBar* toolbar = new QToolBar(this);
    toolbar->setIconSize(QSize(24, 24));
    toolbar->setFloatable(false);
    toolbar->setContextMenuPolicy(Qt::PreventContextMenu);
    addToolBar(toolbar);

    //QAction* refreshAction = toolbar->addAction(QIcon(":/icons/refresh.png"), "refresh");
    //connect(refreshAction, SIGNAL(triggered()), this, SLOT(refreshPlanets()));

    QDockWidget* inputDock = new QDockWidget(this);
    inputDock->setObjectName("Input dock");
    inputDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    inputDock->setTitleBarWidget(new QWidget(inputDock));
    inputDock->setContextMenuPolicy(Qt::PreventContextMenu);
    addDockWidget(Qt::BottomDockWidgetArea, inputDock);
    QWidget* inputDockWidget = new QWidget(inputDock);
    QHBoxLayout* inputDockWidgetLayout = new QHBoxLayout(inputDockWidget);
    nickLabel = new QLabel(inputDockWidget);
    nickLabel->hide();
    inputLine = new QLineEdit(inputDockWidget);

    connect(inputLine, &QLineEdit::returnPressed, this, &Window::sendMessage);
    inputDockWidgetLayout->addWidget(nickLabel);
    inputDockWidgetLayout->addWidget(inputLine);
    inputDockWidgetLayout->setContentsMargins(2, 2, 2, 6);
    inputDockWidget->setLayout(inputDockWidgetLayout);
    inputDock->setFixedHeight(inputDock->height());
    inputDock->setWidget(inputDockWidget);

    QDockWidget* tabDock = new QDockWidget(this);
    tabDock->setObjectName("Tab dock");
    tabDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    tabDock->setTitleBarWidget(new QWidget(tabDock));
    tabDock->setContextMenuPolicy(Qt::PreventContextMenu);
    addDockWidget(Qt::LeftDockWidgetArea, tabDock);
    setCorner(Qt::BottomLeftCorner, Qt::LeftDockWidgetArea);
    setCorner(Qt::TopLeftCorner, Qt::LeftDockWidgetArea);
    tabTree = new TabTree(tabDock, 100);
    tabTree->setHeaderLabel("Chats");
    tabTree->setIndentation(8);
    tabTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tabTree->setMinimumWidth(1);
    tabTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    tabDock->setWidget(tabTree);
    tabTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabTree, &QTreeWidget::itemSelectionChanged, this, &Window::tabSelected);
    connect(tabTree, &QTreeWidget::customContextMenuRequested, this, &Window::showTabTreeContextMenu);

    QAction* connectAction = new QAction(QIcon(":/icons/connect.png"), "Connect", toolbar);
    QAction* disconnectAction = new QAction(QIcon(":/icons/disconnect.png"), "Disconnect", toolbar);
    QAction* settingsAction = toolbar->addAction(QIcon(":/icons/settings.png"), "Settings");
    connect(connectAction, &QAction::triggered, this, &Window::connectToServer);
    connect(disconnectAction, &QAction::triggered, this, &Window::disconnectFromServer);
    connect(settingsAction, &QAction::triggered, this, &Window::showSettingsDialog);
    toolbar->addActions(QList<QAction*>() << connectAction << disconnectAction << settingsAction);

    serverTab = new QTreeWidgetItem(tabTree, QStringList() << "IRC Server");
    tabTree->addTopLevelItem(serverTab);

    userDock = new QDockWidget(this);
    userDock->setObjectName("User dock");
    userDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    userDock->setTitleBarWidget(new QWidget(userDock));
    userDock->setContextMenuPolicy(Qt::PreventContextMenu);
    addDockWidget(Qt::RightDockWidgetArea, userDock);
    setCorner(Qt::BottomRightCorner, Qt::RightDockWidgetArea);
    setCorner(Qt::TopRightCorner, Qt::RightDockWidgetArea);
    userTree = new UserTree(userDock, 100);
    userTree->setItemsExpandable(false);
    userTree->setIndentation(8);
    userTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    userTree->setMinimumWidth(1);
    userTree->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
    userDock->setWidget(userTree);
    connect(userTree, &UserTree::privateActionTriggered, this, &Window::startPrivate);

    topicDock = new QDockWidget(this);
    topicDock->setObjectName("Topic dock");
    topicDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    topicDock->setTitleBarWidget(new QWidget(topicDock));
    topicDock->setContextMenuPolicy(Qt::PreventContextMenu);
    addDockWidget(Qt::TopDockWidgetArea, topicDock);
    topicLine = new TopicLabel(topicDock);
    topicDock->setWidget(topicLine);

    QMainWindow* pagesWindow = new QMainWindow(0);
    pages = new QStackedWidget(pagesWindow);

    serverPage = new ServerPage(serverTab, tabTree);
    connect(serverPage, &ServerPage::connectActionTriggered,    this, &Window::connectToServer);
    connect(serverPage, &ServerPage::disconnectActionTriggered, this, &Window::disconnectFromServer);

    pagesWindow->setCentralWidget(pages);
    pages->addWidget(serverPage);

    setCentralWidget(pagesWindow);

    tabTree->setItemSelected(serverTab, true);

    autojoinTimer = new QTimer(this);
    autojoinTimer->setSingleShot(true);
    connect(autojoinTimer, &QTimer::timeout, this, &Window::joinChannels);
    applySettings();

    ircClient->setVersion(QString("%1 %2 by %3; build date: %4").arg(MainWindow::name).arg(MainWindow::version).arg(MainWindow::author).arg(MainWindow::buildDate));

    connect(ircClient, &IrcClient::IrcClient::notice,             serverPage, &ServerPage::notice);
    connect(ircClient, &IrcClient::IrcClient::serverResponse,     serverPage, &ServerPage::serverResponse);
    connect(ircClient, &IrcClient::IrcClient::ctcpRequest,        serverPage, &ServerPage::ctcpRequest);
    connect(ircClient, &IrcClient::IrcClient::ctcpReply,          serverPage, &ServerPage::ctcpReply);
    connect(ircClient, &IrcClient::IrcClient::quit,               serverPage, &ServerPage::quit);
    connect(ircClient, &IrcClient::IrcClient::connecting,         serverPage, &ServerPage::connecting);
    connect(ircClient, &IrcClient::IrcClient::disconnected,       serverPage, &ServerPage::disconnected);
    connect(ircClient, &IrcClient::IrcClient::userModeChanged,    serverPage, &ServerPage::userModeChanged);
    connect(ircClient, &IrcClient::IrcClient::join,               this,       &Window::joinedChannel);
    connect(ircClient, &IrcClient::IrcClient::connected,          this,       &Window::connected);
    connect(ircClient, &IrcClient::IrcClient::privateMessage,     this,       &Window::routePrivateMessage);
    connect(ircClient, &IrcClient::IrcClient::action,             this,       &Window::routePrivateAction);
    connect(ircClient, &IrcClient::IrcClient::nickChange,         nickLabel,  &QLabel::setText);
    connect(ircClient, &IrcClient::IrcClient::connected,          nickLabel,  &QLabel::show);
    connect(ircClient, &IrcClient::IrcClient::disconnected,       nickLabel,  &QLabel::hide);
    if (Settings::getInstance().getAutoConnect()) {
        connectToServer();
    }

    ::Settings::loadWindow(this);
}

Window::~Window()
{

    QMutableHashIterator<QString, ChannelPage*> iterator (channelPages);
    while (iterator.hasNext()) {
        delete iterator.next().value();
    }
    delete ircClient;

    ::Settings::saveWindow(this);
}

void Window::joinedChannel(const IrcClient::Channel& channel)
{
    ChannelPage* channelPage;

    if (channelPages.contains(channel.getName())) {
        channelPage = channelPages[channel.getName()];
        channelPage->enable();
    } else {
        channelPage = createChannelPage(channel.getName());
    }

    connect(&channel, &IrcClient::Channel::topicTextResponse,  channelPage, &ChannelPage::topicTextResponse);
    connect(&channel, &IrcClient::Channel::topicInfoResponse,  channelPage, &ChannelPage::topicInfoResponse);
    connect(&channel, &IrcClient::Channel::nicksReady,         channelPage, &ChannelPage::nicksReady);
    connect(&channel, &IrcClient::Channel::channelMessage,     channelPage, &ChannelPage::channelMessage);
    connect(&channel, &IrcClient::Channel::action,             channelPage, &ChannelPage::action);
    connect(&channel, &IrcClient::Channel::join,               channelPage, &ChannelPage::join);
    connect(&channel, &IrcClient::Channel::nickChange,         channelPage, &ChannelPage::nickChange);
    connect(&channel, &IrcClient::Channel::notice,             channelPage, &ChannelPage::notice);
    connect(&channel, &IrcClient::Channel::part,               channelPage, &ChannelPage::part);
    connect(&channel, &IrcClient::Channel::quit,               channelPage, &ChannelPage::quit);
    connect(&channel, &IrcClient::Channel::topicChanged,       channelPage, &ChannelPage::topicChanged);
    connect(&channel, &IrcClient::Channel::modeChanged,        channelPage, &ChannelPage::modeChanged);
    connect(&channel, &IrcClient::Channel::kick,               channelPage, &ChannelPage::kick);
    connect(&channel, &IrcClient::Channel::topicChanged,       this,        &Window::changeTopic);
    connect(&channel, &IrcClient::Channel::topicTextResponse,  this,        &Window::changeTopic);
    connect(&channelPage->getUserTreeModel(), &UserTreeModel::branchAdded, userTree, &UserTree::expand);

    switchToTab(channelPage->getTab());
}

void Window::switchToTab(QTreeWidgetItem* tab)
{
    QList<QTreeWidgetItem*> selectedItems = tabTree->selectedItems();
    for (int i = 0; i < selectedItems.size(); i++) {
        selectedItems[i]->setSelected(false);
    }
    tab->setSelected(true);
}

void Window::startPrivate(const QString &nick)
{
    PrivatePage* privatePage = getPrivatePage(nick);
    switchToTab(privatePage->getTab());
}

/*This method should be only invoked from joinedChannel because it sets needed connects*/
ChannelPage* Window::createChannelPage(const QString& channelName)
{
    ChannelPage* channelPage;
    QTreeWidgetItem* channelTab;
    channelTab = new QTreeWidgetItem(serverTab);
    channelTab->setText(0, channelName);
    serverTab->addChild(channelTab);
    channelPage = new ChannelPage(channelTab, tabTree);
    connect(ircClient, &IrcClient::IrcClient::disconnected, channelPage, &ChannelPage::disable);
    connect(channelPage, &ChannelPage::partActionTriggered, this, &Window::partChannelAction);
    connect(channelPage, &ChannelPage::joinActionTriggered, this, &Window::joinChannelAction);
    connect(channelPage, &ChannelPage::closeActionTriggered, this, &Window::closeChannelAction);
    pages->addWidget(channelPage);
    if (!channelPages.size()) {
        tabTree->expandAll();
    }
    channelPages[channelName] = channelPage;
    return channelPage;
}

/*TODO: add Close action to the Channel tabContextMenu and trigger removeChannel*/

void Window::deleteChannelPage(const QString& channelName)
{
    ChannelPage* channelPage = channelPages.take(channelName);
    QTreeWidgetItem* channelTab = channelPage->getTab();
    channelPages.remove(channelName);
    serverTab->removeChild(channelTab);
    delete channelTab;
    pages->removeWidget(channelPage);
    delete channelPage;
}


void Window::tabSelected()
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

void Window::showNicksFor(const QString& channel)
{
    userTree->setModel(&channelPages[channel]->getUserTreeModel());
    userTree->expandAll();
}

void Window::changeTopic(const QString& topicText)
{
    const IrcClient::Channel* channel = static_cast<IrcClient::Channel*>(sender());
    QString tabName = getSelectedTabName();
    if (tabName == channel->getName()) {
        topicLine->setText(topicText);
    }
}

void Window::sendMessage()
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

QString Window::getSelectedTabName() const
{
    QTreeWidgetItem* tabItem = tabTree->selectedItems().at(0);
    return tabItem ? tabItem->text(0) : QString();
}

void Window::connected()
{
    const Settings& settings = Settings::getInstance();
    if (settings.getAutoIdentify()) {
        ircClient->identify(settings.getPassword());
    }
    if (settings.getAutojoinChannels()) {
        if (settings.getWaitBeforeAutojoining()) {
            autojoinTimer->start();
        } else {
            joinChannels();
        }
    }
}

void Window::showTabTreeContextMenu(const QPoint &pos)
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

void Window::connectToServer()
{
    const Settings& settings = Settings::getInstance();
    //ircSrv->connect("irc.wenet.ru", 6667, "", "wolferiac", "username", "windows-1251");
    const int serverId = settings.getServerId();
    const QList<Settings::Server>& servers = settings.getServerList();
    if (serverId >= 0 && serverId < servers.size()) {
        ircClient->connect(servers[serverId].address, servers[serverId].port, servers[serverId].password, settings.getNick(), settings.getUsername(), servers[serverId].encoding);
    }
}

void Window::joinChannels()
{
    const QList<Settings::Channel>& channels = Settings::getInstance().getAutojoinChannelList();
    for (const Settings::Channel& channel : channels) {
        ircClient->joinChannel(channel.name, channel.password);
    }
}

void Window::disconnectFromServer()
{
    ircClient->disconnect();
}

void Window::partChannelAction()
{
    QString channel = getSelectedTabName();
    /*rename sendPart to partChannel*/
    ircClient->sendPart(channel);
}

void Window::joinChannelAction()
{
    QString channel = getSelectedTabName();
    ircClient->joinChannel(channel);
}

void Window::routePrivateMessage(const IrcClient::User &sender, const QString &message, const QString &target)
{
    QString pageName;
    if (sender.nick == ircClient->getUs().nick) {
        pageName = target;
    } else {
        pageName = sender.nick;
    }

    getPrivatePage(pageName)->privateMessage(sender, message);
}

void Window::routePrivateAction(const IrcClient::User &sender, const QString &message, const QString &target)
{
    QString pageName;
    if (sender.nick == ircClient->getUs().nick) {
        pageName = target;
    } else {
        pageName = sender.nick;
    }

    getPrivatePage(pageName)->action(sender, message);
}

PrivatePage* Window::getPrivatePage(const QString &pageName)
{
    PrivatePage* privatePage;
    if (!privatePages.contains(pageName)) {
        privatePage = createPrivatePage(pageName);
    } else {
        privatePage = privatePages[pageName];
    }
    return privatePage;
}

PrivatePage* Window::createPrivatePage(const QString &pageName)
{
    if (!privatePages.contains(pageName)) {
        PrivatePage* privatePage;
        QTreeWidgetItem* privateTab = new QTreeWidgetItem(serverTab);
        privateTab->setText(0, pageName);
        serverTab->addChild(privateTab);
        privatePage = new PrivatePage(privateTab, tabTree);
        connect(ircClient, &IrcClient::IrcClient::disconnected,   privatePage, &PrivatePage::disable);
        connect(ircClient, &IrcClient::IrcClient::connected,      privatePage, &PrivatePage::enable);
        connect(privatePage, &PrivatePage::closeActionTriggered, this, &Window::deletePrivatePage);
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

void Window::deletePrivatePage()
{
    PrivatePage* privatePage = privatePages.take(getSelectedTabName());
    QTreeWidgetItem* privateTab = privatePage->getTab();
    serverTab->removeChild(privateTab);
    delete privateTab;
    pages->removeWidget(privatePage);
    delete privatePage;
}

void Window::showSettingsDialog()
{
    Settings::getInstance().executeSettingsDialog(this);
}

void Window::applySettings()
{
    const Settings& settings = Settings::getInstance();
    tabTree->setFont(settings.getChatListFont());
    userTree->setFont(settings.getUserListFont());
    ircClient->setQuitMessage(settings.getQuitMessage());
    autojoinTimer->setInterval(settings.getTimeToWaitBeforeAutojoining() * 1000);
}

void Window::closeChannelAction()
{
    QString channel = getSelectedTabName();
    //FIXME: rename sendPart to partChannel
    ircClient->sendPart(channel);
    deleteChannelPage(channel);
}

} // namespace Chat

