/*#include "chatsettings.hpp"
#include <QSettings>


ChatSettings::ChatSettings(QObject *parent) :
    QObject(parent)
{
}

void ChatSettings::save()
{
    QSettings set("settings.ini", QSettings::IniFormat);
    set.beginGroup("Chat");

    set.beginGroup("General");
    set.setValue("serverId", serverId);
    set.setValue("nick", nick);
    set.setValue("username", username);
    set.setValue("quitMessage", quitMessage);
    set.setValue("autoIdentify", autoIdentify);
    set.setValue("password", password);
    set.endGroup();

    set.beginGroup("Server");
    set.beginWriteArray("serverList");
    for (int i = 0; i < serverList.size(); i ++) {
        set.setArrayIndex(i);
        set.setValue("name", serverList[i].name);
        set.setValue("address", serverList[i].address);
        set.setValue("port", serverList[i].port);
        set.setValue("password", serverList[i].password);
        set.setValue("encoding", serverList[i].encoding);
    }
    set.endArray();
    set.endGroup();

    set.beginGroup("Channel");
    set.setValue("autojoinChannels", autojoinChannels);
    set.beginWriteArray("autojoinChannelList");
    for (int i = 0; i < autojoinChannelList.size(); i ++) {
        set.setArrayIndex(i);
        set.setValue("name", autojoinChannelList[i].name);
        set.setValue("password", autojoinChannelList[i].password);
    }
    set.endArray();
    set.setValue("waitBeforeAutojotining", waitBeforeAutojoining);
    set.setValue("timeToWaitBeforeAutojoining", timeToWaitBeforeAutojoining);
    set.endGroup();

    set.beginGroup("Font");
    set.setValue("chatListFont", chatListFont);
    set.setValue("chatWindowFont", chatWindowFont);
    set.setValue("userListFont", userListFont);
    set.endGroup();
}
*/
