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

#include "licensedialog.hpp"

#include <QFile>
#include <QTextEdit>
#include <QVBoxLayout>

LicenseDialog::LicenseDialog(QWidget *parent) :
    QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("License");

    QVBoxLayout* layout = new QVBoxLayout(this);

    QTextEdit* licenseEdit = new QTextEdit(this);
    licenseEdit->setReadOnly(true);
    QFile licenseFile(":/license");
    licenseFile.open(QIODevice::ReadOnly | QIODevice::Text);
    licenseEdit->setText(licenseFile.readAll());
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    licenseEdit->setFont(font);
    layout->addWidget(licenseEdit);

    setMinimumSize(600, 500);
}
