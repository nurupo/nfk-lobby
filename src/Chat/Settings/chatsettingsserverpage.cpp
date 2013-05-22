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

#include "chatsettingsmodifyserverdialog.hpp"
#include "chatsettingsserverpage.hpp"

#include <QPushButton>
#include <QStandardItemModel>
#include <QVBoxLayout>

QStandardItemModel* ChatSettingsServerPage::serverListModel = 0;
QList<ChatSettingsServerPage::Server> ChatSettingsServerPage::serverList;

ChatSettingsServerPage::ChatSettingsServerPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath) :
    AbstractSettingsPage(parent, pageName, iconPath)
{
    modifiedServerList = QList<Server>(serverList);
}

void ChatSettingsServerPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* serverGroup = buildServerGroup();

    layout->addWidget(serverGroup);
}

QGroupBox* ChatSettingsServerPage::buildServerGroup()
{
    QGroupBox* group = new QGroupBox("Servers", this);
    QGridLayout* groupLayout = new QGridLayout(group);

    serverListView = new QTreeView(group);
    serverListView->setModel(serverListModel);
    serverListView->setIndentation(0);
    serverListModel->setHorizontalHeaderLabels(QStringList() << "Name");

    QPushButton* serverAddButton = new QPushButton("Add", group);
    QPushButton* serverRemoveButton = new QPushButton("Remove", group);
    QPushButton* serverEditButton = new QPushButton("Edit", group);

    connect(serverAddButton, &QPushButton::clicked, this, &ChatSettingsServerPage::serverAddButtonClicked);
    connect(serverRemoveButton, &QPushButton::clicked, this, &ChatSettingsServerPage::serverRemoveButtonClicked);
    connect(serverEditButton, &QPushButton::clicked, this, &ChatSettingsServerPage::serverEditButtonClicked);
    groupLayout->addWidget(serverListView,     0, 0, 6, 3);
    groupLayout->addWidget(serverAddButton,    0, 3, 1, 1);
    groupLayout->addWidget(serverEditButton,   1, 3, 1, 1);
    groupLayout->addWidget(serverRemoveButton, 2, 3, 1, 1);

    return group;
}

void ChatSettingsServerPage::setGui()
{
}

void ChatSettingsServerPage::apply()
{
    serverList = QList<Server>(modifiedServerList);
    delete serverListModel;
    buildServerModel();
}

void ChatSettingsServerPage::load(QSettings& settings)
{
    int serverListSize = settings.beginReadArray("serverList");
    for (int i = 0; i < serverListSize; i ++) {
        settings.setArrayIndex(i);
        Server server;
        server.name = settings.value("name").toString();
        server.address = settings.value("address").toString();
        server.port = settings.value("port").toInt();
        server.password = settings.value("password").toString();
        server.encoding = settings.value("encoding").toString();
        serverList << server;
    }
    settings.endArray();

    buildServerModel();
}

void ChatSettingsServerPage::buildServerModel()
{
    QStandardItemModel* model = new QStandardItemModel();
    for (int i = 0; i < serverList.size(); i ++) {
        QStandardItem* serverItem = new QStandardItem(serverList[i].name);
        model->appendRow(serverItem);
    }
    serverListModel = model;
}

void ChatSettingsServerPage::save(QSettings& settings)
{
    settings.beginWriteArray("serverList");
    for (int i = 0; i < serverList.size(); i ++) {
        settings.setArrayIndex(i);
        settings.setValue("name", serverList[i].name);
        settings.setValue("address", serverList[i].address);
        settings.setValue("port", serverList[i].port);
        settings.setValue("password", serverList[i].password);
        settings.setValue("encoding", serverList[i].encoding);
    }
    settings.endArray();
}

void ChatSettingsServerPage::serverAddButtonClicked()
{
    ChatSettingsModifyServerDialog serverInfoDialog(this);
    if (serverInfoDialog.exec() == QDialog::Accepted) {
        Server serverInfo = serverInfoDialog.getServerInformation();
        QStandardItem* name = new QStandardItem(serverInfo.name);
        serverListModel->appendRow(name);
        serverListView->setCurrentIndex(serverListModel->index(serverListModel->rowCount() - 1, 0));
        modifiedServerList << serverInfo;
    }
}

void ChatSettingsServerPage::serverRemoveButtonClicked()
{
    QModelIndex currentIndex = serverListView->currentIndex();
    if (currentIndex != QModelIndex()) {
        serverListModel->removeRow(currentIndex.row());
        modifiedServerList.removeAt(currentIndex.row());
    }
}

void ChatSettingsServerPage::serverEditButtonClicked()
{
    QModelIndex currentIndex = serverListView->currentIndex();
    if (currentIndex == QModelIndex()) {
        return;
    }
    ChatSettingsModifyServerDialog serverInfoDialog(this);
    Server oldServerInfo = modifiedServerList[currentIndex.row()];
    serverInfoDialog.setServerInformation(oldServerInfo);
    if (serverInfoDialog.exec() == QDialog::Accepted) {
        Server newServerInfo = serverInfoDialog.getServerInformation();
        serverListModel->setData(currentIndex, newServerInfo.name);
        modifiedServerList[currentIndex.row()] = newServerInfo;
    }
}

