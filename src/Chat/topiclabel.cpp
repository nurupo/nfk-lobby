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

#include "Pages/basicpage.hpp"
#include "topiclabel.hpp"

#include <QEvent>
#include <QRegularExpression>

TopicLabel::TopicLabel(QWidget* parent) : QLabel(parent)
{
    setWordWrap(true);
    setAttribute(Qt::WA_Hover);
    setTextFormat(Qt::RichText);
    setTextInteractionFlags(textInteractionFlags() | Qt::LinksAccessibleByMouse);
    setOpenExternalLinks(true);
    setMinimumHeight(1);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

bool TopicLabel::event(QEvent* event)
{
    if (event->type() == QEvent::HoverEnter) {
        /*FIXME: isn't very precise since UrlText, if it does contain URLs, has bigger width than planText does*/
        int hiddenLines = fontMetrics().width(plainText)/width();
        if (hiddenLines > 0) {
            setFixedHeight((hiddenLines+1)*fontMetrics().height());
        }
        return true;
    }
    if (event->type() == QEvent::HoverLeave) {
        setFixedHeight(fontMetrics().height());
        return true;
    }
    if (event->type() == QEvent::Resize) {
        QLabel::setText(UrlText);
    }
    return QLabel::event(event);
}

void TopicLabel::setText(const QString &newText)
{
    setFixedHeight(fontMetrics().height());
    plainText = newText;
    BasicPage::eraseControlCharacters(plainText);
    UrlText = plainText.toHtmlEscaped();
    UrlText.prepend("<style>a {text-decoration:none; color:#2266ff;}</style>");
    UrlText.replace(QRegularExpression("((?:https?|ftp)://\\S+)"), "<a href=\"\\1\">\\1</a>");
    QLabel::setText(UrlText);
}

QSize TopicLabel::sizeHint() const
{
    return QSize(1, 1);
}
