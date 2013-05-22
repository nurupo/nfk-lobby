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

#include "chatsettingschannelpage.hpp"

#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>

bool ChatSettingsChannelPage::autojoinChannels;
QList<ChatSettingsChannelPage::Channel> ChatSettingsChannelPage::autojoinChannelList;
bool ChatSettingsChannelPage::waitBeforeAutojoining;
int ChatSettingsChannelPage::timeToWaitBeforeAutojoining;

ChatSettingsChannelPage::ChatSettingsChannelPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath) :
    AbstractSettingsPage(parent, pageName, iconPath)
{
}

void ChatSettingsChannelPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* group = buildAutojoinGroup();

    layout->addWidget(group);
}

QGroupBox* ChatSettingsChannelPage::buildAutojoinGroup()
{
    autojoinGroup = new QGroupBox("Autojoin channels", this);
    autojoinGroup->setCheckable(true);
    QVBoxLayout* groupLayout = new QVBoxLayout(autojoinGroup);

    QGroupBox* channelGroup = buildChannelGroup(autojoinGroup);
    QGroupBox* optionsGroup = buildOptionsGroup(autojoinGroup);

    groupLayout->addWidget(channelGroup);
    groupLayout->addWidget(optionsGroup);
    groupLayout->setStretch(0, 1);

    return autojoinGroup;
}

QGroupBox* ChatSettingsChannelPage::buildChannelGroup(QGroupBox* parent)
{
    QGroupBox* group = new QGroupBox("Channels", parent);
    QGridLayout* groupLayout = new QGridLayout(group);

    channelListView = new QTreeView(group);
    channelListModel = new QStandardItemModel(channelListView);
    channelListModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Password");
    channelListView->setModel(channelListModel);
    channelListView->setIndentation(0);
    channelListView->setColumnWidth(0, 150);
    channelListView->setSortingEnabled(true);

    QPushButton* channelAddButton = new QPushButton("Add", group);
    connect(channelAddButton, &QPushButton::clicked, this, &ChatSettingsChannelPage::channelAddButtonClicked);
    QPushButton* channelRemoveButton = new QPushButton("Remove", group);
    connect(channelRemoveButton, &QPushButton::clicked, this, &ChatSettingsChannelPage::channelRemoveButtonClicked);

    groupLayout->addWidget(channelListView,     0, 0, 3, 1);
    groupLayout->addWidget(channelAddButton,    0, 1, 1, 1);
    groupLayout->addWidget(channelRemoveButton, 1, 1, 1, 1);

    return group;
}

QGroupBox* ChatSettingsChannelPage::buildOptionsGroup(QGroupBox* parent)
{
    QGroupBox* group = new QGroupBox("Options", parent);
    QHBoxLayout* groupLayout = new QHBoxLayout(group);

    waitCheckBox = new QCheckBox("Wait before autojoining", group);
    waitCheckBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

    waitSpinBox = new QSpinBox(group);
    waitSpinBox->setMaximum(60);
    waitSpinBox->setMinimum(1);
    waitSpinBox->setValue(10);
    waitSpinBox->setSuffix(" sec.");
    waitSpinBox->setFixedWidth(76);

    groupLayout->addWidget(waitCheckBox, 0, Qt::AlignLeft);
    groupLayout->addWidget(waitSpinBox, 0, Qt::AlignRight);

    return group;
}

void ChatSettingsChannelPage::setGui()
{
    autojoinGroup->setChecked(autojoinChannels);
    for (int i = 0; i < autojoinChannelList.size(); i ++) {
        QStandardItem* nameColumn = new QStandardItem(autojoinChannelList[i].name);
        QStandardItem* passwordColumn = new QStandardItem(autojoinChannelList[i].password);
        channelListModel->appendRow(QList<QStandardItem*>() << nameColumn << passwordColumn);
    }
    waitCheckBox->setChecked(waitBeforeAutojoining);
    waitSpinBox->setValue(timeToWaitBeforeAutojoining);
}

void ChatSettingsChannelPage::apply()
{
    autojoinChannels = autojoinGroup->isChecked();
    autojoinChannelList = QList<Channel>();
    for (int i = 0; i < channelListModel->rowCount(); i ++) {
        Channel channel;
        channel.name = channelListModel->data(channelListModel->index(i, 0)).toString();
        channel.password = channelListModel->data(channelListModel->index(i, 1)).toString();
        autojoinChannelList << channel;
    }
    waitBeforeAutojoining = waitCheckBox->isChecked();
    timeToWaitBeforeAutojoining = waitSpinBox->value();
}

void ChatSettingsChannelPage::load(QSettings& settings)
{
    autojoinChannels = settings.value("autojoinChannels", false).toBool();
    int autojoinChannelListSize = settings.beginReadArray("autojoinChannelList");
    for (int i = 0; i < autojoinChannelListSize; i ++) {
        settings.setArrayIndex(i);
        Channel channel;
        channel.name = settings.value("name").toString();
        channel.password = settings.value("password").toString();
        autojoinChannelList << channel;
    }
    settings.endArray();
    waitBeforeAutojoining = settings.value("waitBeforeAutojoining", false).toBool();
    timeToWaitBeforeAutojoining = settings.value("timeToWaitBeforeAutojoining", 10).toInt();
}

void ChatSettingsChannelPage::save(QSettings& settings)
{
    settings.setValue("autojoinChannels", autojoinChannels);
    settings.beginWriteArray("autojoinChannelList");
    for (int i = 0; i < autojoinChannelList.size(); i ++) {
        settings.setArrayIndex(i);
        settings.setValue("name", autojoinChannelList[i].name);
        settings.setValue("password", autojoinChannelList[i].password);
    }
    settings.endArray();
    settings.setValue("waitBeforeAutojoining", waitBeforeAutojoining);
    settings.setValue("timeToWaitBeforeAutojoining", timeToWaitBeforeAutojoining);
}

void ChatSettingsChannelPage::channelAddButtonClicked()
{
    QStandardItem* name = new QStandardItem("click to edit");
    QStandardItem* password = new QStandardItem();
    name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    password->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    channelListModel->appendRow(QList<QStandardItem*>() << name << password);
    channelListView->setCurrentIndex(channelListModel->index(channelListModel->rowCount() - 1, 0));
}

void ChatSettingsChannelPage::channelRemoveButtonClicked()
{
    QModelIndex currentIndex = channelListView->currentIndex();
    if (currentIndex != QModelIndex()) {
        channelListModel->removeRow(currentIndex.row());
    }
}
