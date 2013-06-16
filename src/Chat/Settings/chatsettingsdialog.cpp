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

#include "chatsettingsdialog.hpp"
#include "chatgeneralsettingspage.hpp"
#include "chatserversettingspage.hpp"
#include "chatchannelsettingspage.hpp"
#include "chatfontsettingspage.hpp"

#include <QStandardItemModel>

namespace Chat {

SettingsDialog::SettingsDialog(QWidget* parent) :
    BasicSettingsDialog(parent)
{
    setWindowTitle("Chat - Settings");

    QStandardItemModel* serverListModel = new QStandardItemModel(this);

    addPage(":/icons/general.png",  "General",  new GeneralSettingsPage(this, serverListModel));
    addPage(":/icons/server.png",   "Server",   new ServerSettingsPage(this, serverListModel));
    addPage(":/icons/channel.png",  "Channel",  new ChannelSettingsPage(this));
    addPage(":/icons/font.png",     "Font",     new FontSettingsPage(this));

    listWidget->setFixedWidth(100);
    setMinimumSize(440, 360);
}

} // namespace Chat
