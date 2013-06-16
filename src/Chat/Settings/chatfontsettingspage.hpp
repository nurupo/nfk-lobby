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

#ifndef CHATFONTSETTINGSPAGE_HPP
#define CHATFONTSETTINGSPAGE_HPP

#include "../../Settings/abstractsettingspage.hpp"

#include <QFont>
#include <QGroupBox>
#include <QLineEdit>
#include <QString>

namespace Chat {

class FontSettingsPage : public AbstractSettingsPage
{
    Q_OBJECT
public:
    FontSettingsPage(QWidget* parent);
    void applyChanges();
    void buildGui();
    void setGui();

private:
    QLineEdit* chatListEdit;
    QLineEdit* chatWindowEdit;
    QLineEdit* userListEdit;

    QGroupBox* buildFontGroup();
    void setEditWithFont(QLineEdit* edit, const QFont& font);
    void setEditWithFont(QLineEdit* edit);

private slots:
    void chatListButtonClicked();
    void chatWindowButtonClicked();
    void userListButtonClicked();

};

} // namespace Chat

#endif // CHATFONTSETTINGSPAGE_HPP
