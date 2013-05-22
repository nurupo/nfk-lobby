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
#include "chatsettingsdialog.hpp"
#include "chatsettingsfontpage.hpp"
#include "chatsettingsgeneralpage.hpp"
#include "chatsettingsserverpage.hpp"

BasicSettingsDialogNotifier ChatSettingsDialog::settingsNotifier;
bool ChatSettingsDialog::previouslyLoaded = false;
QString ChatSettingsDialog::sectionName = "Chat";

ChatSettingsDialog::ChatSettingsDialog(QWidget *parent)
    : BasicSettingsDialog(sectionName, parent)
{
    connect(this, &ChatSettingsDialog::accepted, &ChatSettingsDialog::settingsNotifier, &BasicSettingsDialogNotifier::updated);
}

void ChatSettingsDialog::addPages(QStackedWidget* stackedWidget)
{
    addPage(new ChatSettingsGeneralPage(stackedWidget, "General", ":/icons/general.png"));
    addPage(new ChatSettingsServerPage(stackedWidget, "Server", ":/icons/server.png"));
    addPage(new ChatSettingsChannelPage(stackedWidget, "Channel", ":/icons/channel.png"));
    addPage(new ChatSettingsFontPage(stackedWidget, "Font", ":/icons/font.png"));
}

void ChatSettingsDialog::buildGui()
{
    addPages(stackedWidget);

    setWindowTitle("Chat - Settings");

    listWidget->setFixedWidth(100);

    setMinimumSize(440, 325);
}

void ChatSettingsDialog::load()
{    
    if (previouslyLoaded) {
        return;
    }

    addPages();

    BasicSettingsDialog::loadAddedPages(sectionName);

    removePages();

    previouslyLoaded = true;
}
