#ifndef SENDMSGHANDLER_H
#define SENDMSGHANDLER_H

#include <QObject>
#include <QUuid>
#include <QJsonObject>
#include <QTcpSocket>
#include <QDebug>

class SendMsgHandler : public QObject
{
    Q_OBJECT
public:
    SendMsgHandler(QObject *parent = 0);
    ~SendMsgHandler();
signals:
    void sendObj(QJsonObject obj);
    void responseRejected(QJsonObject response);
    void completePayload(QJsonObject obj);
public slots:
    void sendPayload(QJsonObject payload);
    void rcvPayload(QJsonObject payload);
private:
    QUuid m_MsgId;
    QUuid m_ObjId;
    enum States {Init, MsgSent, Waiting, Rejected, MsgComplete} m_HandlerStates;
};

#endif // SENDMSGHANDLER_H
