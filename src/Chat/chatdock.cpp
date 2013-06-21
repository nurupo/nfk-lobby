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

#include "chatdock.hpp"
#include "chatwindow.hpp"

namespace Chat {

Dock::Dock(QWidget* parent) :
    QDockWidget(parent)
{
    const QString dockName = "Chat";
    setObjectName(dockName);
    setWindowTitle(dockName);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetClosable);

    Window* window = new Window();
    setWidget(window);
    window->setParent(this);
}


} // namespace Chat
