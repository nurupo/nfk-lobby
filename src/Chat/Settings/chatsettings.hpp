/*#ifndef CHATSETTINGS_HPP
#define CHATSETTINGS_HPP

#include <QObject>
#include <QString>
#include <QList>
#include <QFont>

class ChatSettings : public QObject
{
    Q_OBJECT
public:
    static void load(){}
    static void save();

    struct Channel
    {
        QString name;
        QString password;
    };

    struct Server
    {
        QString name;
        QString address;
        int port;
        QString password;
        QString encoding;
        int id;
    };

    static int getServerId()                        {return serverId;}
    static QString getNick()                        {return nick;}
    static QString getUsername()                    {return username;}
    static QString getQuitMessage()                 {return quitMessage;}
    static bool getAutoIdentify()                   {return autoIdentify;}
    static QString getPassword()                    {return password;}

    static QList<Server> getServerList()            {return serverList;}

    static bool getAutojoinChannels()               {return autojoinChannels;}
    static QList<Channel> getAutojoinChannelList()  {return autojoinChannelList;}
    static bool getWaitBeforeAutojoining()          {return waitBeforeAutojoining;}
    static int getTimeToWaitBeforeAutojoining()     {return timeToWaitBeforeAutojoining;}

    static QFont getChatListFont()                  {return chatListFont;}
    static QFont getChatWindowFont()                {return chatWindowFont;}
    static QFont getUserListFont()                  {return userListFont;}


    static void setServerId(int serverId)                                           {this->serverId = serverId;}
    static void setNick(const QString &nick)                                        {this->nick = nick;}
    static void setUsername(const QString &username)                                {this->username = username;}
    static void setQuitMessage(const QString &quitMessage)                          {this->quitMessage = quitMessage;}
    static void setAutoIdentify(bool autoIdentify)                                  {this->autoIdentify = autoIdentify;}
    static void setPassword(const QString &password)                                {this->password = password;}

    static void setServerList(const QList<Server> &serverList)                      {this->serverList = serverList;}

    static void setAutojoinChannels(bool autojoinChannels)                          {this->autojoinChannels = autojoinChannels;}
    static void setAutojoinChannelList(const QList<Channel> &autojoinChannelList)   {this->autojoinChannelList = autojoinChannelList;}
    static void setWaitBeforeAutojoining(bool waitBeforeAutojoining)                {this->waitBeforeAutojoining = waitBeforeAutojoining;}
    static void setTimeToWaitBeforeAutojoining(int timeToWaitBeforeAutojoining)     {this->timeToWaitBeforeAutojoining = timeToWaitBeforeAutojoining;}

    static void setChatListFont(const QFont &chatListFont)                          {this->chatListFont = chatListFont;}
    static void setChatWindowFont(const QFont &chatWindowFont)                      {this->chatWindowFont = chatWindowFont;}
    static void setUserListFont(const QFont &userListFont)                          {this->userListFont = userListFont;}

private:
    explicit ChatSettings(QObject *parent = 0);
    ChatSettings(const ChatSettings &cs);
    ChatSettings& operator=(const ChatSettings &cs);

    static int serverId;
    static QString nick;
    static QString username;
    static QString quitMessage;
    static bool autoIdentify;
    static QString password;

    static QList<Server> serverList;

    static bool autojoinChannels;
    static QList<Channel> autojoinChannelList;
    static bool waitBeforeAutojoining;
    static int timeToWaitBeforeAutojoining;

    static QFont chatListFont;
    static QFont chatWindowFont;
    static QFont userListFont;

signals:
    
public slots:
    
};

#endif // CHATSETTINGS_HPP
*/
