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

#include "chatgeneralsettingspage.hpp"
#include "chatsettings.hpp"

#include <QDateTime>
#include <QLabel>
#include <QVBoxLayout>

namespace Chat {

GeneralSettingsPage::GeneralSettingsPage(QWidget *parent, QStandardItemModel* serverListModel) :
    AbstractSettingsPage(parent), serverListModel(serverListModel)
{
}

void GeneralSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* generalGroup = buildGeneralGroup();
    autoIdentifyGroup = buildAutoIdentifyGroup();

    layout->addWidget(generalGroup, 0, Qt::AlignTop);
    layout->addWidget(autoIdentifyGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* GeneralSettingsPage::buildGeneralGroup()
{
    QGroupBox* group = new QGroupBox("General options", this);
    QGridLayout* layout = new QGridLayout(group);

    QLabel* serverSelectionLabel = new QLabel("Server:", group);
    serverSelectionComboBox = new QComboBox(group);
    serverSelectionComboBox->setModel(serverListModel);

    autoConnectCheckBox = new QCheckBox("Auto connect on the startup", group);

    QLabel* nickLabel = new QLabel("Nick:", group);
    nickEdit = new QLineEdit(group);

    QLabel* usernameLabel = new QLabel("Username:", group);
    usernameEdit = new QLineEdit(group);

    QLabel* quitMessageLabel = new QLabel("Quit Message:", group);
    quitMessageEdit = new QLineEdit(group);

    layout->addWidget(serverSelectionLabel,    0, 0, 1, 1);
    layout->addWidget(serverSelectionComboBox, 0, 1, 1, 1);
    layout->addWidget(autoConnectCheckBox,     1, 1, 1, 1);
    layout->addWidget(nickLabel,               2, 0, 1, 1);
    layout->addWidget(nickEdit,                2, 1, 1, 1);
    layout->addWidget(usernameLabel,           3, 0, 1, 1);
    layout->addWidget(usernameEdit,            3, 1, 1, 1);
    layout->addWidget(quitMessageLabel,        4, 0, 1, 1);
    layout->addWidget(quitMessageEdit,         4, 1, 1, 1);

    return group;
}

QGroupBox* GeneralSettingsPage::buildAutoIdentifyGroup()
{
    QGroupBox* group = new QGroupBox("Auto Identify", this);
    group->setCheckable(true);
    QHBoxLayout* layout = new QHBoxLayout(group);

    QLabel* passwordLabel = new QLabel("Password:", group);

    passwordEdit = new QLineEdit(group);
    passwordEdit->setEchoMode(QLineEdit::Password);

    layout->addWidget(passwordLabel);
    layout->addWidget(passwordEdit);

    return group;
}

void GeneralSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    serverSelectionComboBox->setCurrentIndex(settings.getServerId());
    autoConnectCheckBox->setChecked(settings.getAutoConnect());
    nickEdit->setText(settings.getNick());
    usernameEdit->setText(settings.getUsername());
    quitMessageEdit->setText(settings.getQuitMessage());

    autoIdentifyGroup->setChecked(settings.getAutoIdentify());
    passwordEdit->setText(settings.getPassword());
}

void GeneralSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    settings.setServerId(serverSelectionComboBox->currentIndex());
    settings.setAutoConnect(autoConnectCheckBox->isChecked());
    settings.setNick(nickEdit->text());
    settings.setUsername(usernameEdit->text());
    settings.setQuitMessage(quitMessageEdit->text());

    settings.setAutoIdentify(autoIdentifyGroup->isChecked());
    settings.setPassword(passwordEdit->text());
}

} // namespace Chat
