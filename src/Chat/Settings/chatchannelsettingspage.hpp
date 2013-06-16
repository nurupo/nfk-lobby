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

#ifndef CHATCHANNELSETTINGSPAGE_HPP
#define CHATCHANNELSETTINGSPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"

#include <QCheckBox>
#include <QGroupBox>
#include <QSpinBox>
#include <QStandardItemModel>
#include <QString>
#include <QTreeView>

namespace Chat {

class ChannelSettingsPage : public AbstractSettingsPage
{
    Q_OBJECT
public:
    ChannelSettingsPage(QWidget* parent);
    void applyChanges();
    void buildGui();
    void setGui();

private:
    QGroupBox* buildAutojoinGroup();
    QGroupBox* buildChannelGroup(QGroupBox* parent);
    QGroupBox* buildOptionsGroup(QGroupBox* parent);

    QGroupBox* autojoinGroup;
    QStandardItemModel* channelListModel;
    QTreeView* channelListView;
    QCheckBox* waitCheckBox;
    QSpinBox* waitSpinBox;

private slots:
    void channelAddButtonClicked();
    void channelRemoveButtonClicked();
};

} // namespace Chat

#endif // CHATCHANNELSETTINGSPAGE_HPP
