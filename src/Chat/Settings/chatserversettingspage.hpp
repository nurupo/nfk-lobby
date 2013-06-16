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

#ifndef CHATSERVERSETTINGSPAGE_HPP
#define CHATSERVERSETTINGSPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"
#include "chatsettings.hpp"

#include <QGroupBox>
#include <QStandardItemModel>
#include <QString>
#include <QTreeView>

namespace Chat {

class ServerSettingsPage : public AbstractSettingsPage
{
    Q_OBJECT
public:
    ServerSettingsPage(QWidget* parent, QStandardItemModel* serverListModel);
    void applyChanges();
    void buildGui();
    void setGui();

private:
    QGroupBox* buildServerGroup();
    void buildServerModel();

    QList<Settings::Server> modifiedServerList;
    QStandardItemModel* serverListModel;
    QTreeView* serverListView;

private slots:
    void serverAddButtonClicked();
    void serverRemoveButtonClicked();
    void serverEditButtonClicked();

};

} // namespace Chat

#endif // CHATSERVERSETTINGSPAGE_HPP
