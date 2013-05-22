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

#ifndef BASICPAGE_HPP
#define BASICPAGE_HPP

#include <QMainWindow>
#include <QMenu>
#include <QTextBrowser>
#include <QTreeWidgetItem>

class BasicPage : public QMainWindow
{
    Q_OBJECT
public:
    explicit BasicPage(QTreeWidgetItem* pageTab, QTreeWidget* tabTree);
    virtual ~BasicPage();

    QString getName() const {return tab->text(0);}
    QTreeWidgetItem* getTab() {return tab;}
    QMenu* getTabContextMenu() {return tabContextMenu;}
    static void eraseControlCharacters(QString& string);
    void clearTabIndication();
    bool isDisabled() const {return disabled;}

protected:
    /*the order of enums does matter, it's used for for tab coloring. see if-statements in BasicPage::updateTabIndication*/
    enum MessageType {None, Information, ServerResponse, Ctcp, Notice, Action, Message, Highlight, Timestamp};

    void print(const QString& string, MessageType type);
    bool replaceActions(QAction* replacee, QAction* replacement);

    bool disabled;
    QMenu* tabContextMenu;

private:
    QTreeWidgetItem* tab;

    QTextBrowser* textBrowser;
    MessageType lastMessageType;
    bool scrollDown;

    static QString getTime();
    static void formatForOutput(QString& string, MessageType type);
    static void urlify(QString& string);
    void updateTabIndication(MessageType event);

public slots:
    virtual void disable();
    virtual void enable();

private slots:
    void updateScrollbarPosition();
    void checkSliderPosition(int value);
    void updateFont();

};

#endif // BASICPAGE_HPP
