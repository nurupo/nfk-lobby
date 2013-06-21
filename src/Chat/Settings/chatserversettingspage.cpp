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

#include "chatserversettingspage.hpp"
#include "chatsettingsmodifyserverdialog.hpp"
#include "customhinttreeview.hpp"

#include <QPushButton>
#include <QStandardItemModel>
#include <QVBoxLayout>

namespace Chat {

ServerSettingsPage::ServerSettingsPage(QWidget *parent, QStandardItemModel* serverListModel) :
    AbstractSettingsPage(parent), serverListModel(serverListModel)
{
}

void ServerSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* serverGroup = buildServerGroup();

    layout->addWidget(serverGroup);
}

QGroupBox* ServerSettingsPage::buildServerGroup()
{
    QGroupBox* group = new QGroupBox("Servers", this);
    QGridLayout* layout = new QGridLayout(group);

    serverListView = new CustomHintTreeView(group, QSize(10, 10));
    serverListView->setModel(serverListModel);
    serverListView->setIndentation(0);
    serverListModel->setHorizontalHeaderLabels(QStringList() << "Name");

    QPushButton* serverAddButton = new QPushButton("Add", group);
    QPushButton* serverRemoveButton = new QPushButton("Remove", group);
    QPushButton* serverEditButton = new QPushButton("Edit", group);

    connect(serverAddButton,    &QPushButton::clicked, this, &ServerSettingsPage::serverAddButtonClicked);
    connect(serverRemoveButton, &QPushButton::clicked, this, &ServerSettingsPage::serverRemoveButtonClicked);
    connect(serverEditButton,   &QPushButton::clicked, this, &ServerSettingsPage::serverEditButtonClicked);

    layout->addWidget(serverListView,     0, 0, 6, 3);
    layout->addWidget(serverAddButton,    0, 3, 1, 1);
    layout->addWidget(serverEditButton,   1, 3, 1, 1);
    layout->addWidget(serverRemoveButton, 2, 3, 1, 1);

    return group;
}

void ServerSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    const QList<Settings::Server>& serverList = settings.getServerList();
    modifiedServerList = serverList;
    for (const Settings::Server& server : serverList) {
        QStandardItem* serverItem = new QStandardItem(server.name);
        serverListModel->appendRow(serverItem);
    }

}

void ServerSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    settings.setServerList(modifiedServerList);
}

void ServerSettingsPage::serverAddButtonClicked()
{
    ModifyServerSettingsDialog serverInfoDialog(this);
    if (serverInfoDialog.exec() == QDialog::Accepted) {
        Settings::Server serverInfo = serverInfoDialog.getServerInformation();
        QStandardItem* name = new QStandardItem(serverInfo.name);
        name->setFlags(name->flags() & ~Qt::ItemIsEditable);
        serverListModel->appendRow(name);
        serverListView->setCurrentIndex(serverListModel->index(serverListModel->rowCount() - 1, 0));
        modifiedServerList << serverInfo;
    }
}

void ServerSettingsPage::serverRemoveButtonClicked()
{
    QModelIndex currentIndex = serverListView->currentIndex();
    if (currentIndex != QModelIndex()) {
        serverListModel->removeRow(currentIndex.row());
        modifiedServerList.removeAt(currentIndex.row());
    }
}

void ServerSettingsPage::serverEditButtonClicked()
{
    QModelIndex currentIndex = serverListView->currentIndex();
    if (currentIndex == QModelIndex()) {
        return;
    }
    ModifyServerSettingsDialog serverInfoDialog(this);
    Settings::Server oldServerInfo = modifiedServerList[currentIndex.row()];
    serverInfoDialog.setServerInformation(oldServerInfo);
    if (serverInfoDialog.exec() == QDialog::Accepted) {
        Settings::Server newServerInfo = serverInfoDialog.getServerInformation();
        serverListModel->setData(currentIndex, newServerInfo.name);
        modifiedServerList[currentIndex.row()] = newServerInfo;
    }
}

} // namespace Chat
