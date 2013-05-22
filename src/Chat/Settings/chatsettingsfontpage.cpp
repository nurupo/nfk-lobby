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

#include "chatsettingsfontpage.hpp"

#include <QFontDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

QFont ChatSettingsFontPage::chatListFont = QFont("Sans Serif", 10);
QFont ChatSettingsFontPage::chatWindowFont = QFont("Curier", 8);
QFont ChatSettingsFontPage::userListFont = QFont("Sans Serif", 10);

ChatSettingsFontPage::ChatSettingsFontPage(QStackedWidget *parent, const QString &pageName, const QString &iconPath) :
    AbstractSettingsPage(parent, pageName, iconPath)
{
}

void ChatSettingsFontPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* fontGroup = buildFontGroup();

    layout->addWidget(fontGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* ChatSettingsFontPage::buildFontGroup()
{
    QGroupBox* group = new QGroupBox("Font options", this);
    QGridLayout* groupLayout = new QGridLayout(group);

    // --- Chat List ---
    QLabel* chatListLabel = new QLabel("Chat List:", group);
    chatListEdit = new QLineEdit(group);
    chatListEdit->setReadOnly(true);
    QPushButton* chatListButton = new QPushButton("...", group);
    chatListButton->setFixedWidth(30);
    connect(chatListButton, &QPushButton::clicked, this, &ChatSettingsFontPage::chatListButtonClicked);

    // --- Chat Window ---
    QLabel* chatWindowLabel = new QLabel("Chat Window:", group);
    chatWindowEdit = new QLineEdit(group);
    chatWindowEdit->setReadOnly(true);
    QPushButton* chatWindowButton = new QPushButton("...", group);
    chatWindowButton->setFixedWidth(30);
    connect(chatWindowButton, &QPushButton::clicked, this, &ChatSettingsFontPage::chatWindowButtonClicked);

    // --- User List ---
    QLabel* userListLabel = new QLabel("User List:", group);
    userListEdit = new QLineEdit(group);
    userListEdit->setReadOnly(true);
    QPushButton* userListButton = new QPushButton("...", group);
    userListButton->setFixedWidth(30);
    connect(userListButton, &QPushButton::clicked, this, &ChatSettingsFontPage::userListButtonClicked);

    groupLayout->addWidget(chatListLabel,       0, 0, 1, 1);
    groupLayout->addWidget(chatListEdit,        0, 1, 1, 1);
    groupLayout->addWidget(chatListButton,      0, 2, 1, 1);

    groupLayout->addWidget(chatWindowLabel,     1, 0, 1, 1);
    groupLayout->addWidget(chatWindowEdit,      1, 1, 1, 1);
    groupLayout->addWidget(chatWindowButton,    1, 2, 1, 1);

    groupLayout->addWidget(userListLabel,       2, 0, 1, 1);
    groupLayout->addWidget(userListEdit,        2, 1, 1, 1);
    groupLayout->addWidget(userListButton,      2, 2, 1, 1);

    return group;
}

void ChatSettingsFontPage::setGui()
{
    setEditWithFont(chatListEdit, chatListFont);
    setEditWithFont(chatWindowEdit, chatWindowFont);
    setEditWithFont(userListEdit, userListFont);
}

void ChatSettingsFontPage::setEditWithFont(QLineEdit* edit, const QFont& font)
{
    edit->setFont(font);
    edit->setText(font.family() + ", " + QString::number(font.pointSize()));
}

void ChatSettingsFontPage::setEditWithFont(QLineEdit* edit)
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, edit->font(), this);
    setEditWithFont(edit, font);
}

void ChatSettingsFontPage::apply()
{
    chatListFont = QFont(chatListEdit->font());
    chatWindowFont = QFont(chatWindowEdit->font());
    userListFont = QFont(userListEdit->font());
}

void ChatSettingsFontPage::load(QSettings& settings)
{
    chatListFont = settings.value("chatListFont", QFont("Sans Serif", 10)).value<QFont>();
    chatWindowFont = settings.value("chatWindowFont", QFont("Curier", 8)).value<QFont>();
    userListFont = settings.value("userListFont", QFont("Sans Serif", 10)).value<QFont>();
}

void ChatSettingsFontPage::save(QSettings& settings)
{
    settings.setValue("chatListFont", chatListFont);
    settings.setValue("chatWindowFont", chatWindowFont);
    settings.setValue("userListFont", userListFont);
}

void ChatSettingsFontPage::chatListButtonClicked()
{
    setEditWithFont(chatListEdit);
}

void ChatSettingsFontPage::chatWindowButtonClicked()
{
    setEditWithFont(chatWindowEdit);
}

void ChatSettingsFontPage::userListButtonClicked()
{
    setEditWithFont(userListEdit);
}
