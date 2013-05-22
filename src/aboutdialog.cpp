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

#include "aboutdialog.hpp"
#include "licensedialog.hpp"
#include "mainwindow.hpp"

#include <QApplication>
#include <QGridLayout>
#include <QIcon>
#include <QLabel>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("About");

    QGridLayout* layout = new QGridLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    const int SPACER_SIZE = 8;
    QString Url("<a href=\"%2\">%1</a>");
    LicenseDialog* licenseDialog = new LicenseDialog(this);

    QLabel* iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(":/icons/appicon.png"));
    iconLabel->setFixedSize(32 + 10, 32 + 10);
    QLabel* applicationNameLabel = getLabel(MainWindow::name, 16, true, Qt::AlignLeft);
    QLabel* versionLabel = getLabel(QString("version %1").arg(MainWindow::version), 8, false, Qt::AlignLeft);
    QLabel* buildDateLabel = getLabel(QString("Build date: %1").arg(MainWindow::buildDate), 8, false, Qt::AlignLeft);
    QLabel* iconsSourceLabel = getLabel(QString("%1 %2").arg("This application uses").arg(Url.arg("FatCow icons").arg("http://www.fatcow.com/free-icons")), 8, false, Qt::AlignLeft);
    iconsSourceLabel->setOpenExternalLinks(true);
    QLabel* licenseLabel = getLabel(QString("License: %1").arg(Url.arg("GPLv3").arg(QString(":/"))), 8, false, Qt::AlignLeft);
    connect(licenseLabel, &QLabel::linkActivated, licenseDialog, &QDialog::exec);
    QLabel* projectPageLabel = getLabel(Url.arg("Project page").arg("https://github.com/nurupo/nfk-lobby"), 8, false, Qt::AlignLeft);
    projectPageLabel->setOpenExternalLinks(true);
    QLabel* copyrightLabel = getLabel(QString("(c) %1, %2").arg(MainWindow::years).arg(MainWindow::author), 8, false, Qt::AlignCenter);

    layout->addWidget(iconLabel,                                0, 0, 2, 1);
    layout->addWidget(applicationNameLabel,                     0, 1, 1, 2);
    layout->addWidget(versionLabel,                             1, 1, 1, 2);
    layout->addItem(new QSpacerItem(SPACER_SIZE, SPACER_SIZE),  2, 2, 1, 1);
    layout->addWidget(buildDateLabel,                           3, 0, 1, 3);
    layout->addItem(new QSpacerItem(SPACER_SIZE, SPACER_SIZE),  4, 2, 1, 1);
    layout->addWidget(iconsSourceLabel,                         5, 0, 1, 3);
    layout->addItem(new QSpacerItem(SPACER_SIZE, SPACER_SIZE),  6, 2, 1, 1);
    layout->addWidget(licenseLabel,                             7, 0, 1, 3);
    layout->addWidget(projectPageLabel,                         8, 0, 1, 3);
    layout->addItem(new QSpacerItem(SPACER_SIZE, SPACER_SIZE),  9, 2, 1, 1);
    layout->addWidget(copyrightLabel,                           10, 0, 1, 3);
}

QLabel* AboutDialog::getLabel(const QString &text, int size, bool bold, Qt::Alignment alignment)
{
    QLabel* label = new QLabel(text, this);
    QFont font = QFont(label->font().family(), size);
    font.setBold(bold);
    label->setFont(font);
    label->setAlignment(alignment);
    return label;
}

