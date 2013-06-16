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

#ifndef CHATGENERALSETTINGSPAGE_HPP
#define CHATGENERALSETTINGSPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"

#include <QAbstractItemModel>
#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QString>

namespace Chat {

class GeneralSettingsPage : public AbstractSettingsPage
{
public:
    GeneralSettingsPage(QWidget* parent, QStandardItemModel* serverListModel);
    void applyChanges();
    void buildGui();
    void setGui();

private:
    QGroupBox* buildGeneralGroup();
    QGroupBox* buildAutoIdentifyGroup();

    QComboBox* serverSelectionComboBox;
    QStandardItemModel* serverListModel;
    QCheckBox* autoConnectCheckBox;
    QLineEdit* nickEdit;
    QLineEdit* usernameEdit;
    QLineEdit* quitMessageEdit;

    QGroupBox* autoIdentifyGroup;
    QLineEdit* passwordEdit;

};

} // namespace Chat

#endif // CHATGENERALSETTINGSPAGE_HPP
