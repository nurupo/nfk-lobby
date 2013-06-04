#-------------------------------------------------
#
# Project created by QtCreator 2013-01-29T20:11:26
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = NFK-Lobby
TEMPLATE = app

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
CONFIG(release, debug|release):CONFIG += static

QMAKE_CXXFLAGS += -std=c++11

RC_FILE = ../../resources/appicon.rc

INCLUDEPATH += ../../src/

SOURCES += \
    ../../src/main.cpp\
    ../../src/mainwindow.cpp \
    ../../src/PlanetScanner/planetscannerdock.cpp \
    ../../src/Chat/chatdock.cpp \
    ../../src/PlanetScanner/planet.cpp \
    ../../src/PlanetScanner/game.cpp \
    ../../src/PlanetScanner/planetscannersettingsdialog.cpp \
    ../../src/Chat/usertreemodel.cpp \
    ../../src/Chat/chattabtree.cpp \
    ../../src/IrcClient/channel.cpp \
    ../../src/Chat/Pages/channelpage.cpp \
    ../../src/Chat/Pages/basicpage.cpp \
    ../../src/Chat/Pages/serverpage.cpp \
    ../../src/Chat/useritem.cpp \
    ../../src/Chat/topiclabel.cpp \
    ../../src/Chat/usertree.cpp \
    ../../src/Chat/Pages/privatepage.cpp \
    ../../src/Chat/Settings/chatsettingsdialog.cpp \
    ../../src/Chat/Settings/chatsettingsserverpage.cpp \
    ../../src/Chat/Settings/chatsettingsgeneralpage.cpp \
    ../../src/Chat/Settings/chatsettingschannelpage.cpp \
    ../../src/Chat/Settings/chatsettingsfontpage.cpp \
    ../../src/Settings/basicsettingsdialog.cpp \
    ../../src/Settings/abstractsettingspage.cpp \
    ../../src/Chat/Settings/chatsettings.cpp \
    ../../src/Chat/Settings/chatsettingsmodifyserverdialog.cpp \
    ../../src/IrcClient/ircclient.cpp \
    ../../src/GamePreferences/gamepreferencesdock.cpp \
    ../../src/GamePreferences/checkboxdelegate.cpp \
    ../../src/GamePreferences/preferencessortingmodel.cpp \
    ../../src/GamePreferences/Settings/gamepreferencessettingsdialog.cpp \
    ../../src/GamePreferences/Settings/gamepreferencesgeneralpage.cpp \
    ../../src/GamePreferences/Settings/gamepreferencespreferencespage.cpp \
    ../../src/GamePreferences/gametype.cpp \
    ../../src/GamePreferences/Settings/gamepreferencestablepage.cpp \
    ../../src/aboutdialog.cpp \
    ../../src/licensedialog.cpp \
    ../../src/PlanetScanner/planettreemodel.cpp \
    ../../src/PlanetScanner/planettreesortfilterproxymodel.cpp

HEADERS  += \
    ../../src/mainwindow.hpp \
    ../../src/PlanetScanner/planetscannerdock.hpp \
    ../../src/Chat/chatdock.hpp \
    ../../src/PlanetScanner/planet.hpp \
    ../../src/PlanetScanner/game.hpp \
    ../../src/PlanetScanner/planetscannersettingsdialog.hpp \
    ../../src/Chat/usertreemodel.hpp \
    ../../src/Chat/chattabtree.hpp \
    ../../src/IrcClient/channel.hpp \
    ../../src/Chat/Pages/channelpage.hpp \
    ../../src/Chat/Pages/basicpage.hpp \
    ../../src/Chat/Pages/serverpage.hpp \
    ../../src/Chat/useritem.hpp \
    ../../src/Chat/topiclabel.hpp \
    ../../src/Chat/usertree.hpp \
    ../../src/IrcClient/usermode.hpp \
    ../../src/IrcClient/user.hpp \
    ../../src/Chat/Pages/privatepage.hpp \
    ../../src/Chat/Settings/chatsettingsdialog.hpp \
    ../../src/Chat/Settings/chatsettingsserverpage.hpp \
    ../../src/Chat/Settings/chatsettingsgeneralpage.hpp \
    ../../src/Chat/Settings/chatsettingschannelpage.hpp \
    ../../src/Chat/Settings/chatsettingsfontpage.hpp \
    ../../src/Settings/basicsettingsdialog.hpp \
    ../../src/Settings/abstractsettingspage.hpp \
    ../../src/Chat/Settings/chatsettings.hpp \
    ../../src/Chat/Settings/chatsettingsmodifyserverdialog.hpp \
    ../../src/IrcClient/ircclient.hpp \
    ../../src/GamePreferences/gamepreferencesdock.hpp \
    ../../src/GamePreferences/checkboxdelegate.hpp \
    ../../src/GamePreferences/preferencessortingmodel.hpp \
    ../../src/GamePreferences/Settings/gamepreferencessettingsdialog.hpp \
    ../../src/GamePreferences/Settings/gamepreferencesgeneralpage.hpp \
    ../../src/GamePreferences/Settings/gamepreferencespreferencespage.hpp \
    ../../src/GamePreferences/gametype.hpp \
    ../../src/GamePreferences/Settings/gamepreferencestablepage.hpp \
    ../../src/aboutdialog.hpp \
    ../../src/licensedialog.hpp \
    ../../src/PlanetScanner/planettreemodel.hpp \
    ../../src/PlanetScanner/planettreesortfilterproxymodel.hpp

RESOURCES += \
    ../../resources/icons.qrc \
    ../../resources/license.qrc
