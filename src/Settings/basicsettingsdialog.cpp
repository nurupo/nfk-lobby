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

#include "basicsettingsdialog.hpp"

#include <QDialogButtonBox>
#include <QGridLayout>
#include <QListWidgetItem>
#include <QPushButton>

QList<AbstractSettingsPage*> BasicSettingsDialog::pages;

BasicSettingsDialog::BasicSettingsDialog(const QString& sectionName, QWidget* parent) :
    QDialog(parent), sectionName(sectionName)
{
}

BasicSettingsDialog::~BasicSettingsDialog()
{
    removePages();
}

void BasicSettingsDialog::buildBasicGui()
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QGridLayout* dialogLayout = new QGridLayout(this);

    listWidget = new QListWidget(this);
    listWidget->setIconSize(QSize(24, 24));

    stackedWidget = new QStackedWidget(this);
    stackedWidget->setCurrentIndex(0);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    dialogLayout->addWidget(listWidget,     0, 0, 1, 1);
    dialogLayout->addWidget(stackedWidget,  0, 1, 1, 1);
    dialogLayout->addWidget(buttonBox,      1, 0, 1, 2);

    connect(listWidget, &QListWidget::currentRowChanged, stackedWidget, &QStackedWidget::setCurrentIndex);
}

void BasicSettingsDialog::showDialog()
{
    buildBasicGui();
    buildGui();
    buildPagesGui();
    exec();
}

void BasicSettingsDialog::accept()
{
    apply();
    save();
    QDialog::accept();
}

void BasicSettingsDialog::apply()
{
    for (int i = 0; i < pages.size(); i ++) {
        pages[i]->apply();
    }
}

void BasicSettingsDialog::save()
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup(sectionName);
    for (int i = 0; i < pages.size(); i ++) {
        settings.remove(pages[i]->getName());
        settings.beginGroup(pages[i]->getName());
        pages[i]->save(settings);
        settings.endGroup();
    }
    settings.endGroup();
}

void BasicSettingsDialog::buildPagesGui()
{
    for (int i = 0; i < pages.size(); i ++) {
        listWidget->addItem(new QListWidgetItem(pages[i]->getIcon(), pages[i]->getName(), listWidget));
        stackedWidget->addWidget(pages[i]);
        pages[i]->buildGui();
        pages[i]->setGui();
    }
}

void BasicSettingsDialog::loadAddedPages(const QString &sectionName)
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup(sectionName);
    for (int i = 0; i < pages.size(); i ++) {
        settings.beginGroup(pages[i]->getName());
        pages[i]->load(settings);
        settings.endGroup();
    }
    settings.endGroup();
}

void BasicSettingsDialog::addPage(AbstractSettingsPage* page)
{
    pages << page;
}

void BasicSettingsDialog::removePages()
{
    while (pages.size() != 0) {
        AbstractSettingsPage* page = pages[0];
        pages.removeAt(0);
        delete page;
    }
}
