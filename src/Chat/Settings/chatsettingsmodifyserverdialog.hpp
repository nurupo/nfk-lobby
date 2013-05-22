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

#ifndef CHATSETTINGSMODIFYSERVERDIALOG_HPP
#define CHATSETTINGSMODIFYSERVERDIALOG_HPP

#include "chatsettingsserverpage.hpp"

#include <QComboBox>
#include <QDialog>
#include <QGroupBox>
#include <QLineEdit>
#include <QSpinBox>

class ChatSettingsModifyServerDialog : public QDialog
{
public:
    explicit ChatSettingsModifyServerDialog(QWidget* parent);
    void setServerInformation(const ChatSettingsServerPage::Server&server);
    ChatSettingsServerPage::Server getServerInformation();

private:
    QGroupBox* buildServerInfoGroup();

    QLineEdit* nameEdit;
    QLineEdit* addressEdit;
    QSpinBox* portSpinBox;
    QLineEdit* passwordEdit;
    QComboBox* encodingComboBox;
};

#endif // CHATSETTINGSMODIFYSERVERDIALOG_HPP
