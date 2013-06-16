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

#include "chatfontsettingspage.hpp"
#include "chatsettings.hpp"

#include <QFontDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

namespace Chat {

FontSettingsPage::FontSettingsPage(QWidget *parent) :
    AbstractSettingsPage(parent)
{
}

void FontSettingsPage::buildGui()
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* fontGroup = buildFontGroup();

    layout->addWidget(fontGroup, 0, Qt::AlignTop);
    layout->addStretch(0);
}

QGroupBox* FontSettingsPage::buildFontGroup()
{
    QGroupBox* group = new QGroupBox("Font options", this);
    QGridLayout* layout = new QGridLayout(group);

    // --- Chat List ---
    QLabel* chatListLabel = new QLabel("Chat List:", group);
    chatListEdit = new QLineEdit(group);
    chatListEdit->setReadOnly(true);
    QPushButton* chatListButton = new QPushButton("...", group);
    chatListButton->setFixedWidth(30);
    connect(chatListButton, &QPushButton::clicked, this, &FontSettingsPage::chatListButtonClicked);

    // --- Chat Window ---
    QLabel* chatWindowLabel = new QLabel("Chat Window:", group);
    chatWindowEdit = new QLineEdit(group);
    chatWindowEdit->setReadOnly(true);
    QPushButton* chatWindowButton = new QPushButton("...", group);
    chatWindowButton->setFixedWidth(30);
    connect(chatWindowButton, &QPushButton::clicked, this, &FontSettingsPage::chatWindowButtonClicked);

    // --- User List ---
    QLabel* userListLabel = new QLabel("User List:", group);
    userListEdit = new QLineEdit(group);
    userListEdit->setReadOnly(true);
    QPushButton* userListButton = new QPushButton("...", group);
    userListButton->setFixedWidth(30);
    connect(userListButton, &QPushButton::clicked, this, &FontSettingsPage::userListButtonClicked);

    layout->addWidget(chatListLabel,    0, 0, 1, 1);
    layout->addWidget(chatListEdit,     0, 1, 1, 1);
    layout->addWidget(chatListButton,   0, 2, 1, 1);

    layout->addWidget(chatWindowLabel,  1, 0, 1, 1);
    layout->addWidget(chatWindowEdit,   1, 1, 1, 1);
    layout->addWidget(chatWindowButton, 1, 2, 1, 1);

    layout->addWidget(userListLabel,    2, 0, 1, 1);
    layout->addWidget(userListEdit,     2, 1, 1, 1);
    layout->addWidget(userListButton,   2, 2, 1, 1);

    return group;
}

void FontSettingsPage::setGui()
{
    const Settings& settings = Settings::getInstance();
    setEditWithFont(chatListEdit, settings.getChatListFont());
    setEditWithFont(chatWindowEdit, settings.getChatWindowFont());
    setEditWithFont(userListEdit, settings.getUserListFont());
}

void FontSettingsPage::setEditWithFont(QLineEdit* edit, const QFont& font)
{
    edit->setFont(font);
    edit->setText(font.family() + ", " + QString::number(font.pointSize()));
}

void FontSettingsPage::setEditWithFont(QLineEdit* edit)
{
    bool ok;
    const QFont font = QFontDialog::getFont(&ok, edit->font(), this);
    setEditWithFont(edit, font);
}

void FontSettingsPage::applyChanges()
{
    Settings& settings = Settings::getInstance();
    settings.setChatListFont(chatListEdit->font());
    settings.setChatWindowFont(chatWindowEdit->font());
    settings.setUserListFont(userListEdit->font());
}

void FontSettingsPage::chatListButtonClicked()
{
    setEditWithFont(chatListEdit);
}

void FontSettingsPage::chatWindowButtonClicked()
{
    setEditWithFont(chatWindowEdit);
}

void FontSettingsPage::userListButtonClicked()
{
    setEditWithFont(userListEdit);
}

} // namespace Chat
