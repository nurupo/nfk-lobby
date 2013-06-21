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

CONFIG += c++11

RC_FILE = ../../resources/appicon.rc

INCLUDEPATH += ../../src/

SOURCES += \
    ../../src/aboutdialog.cpp \
    ../../src/licensedialog.cpp \
    ../../src/main.cpp \
    ../../src/mainwindow.cpp \
    ../../src/Chat/chatdock.cpp \
    ../../src/Chat/chattabtree.cpp \
    ../../src/Chat/chatwindow.cpp \
    ../../src/Chat/topiclabel.cpp \
    ../../src/Chat/useritem.cpp \
    ../../src/Chat/usertree.cpp \
    ../../src/Chat/usertreemodel.cpp \
    ../../src/Chat/Pages/basicpage.cpp \
    ../../src/Chat/Pages/channelpage.cpp \
    ../../src/Chat/Pages/privatepage.cpp \
    ../../src/Chat/Pages/serverpage.cpp \
    ../../src/Chat/Settings/chatchannelsettingspage.cpp \
    ../../src/Chat/Settings/chatfontsettingspage.cpp \
    ../../src/Chat/Settings/chatgeneralsettingspage.cpp \
    ../../src/Chat/Settings/chatmodifyserversettingsdialog.cpp \
    ../../src/Chat/Settings/chatserversettingspage.cpp \
    ../../src/Chat/Settings/chatsettings.cpp \
    ../../src/Chat/Settings/chatsettingsdialog.cpp \
    ../../src/Chat/Settings/customhinttreeview.cpp \
    ../../src/GamePreferences/checkboxdelegate.cpp \
    ../../src/GamePreferences/gamepreferencesdock.cpp \
    ../../src/GamePreferences/gamepreferenceswindow.cpp \
    ../../src/GamePreferences/gametype.cpp \
    ../../src/GamePreferences/preferencessortingmodel.cpp \
    ../../src/GamePreferences/Settings/gamepreferencesgeneralsettingspage.cpp \
    ../../src/GamePreferences/Settings/gamepreferencespreferencessettingspage.cpp \
    ../../src/GamePreferences/Settings/gamepreferencessettings.cpp \
    ../../src/GamePreferences/Settings/gamepreferencessettingsdialog.cpp \
    ../../src/GamePreferences/Settings/gamepreferencestablesettingspage.cpp \
    ../../src/IrcClient/channel.cpp \
    ../../src/IrcClient/ircclient.cpp \
    ../../src/PlanetScanner/game.cpp \
    ../../src/PlanetScanner/planet.cpp \
    ../../src/PlanetScanner/planetscannerdock.cpp \
    ../../src/PlanetScanner/planetscannerwindow.cpp \
    ../../src/PlanetScanner/planettreemodel.cpp \
    ../../src/PlanetScanner/planettreesortfilterproxymodel.cpp \
    ../../src/PlanetScanner/Settings/filtersettingspage.cpp \
    ../../src/PlanetScanner/Settings/gamesettingspage.cpp \
    ../../src/PlanetScanner/Settings/miscsettingspage.cpp \
    ../../src/PlanetScanner/Settings/planetscannersettings.cpp \
    ../../src/PlanetScanner/Settings/planetsettingspage.cpp \
    ../../src/PlanetScanner/Settings/settingsdialog.cpp \
    ../../src/PlanetScanner/Settings/tablesettingspage.cpp \
    ../../src/Settings/basicsettingsdialog.cpp \
    ../../src/Settings/abstractsettingspage.cpp

HEADERS  += \
    ../../src/aboutdialog.hpp \
    ../../src/licensedialog.hpp \
    ../../src/mainwindow.hpp \
    ../../src/Chat/chatdock.hpp \
    ../../src/Chat/chattabtree.hpp \
    ../../src/Chat/chatwindow.hpp \
    ../../src/Chat/topiclabel.hpp \
    ../../src/Chat/useritem.hpp \
    ../../src/Chat/usertree.hpp \
    ../../src/Chat/usertreemodel.hpp \
    ../../src/Chat/Pages/basicpage.hpp \
    ../../src/Chat/Pages/channelpage.hpp \
    ../../src/Chat/Pages/privatepage.hpp \
    ../../src/Chat/Pages/serverpage.hpp \
    ../../src/Chat/Settings/chatchannelsettingspage.hpp \
    ../../src/Chat/Settings/chatfontsettingspage.hpp \
    ../../src/Chat/Settings/chatgeneralsettingspage.hpp \
    ../../src/Chat/Settings/chatmodifyserversettingsdialog.hpp \
    ../../src/Chat/Settings/chatserversettingspage.hpp \
    ../../src/Chat/Settings/chatsettings.hpp \
    ../../src/Chat/Settings/chatsettingsdialog.hpp \
    ../../src/Chat/Settings/customhinttreeview.hpp \
    ../../src/GamePreferences/checkboxdelegate.hpp \
    ../../src/GamePreferences/gamepreferencesdock.hpp \
    ../../src/GamePreferences/gamepreferenceswindow.hpp \
    ../../src/GamePreferences/gametype.hpp \
    ../../src/GamePreferences/preferencessortingmodel.hpp \
    ../../src/GamePreferences/Settings/gamepreferencesgeneralsettingspage.hpp \
    ../../src/GamePreferences/Settings/gamepreferencespreferencessettingspage.hpp \
    ../../src/GamePreferences/Settings/gamepreferencessettings.hpp \
    ../../src/GamePreferences/Settings/gamepreferencessettingsdialog.hpp \
    ../../src/GamePreferences/Settings/gamepreferencestablesettingspage.hpp \
    ../../src/IrcClient/channel.hpp \
    ../../src/IrcClient/ircclient.hpp \
    ../../src/IrcClient/user.hpp \
    ../../src/IrcClient/usermode.hpp \
    ../../src/PlanetScanner/game.hpp \
    ../../src/PlanetScanner/planet.hpp \
    ../../src/PlanetScanner/planetscannerdock.hpp \
    ../../src/PlanetScanner/planetscannerwindow.hpp \
    ../../src/PlanetScanner/planettreemodel.hpp \
    ../../src/PlanetScanner/planettreesortfilterproxymodel.hpp \
    ../../src/PlanetScanner/Settings/filtersettingspage.hpp \
    ../../src/PlanetScanner/Settings/gamesettingspage.hpp \
    ../../src/PlanetScanner/Settings/miscsettingspage.hpp \
    ../../src/PlanetScanner/Settings/planetscannersettings.hpp \
    ../../src/PlanetScanner/Settings/planetsettingspage.hpp \
    ../../src/PlanetScanner/Settings/settingsdialog.hpp \
    ../../src/PlanetScanner/Settings/tablesettingspage.hpp \
    ../../src/Settings/abstractsettingspage.hpp \
    ../../src/Settings/basicsettingsdialog.hpp

RESOURCES += \
    ../../resources/icons.qrc \
    ../../resources/license.qrc
