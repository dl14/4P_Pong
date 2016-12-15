#ifndef USERGO_H
#define USERGO_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>

#include "sendmsghandler.h"

class UserGO : public QObject
{
    Q_OBJECT
public:
    UserGO(QObject *parent = 0);
signals:
    void sendObj(QJsonObject obj);
    void rcvObj(QJsonObject obj);

    void addUser(QJsonObject user);
    void sendUserName(QUuid id, QString name);
    void initUserRcvd();
    void sendUserId(QUuid id);

    void sendUserNameToInfo(QString name);

    void rcvUserPayload(QJsonObject obj);

    void showShape(QJsonObject shape);

    void initPayload(QJsonObject obj);
public slots:
    void rcvUserName(QString name){m_UserName = name;}
    void checkPayload(QJsonObject payload);
    void rcvUser(QJsonObject user);
    void setStateAfterDisconnection(){m_UserState = UserStates::Init;}
private:
    enum UserStates{Init, InitPayloadRcvd, FirstUserRcvd} m_UserState;
    QUuid m_RootID;
    QUuid m_GMID;
    QUuid m_UserID;
    QString m_UserName;
    QJsonObject m_InitPayload;
};

#endif // USERGO_H
