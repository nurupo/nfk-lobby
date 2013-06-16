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

#include "checkboxdelegate.hpp"

#include <QCheckBox>
#include <QVBoxLayout>

namespace GamePreferences {

CheckBoxDelegate::CheckBoxDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
    tableView = qobject_cast<QTableView*>(parent);
}

QWidget* CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &/*option*/, const QModelIndex &/*index*/) const
{
    QWidget* dummy = new QWidget(parent);
    QVBoxLayout* layout = new QVBoxLayout(dummy);

    QCheckBox* checkBox = new QCheckBox(parent);
    checkBox->setAutoFillBackground(true);
    checkBox->setBackgroundRole(QPalette::Base);
    layout->addWidget(checkBox);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(checkBox, Qt::AlignCenter);
    return dummy;
}

void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QCheckBox* checkBox = static_cast<QCheckBox*>(editor->layout()->itemAt(0)->widget());
    checkBox->setChecked(value == "✔");
}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QCheckBox *checkBox = static_cast<QCheckBox*>(editor->layout()->itemAt(0)->widget());
    model->setData(index, checkBox->isChecked() ? "✔" : "", Qt::EditRole);
}
void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/*index*/) const
{
    editor->setGeometry(option.rect);
}

} // namespace GamePreferences
