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

#ifndef BASICSETTINGSDIALOG_HPP
#define BASICSETTINGSDIALOG_HPP

#include <QDialog>
#include <QList>
#include <QListWidget>
#include "abstractsettingspage.hpp"

class BasicSettingsDialogNotifier : public QObject
{
Q_OBJECT

signals:
    void updated();
};

class BasicSettingsDialog : protected QDialog
{
public:
    explicit BasicSettingsDialog(const QString &sectionName, QWidget* parent);
    virtual ~BasicSettingsDialog();

    void showDialog();

protected:
    static QList<AbstractSettingsPage*> pages;
    QListWidget* listWidget;
    QStackedWidget* stackedWidget;

    virtual void buildGui() = 0;
    void buildPagesGui();
    static void addPage(AbstractSettingsPage* page);
    static void loadAddedPages(const QString &sectionName);
    static void removePages();

private:
    void accept();
    void apply();
    void save();
    void buildBasicGui();

    QString sectionName;

};

#endif // BASICSETTINGSDIALOG_HPP
