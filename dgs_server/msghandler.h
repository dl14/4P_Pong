#ifndef MSGHANDLER_H
#define MSGHANDLER_H

#include "globalvalues.h"

#include <QObject>
#include <QTcpSocket>
#include <QUuid>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>
#include <QByteArray>

class MsgHandler : public QObject
{
    Q_OBJECT
public:
    MsgHandler(QTcpSocket *socket, QObject *parent = 0);
public slots:
    void sendInitData(QJsonArray initArray, QUuid rootId);
    void checkPayload(QJsonObject obj, QUuid msgId);
    void sendResponse(QString errorMsg, QUuid msgId);
    void sendBadResponse(QString errorMsg, QString msgId); //only connected to parser? Disconnect as well?
    void sendRepoChange(QJsonObject obj);
    void initUserAdded(QUuid msgId);
    void disconnectClient();
signals:
    void insertObject(QJsonObject obj, QUuid msgId);
    void removeObject(QJsonObject obj, QUuid msgId);
    void changeObject(QJsonObject obj, QUuid msgId);

    void sendToLogger(QByteArray data, int socket);
private:
    enum States {Init, InitArraySent, UserPayloadRecvd, Error} m_State;
    QTcpSocket *m_Socket;
    QList<QUuid> m_Messages;
    QUuid m_RootID;
};

#endif // MSGHANDLER_H
