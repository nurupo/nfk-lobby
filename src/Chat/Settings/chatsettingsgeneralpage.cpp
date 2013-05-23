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

#include "chatsettingsgeneralpage.hpp"
#include "chatsettingsserverpage.hpp"

#include <QDateTime>
#include <QLabel>
#include <QVBoxLayout>

int ChatSettingsGeneralPage::serverId;
bool ChatSettingsGeneralPage::autoConnect;
QString ChatSettingsGeneralPage::nick;
QString ChatSettingsGeneralPage::username;
QString ChatSettingsGeneralPage::quitMessage;
bool ChatSettingsGeneralPage::autoIdentify;
QString ChatSettingsGeneralPage::password;

ChatSettingsGeneralPage::ChatSettingsGeneralPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath) :
    AbstractSettingsPage(parent, pageName, iconPath)
{
}

void ChatSettingsGeneralPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* generalGroup = buildGeneralGroup();
    QGroupBox* autoIdentifyGroup = buildAutoIdentifyGroup();

    layout->addWidget(generalGroup, 0, Qt::AlignTop);
    layout->addWidget(autoIdentifyGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* ChatSettingsGeneralPage::buildGeneralGroup()
{
    QGroupBox* group = new QGroupBox("General options", this);
    QGridLayout* groupLayout = new QGridLayout(group);

    QLabel* serverSelectionLabel = new QLabel("Server:", group);
    serverSelectionComboBox = new QComboBox(group);
    serverSelectionComboBox->setModel(ChatSettingsServerPage::getServerListModel());

    autoConnectCheckBox = new QCheckBox("Auto connect on the startup", group);

    QLabel* nickLabel = new QLabel("Nick:", group);
    nickEdit = new QLineEdit(group);

    QLabel* usernameLabel = new QLabel("Username:", group);
    usernameEdit = new QLineEdit(group);

    QLabel* quitMessageLabel = new QLabel("Quit Message:", group);
    quitMessageEdit = new QLineEdit(group);

    groupLayout->addWidget(serverSelectionLabel,    0, 0, 1, 1);
    groupLayout->addWidget(serverSelectionComboBox, 0, 1, 1, 1);
    groupLayout->addWidget(autoConnectCheckBox,     1, 1, 1, 1);
    groupLayout->addWidget(nickLabel,               2, 0, 1, 1);
    groupLayout->addWidget(nickEdit,                2, 1, 1, 1);
    groupLayout->addWidget(usernameLabel,           3, 0, 1, 1);
    groupLayout->addWidget(usernameEdit,            3, 1, 1, 1);
    groupLayout->addWidget(quitMessageLabel,        4, 0, 1, 1);
    groupLayout->addWidget(quitMessageEdit,         4, 1, 1, 1);

    return group;
}

QGroupBox* ChatSettingsGeneralPage::buildAutoIdentifyGroup()
{
    autoIdentifygroup = new QGroupBox("Auto Identify", this);
    autoIdentifygroup->setCheckable(true);
    QHBoxLayout* groupLayout = new QHBoxLayout(autoIdentifygroup);

    QLabel* passwordLabel = new QLabel("Password:", autoIdentifygroup);

    passwordEdit = new QLineEdit(autoIdentifygroup);
    passwordEdit->setEchoMode(QLineEdit::Password);

    groupLayout->addWidget(passwordLabel);
    groupLayout->addWidget(passwordEdit);

    return autoIdentifygroup;
}

void ChatSettingsGeneralPage::setGui()
{
    serverSelectionComboBox->setCurrentIndex(serverId);
    autoConnectCheckBox->setChecked(autoConnect);
    nickEdit->setText(nick);
    usernameEdit->setText(username);
    quitMessageEdit->setText(quitMessage);

    autoIdentifygroup->setChecked(autoIdentify);
    passwordEdit->setText(password);
}

void ChatSettingsGeneralPage::apply()
{
    serverId = serverSelectionComboBox->currentIndex();
    autoConnect = autoConnectCheckBox->isChecked();
    nick = nickEdit->text();
    username = usernameEdit->text();
    quitMessage = quitMessageEdit->text();

    autoIdentify = autoIdentifygroup->isChecked();
    password = passwordEdit->text();
}

void ChatSettingsGeneralPage::load(QSettings& settings)
{
    serverId = settings.value("serverId", 0).toInt();
    srand(QDateTime::currentDateTime().toTime_t());
    autoConnect = settings.value("autoConnect", false).toBool();
    nick = settings.value("nick", QString("nfkl_user%1").arg(rand() % 10000)).toString();
    username = settings.value("username", "nfkl_user").toString();
    quitMessage = settings.value("quitMessage").toString();
    autoIdentify = settings.value("autoIdentify", false).toBool();
    password = settings.value("password").toString();
}

void ChatSettingsGeneralPage::save(QSettings& settings)
{
    settings.setValue("serverId", serverId);
    settings.setValue("autoConnect", autoConnect);
    settings.setValue("nick", nick);
    settings.setValue("username", username);
    settings.setValue("quitMessage", quitMessage);
    settings.setValue("autoIdentify", autoIdentify);
    settings.setValue("password", password);
}
