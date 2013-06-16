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

#include "chatchannelsettingspage.hpp"
#include "chatsettings.hpp"
#include "customhinttreeview.hpp"

#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>

namespace Chat {

ChannelSettingsPage::ChannelSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void ChannelSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    autojoinGroup = buildAutojoinGroup();

    layout->addWidget(autojoinGroup);
}

QGroupBox* ChannelSettingsPage::buildAutojoinGroup()
{
    QGroupBox* group = new QGroupBox("Autojoin channels", this);
    group->setCheckable(true);
    QVBoxLayout* layout = new QVBoxLayout(group);

    QGroupBox* channelGroup = buildChannelGroup(group);
    QGroupBox* optionsGroup = buildOptionsGroup(group);

    layout->addWidget(channelGroup);
    layout->addWidget(optionsGroup);
    layout->setStretch(0, 1);

    return group;
}

QGroupBox* ChannelSettingsPage::buildChannelGroup(QGroupBox* parent)
{
    QGroupBox* group = new QGroupBox("Channels", parent);
    QGridLayout* layout = new QGridLayout(group);

    channelListView = new CustomHintTreeView(group, QSize(10, 10));
    channelListModel = new QStandardItemModel(channelListView);
    channelListModel->setHorizontalHeaderLabels(QStringList() << "Name" << "Password");
    channelListView->setModel(channelListModel);
    channelListView->setIndentation(0);
    channelListView->setColumnWidth(0, 90);
    channelListView->setColumnWidth(1, 60);
    channelListView->setSortingEnabled(true);

    QPushButton* channelAddButton = new QPushButton("Add", group);
    connect(channelAddButton, &QPushButton::clicked, this, &ChannelSettingsPage::channelAddButtonClicked);
    QPushButton* channelRemoveButton = new QPushButton("Remove", group);
    connect(channelRemoveButton, &QPushButton::clicked, this, &ChannelSettingsPage::channelRemoveButtonClicked);

    layout->addWidget(channelListView,     0, 0, 3, 1);
    layout->addWidget(channelAddButton,    0, 1, 1, 1);
    layout->addWidget(channelRemoveButton, 1, 1, 1, 1);

    return group;
}

QGroupBox* ChannelSettingsPage::buildOptionsGroup(QGroupBox* parent)
{
    QGroupBox* group = new QGroupBox("Options", parent);
    QHBoxLayout* layout = new QHBoxLayout(group);

    waitCheckBox = new QCheckBox("Wait before autojoining", group);
    waitCheckBox->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred));

    waitSpinBox = new QSpinBox(group);
    waitSpinBox->setMaximum(60);
    waitSpinBox->setMinimum(1);
    waitSpinBox->setValue(10);
    waitSpinBox->setSuffix(" sec.");
    waitSpinBox->setFixedWidth(76);

    layout->addWidget(waitCheckBox, 0, Qt::AlignLeft);
    layout->addWidget(waitSpinBox,  0, Qt::AlignRight);

    return group;
}

void ChannelSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    autojoinGroup->setChecked(settings.getAutojoinChannels());
    const QList<Settings::Channel>& channelList = settings.getAutojoinChannelList();
    for (const Settings::Channel& channel : channelList) {
        QStandardItem* nameColumn = new QStandardItem(channel.name);
        QStandardItem* passwordColumn = new QStandardItem(channel.password);
        channelListModel->appendRow(QList<QStandardItem*>() << nameColumn << passwordColumn);
    }

    waitCheckBox->setChecked(settings.getWaitBeforeAutojoining());
    waitSpinBox->setValue(settings.getTimeToWaitBeforeAutojoining());
}

void ChannelSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    settings.setAutojoinChannels(autojoinGroup->isChecked());
    QList<Settings::Channel> channelList;
    for (int i = 0; i < channelListModel->rowCount(); i ++) {
        Settings::Channel channel;
        channel.name = channelListModel->data(channelListModel->index(i, 0)).toString();
        channel.password = channelListModel->data(channelListModel->index(i, 1)).toString();
        channelList << channel;
    }
    settings.setAutojoinChannelList(channelList);

    settings.setWaitBeforeAutojoining(waitCheckBox->isChecked());
    settings.setTimeToWaitBeforeAutojoining(waitSpinBox->value());
}

void ChannelSettingsPage::channelAddButtonClicked()
{
    QStandardItem* name = new QStandardItem("click to edit");
    QStandardItem* password = new QStandardItem();
    //name->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    //password->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    channelListModel->appendRow(QList<QStandardItem*>() << name << password);
    channelListView->setCurrentIndex(channelListModel->index(channelListModel->rowCount() - 1, 0));
}

void ChannelSettingsPage::channelRemoveButtonClicked()
{
    QModelIndex currentIndex = channelListView->currentIndex();
    if (currentIndex != QModelIndex()) {
        channelListModel->removeRow(currentIndex.row());
    }
}

} // namespace Chat
