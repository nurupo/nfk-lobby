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

#include "ircclient.hpp"

#include <QDebug>
#include <QHashIterator>
#include <QStringList>

namespace IrcClient {

const QString IrcClient::ChannelPrefixes = "#+!&";

IrcClient::IrcClient() :
    QObject(0)
{
    isConnected = false;
    setUs = false;
    QObject::connect(&sock, &QTcpSocket::connected, this, &IrcClient::connectionEstablished);
    QObject::connect(&sock, &QTcpSocket::readyRead, this, &IrcClient::readServerMessages);
}

IrcClient::~IrcClient()
{
    QObject::disconnect(&sock, &QTcpSocket::disconnected, this, &IrcClient::disconnect);
    removeChannels();
}

void IrcClient::removeChannels()
{
    QMutableHashIterator<QString, Channel*> iterator(channels);
    while (iterator.hasNext()) {
        delete iterator.next().value();
    }
    channels.clear();
}

void IrcClient::disconnect()
{
    if (!isConnected) {
        return;
    }

    if (sock.isOpen()) {
        if (!hasQuit) {
            sendQuit();
            quitUser(us, QString("Quit: %1").arg(quitMessage));
        }
        sock.disconnectFromHost();
    }
    emit disconnecting();
    removeChannels();
    calledWho.clear();
    isConnected = false;
    emit disconnected();
}

void IrcClient::connect(const QString &address, int port, const QString &password, const QString &nick, const QString &username, const QString &encoding)
{
    if (isConnected) {
        return;
    }
    isConnected = true;
    hasQuit = false;
    setUs = false;
    emit connecting(address, port);
    this->address = address;
    this->port = port;
    this->password = password;
    us.nick = nick;
    us.username = username;
    setCodec(encoding);
    QObject::connect(&sock, &QTcpSocket::disconnected,  this, &IrcClient::disconnect);
    sock.connectToHost(address, port);
}

void IrcClient::setCodec(const QString &encoding)
{
    codec = QTextCodec::codecForName(encoding.toLatin1());
    /*TODO: make a list of all available encodings in the GUI, so that there won't be any need to do this check*/
    if (codec == 0) {
        qDebug() << "unsupported encoding";
    }
}

void IrcClient::joinChannel(const QString &name, const QString &password)
{
    sendRaw(QString("JOIN %1 %2")
            .arg(name)
            .arg(password)
   );
}

void IrcClient::sendRaw(const QString &string)
{
    if (sock.isOpen()) {
        sock.write(codec->fromUnicode(string + "\r\n"));
        processUserLine(string);
        qDebug() << "<--" << string;
    }
}

void IrcClient::sendNotice(const QString &nick, const QString &string)
{
    sendRaw(QString("NOTICE %1 :%2")
            .arg(nick)
            .arg(string)
   );
}

void IrcClient::sendCtcpReply(const QString &nick, const QString &string)
{
    sendNotice(nick, QString("%1%2%1")
                        .arg(Ctcp)
                        .arg(string)
   );
}

void IrcClient::sendMessage(const QString &target, const QString &string)
{
    sendRaw(QString("PRIVMSG %1 :%2")
            .arg(target)
            .arg(string)
    );
}

void IrcClient::sendCtcpRequest(const QString &target, const QString &string)
{
    sendMessage(target, QString("%1%2%1")
                        .arg(Ctcp)
                        .arg(string)
    );
}

void IrcClient::sendAction(const QString &target, const QString &string)
{
    sendCtcpRequest(target, QString("ACTION %1").arg(string));
}

void IrcClient::sendQuit()
{
    sendRaw(QString("QUIT :%1").arg(quitMessage));
}

void IrcClient::sendPart(const QString &channel)
{
    sendRaw(QString("PART %1").arg(channel));
}

void IrcClient::callWho(const QString &target)
{
    sendRaw(QString("WHO %1").arg(target));
    calledWho[QString(target)];
}

void IrcClient::identify(const QString &password)
{
    sendRaw(QString("NickServ IDENTIFY %1").arg(password));
}

void IrcClient::connectionEstablished()
{
    if (!password.isEmpty()) {
        sendRaw(QString("PASS %1").arg(password));
    }
    sendRaw(QString("NICK %1").arg(us.nick));

    sendRaw(QString("USER %1 0 0 :%2")
            .arg(us.username)
            .arg(us.nick)
   );
}

void IrcClient::readServerMessages()
{
    while (sock.canReadLine()) {
        QByteArray line = sock.readLine();
        line.chop(2);
        processServerLine(codec->toUnicode(line));
    }
}

void IrcClient::processUserLine(const QString &line)
{
    if (line.startsWith("PONG ", Qt::CaseInsensitive)) {
        return;
    }

    QStringListIterator iterator (line.split(' ', QString::SkipEmptyParts));
    QString command = iterator.next();
    if (!iterator.hasNext()) {
        return;
    }

    QString target = iterator.next();
    if (target.at(0) == ':') {
        target = target.mid(1);
    }
    const QString targetLower = target.toLower();

    if (!command.compare("PRIVMSG", Qt::CaseInsensitive)) {
        line.indexOf(target);
        const int ctcpIndex = line.indexOf(QString(":%1").arg(Ctcp));
        if (ctcpIndex > 0 && line.endsWith(Ctcp)) {
            const QString request = line.mid(ctcpIndex + 1 + 1, line.length() - (ctcpIndex + 1 + 1) - 1);
            if (request.startsWith("ACTION", Qt::CaseInsensitive)) {
                if (isChannel(target) && channels.contains(targetLower)) {
                    emit channels[targetLower]->action(us, request.mid(7));
                } else if (!isChannel(target)) {
                    emit action(us, request.mid(7), target);
                }
            } else {
                emit ctcpRequest(target, us, request);
            }

        } else if (channels.contains(targetLower)) {
            emit channels[targetLower]->channelMessage(us, line.mid(line.indexOf(" :") + 2));
        } else if (!isChannel(target)) {
            emit privateMessage(us, line.mid(line.indexOf(" :") + 2), target);
        } else {
            emit unknown(line);
        }
    } else if (!command.compare("NOTICE", Qt::CaseInsensitive)) {
        const int ctcpIndex = line.indexOf(QString(":%1").arg(Ctcp));
        if (ctcpIndex > 0 && line.endsWith(Ctcp)) {
            const QString request = line.mid(ctcpIndex + 1 + 1, line.length() - (ctcpIndex + 1 + 1) - 1);
            int fisrtSpace = request.indexOf(' ');
            emit ctcpReply(target, us, request.left(fisrtSpace), request.mid(fisrtSpace + 1));
        } else {
            if (isChannel(target)) {
                emit channels[targetLower]->notice(us, line.mid(line.indexOf(" :") + 2));
            } else {
                emit notice(us, line.mid(line.indexOf(" :") + 2));
            }
        }
    }
}

void IrcClient::processServerLine(const QString &line)
{
    qDebug() << "-->" << line;
    if (line.startsWith("PING ", Qt::CaseInsensitive)) {
        const QString response = line.mid(5);
        sendRaw("PONG " + response);
        return;
    }
    QStringListIterator iterator(line.split(' ', QString::SkipEmptyParts));
    QString senderInfo = iterator.next();
    QString command = iterator.next();

    User sender;
    QString target;

    if (senderInfo.startsWith(':')) {
        const int exclamation = senderInfo.indexOf('!');
        const int at = senderInfo.indexOf('@');
        if (exclamation > 0 && at > exclamation) {
            sender.nick = senderInfo.mid(1, exclamation - 1);
            sender.username = senderInfo.mid(exclamation + 1, at - (exclamation + 1));
            sender.hostname = senderInfo.mid(at + 1);
        } else {
            if (iterator.hasNext()) {
                bool ok;
                command.toInt(&ok);
                if (ok && command.length() == 3) {
                    const QString response = line.mid(line.indexOf(command, senderInfo.length()) + 3 + 1);
                    processServerResponse(command, response);
                    return;
                }
                sender.nick = senderInfo;
                if (sender.nick.at(0) == ':') {
                    sender.nick = sender.nick.mid(1);
                }
            } else {
                emit unknown(line);
                return;
            }
        }
    }
    target = iterator.next();
    if (target.at(0) == ':') {
        target = target.mid(1);
    }
    const QString targetLower = target.toLower();
    if (!command.compare("PRIVMSG", Qt::CaseInsensitive)) {
        const int ctcpIndex = line.indexOf(QString(":%1").arg(Ctcp));
        if (ctcpIndex > 0 && line.endsWith(Ctcp)) {
            const QString request = line.mid(ctcpIndex + 1 + 1, line.length() - (ctcpIndex + 1 + 1) - 1);

            if (request.startsWith("ACTION", Qt::CaseInsensitive)) {
                if (channels.contains(targetLower)) {
                    emit channels[targetLower]->action(sender, request.mid(7));
                } else if (!isChannel(target)) {
                    emit action(sender, request.mid(7), us.nick);
                }
            } else {
                emit ctcpRequest(target, sender, request);

                if (!request.compare("VERSION", Qt::CaseInsensitive)) {
                    sendCtcpReply(sender.nick, "VERSION " + version);
                } else if (!request.compare("TIME", Qt::CaseInsensitive)) {
                    sendCtcpReply(sender.nick, "TIME " + QDateTime::currentDateTime().toString(Qt::DefaultLocaleShortDate));
                } else if (!request.compare("FINGER", Qt::CaseInsensitive)) {
                    sendCtcpReply(sender.nick, "FINGER " + Finger);
                } else if (request.startsWith("PING", Qt::CaseInsensitive)) {
                    sendCtcpReply(sender.nick, "PING " + QString("%1").arg(QDateTime::currentMSecsSinceEpoch()/1000));
                }
            }
        } else if (isChannel(target)) {
            emit channels[targetLower]->channelMessage(sender, line.mid(line.indexOf(" :") + 2));
        } else if (!target.compare(us.nick, Qt::CaseInsensitive)) {
            emit privateMessage(sender, line.mid(line.indexOf(" :") + 2), us.nick);
        } else {
            emit unknown(line);
        }
    } else if (!command.compare("JOIN", Qt::CaseInsensitive)) {
        if (!sender.nick.compare(us.nick, Qt::CaseInsensitive)) {
            channels[targetLower] = new Channel(target);
            emit join(*channels[targetLower]);
        } else {
            channels[targetLower]->addUser(sender.nick);
        }
        emit channels[targetLower]->join(sender);
    } else if (!command.compare("PART", Qt::CaseInsensitive)) {
        if (!sender.nick.compare(us.nick, Qt::CaseInsensitive)) {
            emit part(*channels[targetLower]);
            emit channels[targetLower]->part(sender);
            delete channels.take(targetLower);
        } else {
            emit channels[targetLower]->part(sender);
            channels[targetLower]->removeUser(sender.nick);
        }
    } else if (!command.compare("NICK", Qt::CaseInsensitive)) {
        renameUser(sender, target);
        if (!sender.nick.compare(us.nick, Qt::CaseInsensitive)) {
            us.nick = target;
        }
    } else if (!command.compare("NOTICE", Qt::CaseInsensitive)) {
        const int ctcpIndex = line.indexOf(QString(":%1").arg(Ctcp));
        if (ctcpIndex > 0 && line.endsWith(Ctcp)) {
            const QString request = line.mid(ctcpIndex + 1 + 1, line.length() - (ctcpIndex + 1 + 1) - 1);
            int firstSpace = request.indexOf(' ');
            emit ctcpReply(target, sender, request.left(firstSpace), request.mid(firstSpace + 1));
        } else {
            if (isChannel(target)) {
                emit channels[targetLower]->notice(sender, line.mid(line.indexOf(" :") + 2));
            } else {
                emit notice(sender, line.mid(line.indexOf(" :") + 2));
            }
        }
    } else if (!command.compare("QUIT", Qt::CaseInsensitive)) {
        if (!sender.nick.compare(us.nick, Qt::CaseInsensitive)) {
            /*server doesn't send QUIT message when we quit, instead it sends `ERROR :Closing Link: hostname (Quit: message)`*/
        } else {
            quitUser(sender, line.mid(line.indexOf(" :") + 2));
            removeUser(sender.nick);
        }
    } else if (line.startsWith("ERROR :Closing Link: ", Qt::CaseInsensitive)) {
        hasQuit = true;
        int quitMessage = line.indexOf("(");
        quitUser(us, line.mid(quitMessage + 1, line.length() - quitMessage - 2));
    } else if (!command.compare("MODE", Qt::CaseSensitive)) {
        QString mode = line.mid(line.indexOf(target, 2) + target.length() + 1);
        if (mode.at(0) == ':') {
            mode = mode.mid(1);
        }
        processMode(target, sender, mode);
    } else if (!command.compare("KICK", Qt::CaseSensitive)) {
        const QString recipient = iterator.next();
        emit channels[targetLower]->kick(sender, line.mid(line.indexOf(" :") + 2), recipient);
        if (!recipient.compare(us.nick, Qt::CaseInsensitive)) {
            delete channels.take(targetLower);
        } else {
            channels[targetLower]->removeUser(recipient);
        }
    }
    /*TODO: invite*/
    else if (!command.compare("TOPIC", Qt::CaseInsensitive)) {
        QString topicText = line.mid(line.indexOf(" :") + 2);
        QDateTime topicDate = QDateTime::currentDateTime();
        Channel* channel = channels[targetLower];
        emit channel->topicChanged(topicText, sender, topicDate);
        channel->setTopicText(topicText);
        channel->setTopicSetter(sender);
        channel->setTopicDate(topicDate);
    }
}

void IrcClient::processMode(const QString &target, const User &sender, const QString &mode)
{
    QStringListIterator iterator(mode.split(' ', QString::SkipEmptyParts));
    const QString modes = iterator.next();
    const bool set = modes.at(0) == '+';

    const QString targetLower = target.toLower();

    if (channels.contains(targetLower)) {
        Channel* channel = channels[targetLower];

        for (int i = 1; i < modes.size(); i ++) {
            const QChar modeChar = modes.at(i);

            if (modeChar == 'v') {
                const QString recipient = iterator.next();
                UserMode newUserMode;
                newUserMode.op = channel->getUserMode(recipient).op;
                newUserMode.voice = set;
                channel->setUserMode(recipient, newUserMode);
            } else if (modeChar == 'o') {
                const QString recipient = iterator.next();
                UserMode newUserMode;
                newUserMode.voice = channel->getUserMode(recipient).voice;
                newUserMode.op = set;
                channel->setUserMode(recipient, newUserMode);
            }

        }
        emit channel->modeChanged(sender, mode);
    } else {
        if (set && mode.contains('i') && !setUs) {
            us.nick = target;
            nickChange(target);
            callWho(target);
        }
        emit userModeChanged(target, sender, mode);
    }
}


void IrcClient::processServerResponse(const QString &code, const QString &response)
{
    if (code == "001") {
        emit connected();
    } else if (code == "433") {
        emit nickAlreadyInUse();
    } else if (code == "353") {
        QStringListIterator iterator (response.split(' ', QString::SkipEmptyParts));
        iterator.next(); /* our nick */
        iterator.next(); /* have no idea what that character means. TODO: figure that out. */
        QString channel = iterator.next().toLower();
        while (iterator.hasNext()) {
            QString prefixedNick = iterator.next();
            if (prefixedNick.at(0) == ':') {
                prefixedNick = prefixedNick.mid(1);
            }
            /* only one of statuses can be presented at a time ['@'|'+'] */
            bool isOp = prefixedNick.at(0) == '@';
            bool hasVoice = prefixedNick.at(0) == '+';
            if (isOp || hasVoice) {
                prefixedNick = prefixedNick.mid(1);
            }
            channels[channel]->addUser(prefixedNick, isOp, hasVoice);
        }
        return;
    } else if (code == "366") {
        QStringListIterator iterator (response.split(' ', QString::SkipEmptyParts));
        iterator.next(); /* our nick */
        QString channel = iterator.next().toLower();
        emit channels[channel]->nicksReady();
        return;
    } else if (code == "352") {
        QStringListIterator iterator (response.split(' ', QString::SkipEmptyParts));
        iterator.next(); /* our nick */
        QString channelName = iterator.next(); /*channel name or '*' for no channel*/
        QString username = iterator.next();
        QString hostname = iterator.next();
        iterator.next(); /* server user connected to */
        QString nick = iterator.next();
        QString status = iterator.next();

        if (!nick.compare(us.nick, Qt::CaseInsensitive)) {
            us.username = username;
            us.hostname = hostname;
            qDebug() << "username" << username << ";" << "hostname" << hostname;
            setUs = true;
        }

        if (hasChannel(channelName)) {
            Channel* channel = channels[channelName];
            if (channel->hasUser(nick)) {
                UserMode& mode = channel->getUserMode(nick);
                if (status.length() > 1) {
                    if (status.at(1) == '@') {
                        mode.op = true;
                    } else if (status.at(1) == '+') {
                        mode.voice =true;
                    }
                }
            }
        }

        if (calledWho.contains(channelName) || calledWho.contains(nick)) {
            return;
        }
    } else if (code == "315") {
        QStringListIterator iterator (response.split(' ', QString::SkipEmptyParts));
        iterator.next(); /* our nick */
        QString whoTarget = iterator.next();
        if (calledWho.contains(whoTarget)) {
            calledWho.remove(whoTarget);
            return;
        }
    } else if (code == "332") {
        QStringListIterator iterator (response.split(' ', QString::SkipEmptyParts));
        iterator.next(); /* our nick */
        QString channelName = iterator.next();
        if (hasChannel(channelName)) {
            QString topicText = response.mid(response.indexOf(" :") + 2);

            Channel* channel = channels[channelName];
            channel->setTopicText(topicText);
            emit channel->topicTextResponse(topicText);
        }
        return;
    } else if (code == "333") {
        QStringListIterator iterator (response.split(' ', QString::SkipEmptyParts));
        iterator.next(); /* our nick */
        QString channelName = iterator.next();
        if (hasChannel(channelName)) {
            QString senderInfo = iterator.next();
            const int exclamation = senderInfo.indexOf('!');
            const int at = senderInfo.indexOf('@');
            User topicSetter;
            topicSetter.nick = senderInfo.mid(0, exclamation);
            topicSetter.username = senderInfo.mid(exclamation + 1, at - (exclamation + 1));
            topicSetter.hostname = senderInfo.mid(at + 1);
            QDateTime topicDate = QDateTime::fromTime_t(iterator.next().toLong());

            Channel* channel = channels[channelName];
            channel->setTopicSetter(topicSetter);
            channel->setTopicDate(topicDate);
            emit channel->topicInfoResponse(topicSetter, topicDate);
        }
        return;
    }
    if (response.startsWith(us.nick, Qt::CaseInsensitive)) {
        QString modifiedResponse(response);
        modifiedResponse.replace(" :", " ");
        modifiedResponse = modifiedResponse.mid(us.nick.length());
        if (modifiedResponse.at(0) == ' ') {
            modifiedResponse = modifiedResponse.mid(1);
        }
        emit serverResponse(modifiedResponse);
    } else {
        QString modifiedResponse(response);
        modifiedResponse.replace("* ", "");
        modifiedResponse.replace(" :", " ");
        emit serverResponse(modifiedResponse);
    }
}

bool IrcClient::isChannel(const QString &channelName) const
{
    return ChannelPrefixes.contains(channelName.at(0));
}

void IrcClient::renameUser(const User &user, const QString &newNick)
{
    if (user.nick == us.nick) {
        emit nickChange(newNick);
        us.nick = newNick;
    }
    QMutableHashIterator<QString, Channel*> iterator(channels);
    while (iterator.hasNext()) {
        Channel* channel = iterator.next().value();
        if (channel->hasUser(user.nick)) {
            emit channel->nickChange(user, newNick);
            channel->renameUser(user.nick, newNick);
        }
    }
}

void IrcClient::quitUser(const User &user, const QString &message)
{
    QMutableHashIterator<QString, Channel*> iterator(channels);
    while (iterator.hasNext()) {
        Channel* channel = iterator.next().value();
        if (channel->hasUser(user.nick)) {
            emit channel->quit(user, message);
        }
    }
}

void IrcClient::removeUser(const QString &nick)
{
    QMutableHashIterator<QString, Channel*> iterator(channels);
    while (iterator.hasNext()) {
        iterator.next().value()->removeUser(nick);
    }
}

} // namespace IrcClient
