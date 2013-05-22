#include "../Settings/chatsettingsdialog.hpp"
#include "../Settings/chatsettingsfontpage.hpp"
#include "basicpage.hpp"
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


#include <QRegularExpression>
#include <QScrollBar>
#include <QTime>

BasicPage::BasicPage(QTreeWidgetItem* pageTab, QTreeWidget* tabTree) :
    QMainWindow(0)
{
    textBrowser = new QTextBrowser(this);
    updateFont();
    textBrowser->setMinimumWidth(400);
    textBrowser->setMinimumHeight(20);
    textBrowser->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    textBrowser->setAcceptRichText(true);
    textBrowser->setOpenExternalLinks(true);
    textBrowser->setTextInteractionFlags((textBrowser->textInteractionFlags() & ~Qt::TextEditable) | Qt::LinksAccessibleByMouse);
    textBrowser->setUndoRedoEnabled(false);
    textBrowser->setCursorWidth(0);
    textBrowser->document()->setDefaultStyleSheet(
                QString("a {text-decoration:none; color:#2266ff;}")
                + QString("p {text-indent:-20px; margin-left:20px; margin-top:0; margin-bottom:0; white-space:pre-wrap;}")
                + QString(".t%1{color:#916409;}").arg(ServerResponse)
                + QString(".t%1{color:#916409;}").arg(Notice)
                + QString(".t%1{color:#960096;}").arg(Information)
                + QString(".t%1{color:#000000;}").arg(Message)
                + QString(".t%1{color:#2222FF;}").arg(Action)
                + QString(".t%1{color:#707C70;}").arg(Timestamp)
                + QString(".t%1{color:#2222FF;}").arg(Ctcp)
                + QString(".t%1{background-color:#FFAA22;}").arg(Highlight)
    );
    setCentralWidget(textBrowser);
    connect(textBrowser, &QTextBrowser::textChanged, this, &BasicPage::updateScrollbarPosition);
    connect(textBrowser->verticalScrollBar(), &QScrollBar::valueChanged, this, &BasicPage::checkSliderPosition);
    tab = pageTab;
    tabContextMenu = new QMenu(tabTree);
    lastMessageType = None;
    scrollDown = true;
    disabled = true;
    connect(&ChatSettingsDialog::settingsNotifier, &BasicSettingsDialogNotifier::updated, this, &BasicPage::updateFont);
}

BasicPage::~BasicPage()
{
}

QString BasicPage::getTime()
{
    return QTime::currentTime().toString("[hh:mm:ss]");
}

void BasicPage::eraseControlCharacters(QString& string)
{
    /*TODO: replace characters with their unicode representation*/
    string.replace(QRegularExpression("(\\((\\d)?\\d)?(\\,(\\d)?\\d)?)|\\|\\|\\|\\"), QString(""));
}

void BasicPage::urlify(QString& string)
{
    string.replace(QRegularExpression("((?:https?|ftp)://\\S+)"), "<a href=\"\\1\">\\1</a>");
}

void BasicPage::formatForOutput(QString& string, MessageType type)
{
    string.prepend(QString("<p><span class=\"t%1\">%2 </span><span class=\"t%3\">").arg(Timestamp).arg(getTime()).arg(type));
    string.append("</span></p>");
}

void BasicPage::print(const QString& string, MessageType type) {
    QString text(string);
    eraseControlCharacters(text);
    text = text.toHtmlEscaped();
    urlify(text);
    formatForOutput(text, type);
    textBrowser->append(text);
    updateTabIndication(type);
}

void BasicPage::updateTabIndication(MessageType event)
{
    if (tab->isSelected()) {
        return;
    }

    if (event <= lastMessageType) {
        return;
    }

    QColor color;
    switch (event) {
        case Information:
            /*greenish*/
            color = QColor(0, 211, 34);     break;

        case ServerResponse:
        case Notice:
        case Ctcp:
        case Action:
        case Message:
            /*bluish*/
            color = QColor(34, 85, 255);    break;

        case Highlight:
            /*orangish*/
            color = QColor(255, 170, 34);   break;

       default:                             break;
    }

    tab->setForeground(0, color);
    lastMessageType = event;
}

void BasicPage::clearTabIndication()
{
    QBrush brush;
    if (disabled) {
        brush = QBrush(Qt::gray);
    } else {
        brush = QBrush(Qt::black);
    }
    tab->setForeground(0, brush);
    lastMessageType = None;
}

void BasicPage::updateScrollbarPosition()
{
    if (scrollDown) {
        QScrollBar* scrollbar = textBrowser->verticalScrollBar();
        scrollbar->setValue(scrollbar->maximum());
    }
}

void BasicPage::checkSliderPosition(int value)
{
    scrollDown = value == textBrowser->verticalScrollBar()->maximum();
}

void BasicPage::disable()
{
    if (!disabled) {
        disabled = true;
        if (lastMessageType == None) {
            clearTabIndication();
        }
    }
}

void BasicPage::enable()
{
    if (disabled) {
        disabled = false;
        if (lastMessageType == None) {
            clearTabIndication();
        }
    }
}

bool BasicPage::replaceActions(QAction* replacee, QAction* replacement)
{
    QList<QAction*> actionList = tabContextMenu->actions();
    bool found = false;
    int i;
    for (i = 0; i < actionList.size(); i ++) {
        if (actionList[i] == replacee) {
            found = true;
            break;
        }
    }
    if (found) { 
        tabContextMenu->insertAction(replacee, replacement);
        tabContextMenu->removeAction(replacee);
   }
   return found;
}

void BasicPage::updateFont()
{
    QFont font = ChatSettingsFontPage::getChatWindowFont();
    font.setStyleHint(QFont::Monospace);
    textBrowser->setFont(font);
}
