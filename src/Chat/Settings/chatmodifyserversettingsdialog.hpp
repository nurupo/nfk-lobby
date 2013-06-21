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

#ifndef CHATMODIFYSERVERSETTINGSDIALOG_HPP
#define CHATMODIFYSERVERSETTINGSDIALOG_HPP

#include "chatsettings.hpp"

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>

namespace Chat {

class ModifyServerSettingsDialog : public QDialog
{
public:
    explicit ModifyServerSettingsDialog(QWidget* parent);
    void setServerInformation(const Settings::Server& server);
    Settings::Server getServerInformation();

private:
    QGroupBox* buildServerInfoGroup();

    QLineEdit* nameEdit;
    QLineEdit* addressEdit;
    QSpinBox* portSpinBox;
    QLineEdit* passwordEdit;
    QComboBox* encodingComboBox;
};

} // namespace Chat

#endif // CHATMODIFYSERVERSETTINGSDIALOG_HPP
