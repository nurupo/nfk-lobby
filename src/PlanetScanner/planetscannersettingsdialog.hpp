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

#ifndef PLANETSCANNERSETTINGSDIALOG_HPP
#define PLANETSCANNERSETTINGSDIALOG_HPP

#include "planetscannerdock.hpp"

#include <QCheckBox>
#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>

class PlanetScannerSettingsDialog : public QDialog
{
    Q_OBJECT
public:
    PlanetScannerSettingsDialog(PlanetScannerDock* parent);
    ~PlanetScannerSettingsDialog();
    bool isPlanetListModified() const {return planetsModified;}
    QLineEdit* gamePathEdit;
    QLineEdit* gameCommandlineEdit;
    QTreeWidget* planetTree;
    QCheckBox* filterGametypeDmCheckbox;
    QCheckBox* filterGametypeTdmCheckbox;
    QCheckBox* filterGametypeCtfCheckbox;
    QCheckBox* filterGametypeDomCheckbox;
    QCheckBox* filterGametypePracCheckbox;
    QCheckBox* filterGametypeRailCheckbox;
    QCheckBox* filterPlayersFullCheckbox;
    QCheckBox* filterPlayersEmptyCheckbox;
    QCheckBox* tableHideColumnCheckbox[5];
    QCheckBox* tableResizeDisableOnRefreshCheckbox;
    QCheckBox* miscRefreshCheckbox;
    QSpinBox* miscRefreshSpinbox;

private:
    bool planetsModified;

    QPushButton* planetRemoveButton;

private slots:
    void getGamePath();
    void onMiscRefreshCheckboxStateChange(const int state);
    void onPlanetTreeChanged(QTreeWidgetItem* item, int column);
    void onPlanetAdd();
    void onPlanetRemove();
};

#endif // PLANETSCANNERSETTINGSDIALOG_HPP
