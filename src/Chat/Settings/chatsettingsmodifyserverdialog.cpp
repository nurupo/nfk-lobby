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

#include "chatsettingsmodifyserverdialog.hpp"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextCodec>

ChatSettingsModifyServerDialog::ChatSettingsModifyServerDialog(QWidget* parent) :
    QDialog(parent)
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setWindowTitle("Server Information");
    setMinimumWidth(350);

    QVBoxLayout* layout = new QVBoxLayout(this);

    QGroupBox* serverInfoGroup = buildServerInfoGroup();

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    buttonBox->button(QDialogButtonBox::Ok)->setDefault(true);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    layout->addWidget(serverInfoGroup);
    layout->addWidget(buttonBox);
}

QGroupBox* ChatSettingsModifyServerDialog::buildServerInfoGroup()
{
    QGroupBox* group = new QGroupBox("Server info", this);
    QGridLayout* groupLayout = new QGridLayout(group);

    QLabel* nameLabel = new QLabel("Name:", group);
    nameEdit = new QLineEdit(group);

    QLabel* addressLabel = new QLabel("Address:", group);
    addressEdit = new QLineEdit(group);

    QLabel* portLabel = new QLabel("Port:", group);
    portSpinBox = new QSpinBox(group);
    portSpinBox->setMaximum(65535);
    portSpinBox->setMinimum(1);
    portSpinBox->setValue(6667);
    portSpinBox->setMinimumWidth(64);

    QLabel* passwordLabel = new QLabel("Password:", group);
    passwordEdit = new QLineEdit(group);
    passwordEdit->setEchoMode(QLineEdit::Password);

    QLabel* encodingLabel = new QLabel("Encoding:", group);
    encodingComboBox = new QComboBox(group);
    QList<int> mlibs = QTextCodec::availableMibs();
    QStringList codecNames;
    for (int i = 0; i < mlibs.size(); i ++) {
        QString codecName = QString(QTextCodec::codecForMib(mlibs[i])->name());
        if (!codecNames.contains(codecName)) {
            codecNames << codecName;
        }
    }
    codecNames.sort();
    encodingComboBox->addItems(codecNames);
    int utf8Index = encodingComboBox->findText("UTF-8");
    if (utf8Index != -1) {
        encodingComboBox->setCurrentIndex(utf8Index);
    }

    groupLayout->addWidget(nameLabel,           0, 0, 1, 1);
    groupLayout->addWidget(nameEdit,            0, 1, 1, 3);
    groupLayout->addWidget(addressLabel,        1, 0, 1, 1);
    groupLayout->addWidget(addressEdit,         1, 1, 1, 1);
    groupLayout->addWidget(portLabel,           1, 2, 1, 1);
    groupLayout->addWidget(portSpinBox,         1, 3, 1, 1);
    groupLayout->addWidget(passwordLabel,       2, 0, 1, 1);
    groupLayout->addWidget(passwordEdit,        2, 1, 1, 3);
    groupLayout->addWidget(encodingLabel,       3, 0, 1, 1);
    groupLayout->addWidget(encodingComboBox,    3, 1, 1, 3);

    return group;
}

void ChatSettingsModifyServerDialog::setServerInformation(const ChatSettingsServerPage::Server &server)
{
    nameEdit->setText(server.name);
    addressEdit->setText(server.address);
    portSpinBox->setValue(server.port);
    passwordEdit->setText(server.password);
    encodingComboBox->setCurrentIndex(encodingComboBox->findText(server.encoding));
}

ChatSettingsServerPage::Server ChatSettingsModifyServerDialog::getServerInformation()
{
    ChatSettingsServerPage::Server server;

    server.name = nameEdit->text();
    server.address = addressEdit->text();
    server.port = portSpinBox->value();
    server.password = passwordEdit->text();
    server.encoding = encodingComboBox->currentText();

    return server;
}
