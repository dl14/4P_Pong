#include "usergo.h"

UserGO::UserGO(QObject *parent) : QObject(parent)
{
    m_UserState = UserStates::Init;
}

void UserGO::checkPayload(QJsonObject payload)
{
    if(m_UserState == UserStates::Init){
        m_InitPayload = payload;
        QJsonArray initPayload = payload.value(QString("InitArray")).toArray();
        bool sendNewClient = true;
        QUuid objID = QUuid().createUuid();
        int objV = 1;
        //int numUsers = 0;
        for(int i = 0; i < initPayload.size(); i++){
            QJsonObject obj = initPayload[i].toObject();
            if(obj.value(QString("ObjType")).toString() == "UserPayload"){
                if(obj.value(QString("UserName")).toString() == "root"){
                    m_RootID = QUuid(obj.value(QString("ObjUuid")).toString());
                }
                else if(obj.value(QString("UserName")).toString() == "GameMaster"){
                    m_GMID = QUuid(obj.value(QString("ObjUuid")).toString());
                }
                if(obj.value(QString("UserName")).toString() == m_UserName){
                    sendNewClient = false;
                    objID = QUuid(obj.value(QString("ObjUuid")).toString());
                    objV = obj.value(QString("ObjVersion")).toInt() + 1;
                }
                emit sendUserName(QUuid(obj.value(QString("ObjUuid")).toString()), obj.value(QString("UserName")).toString());
            }
            else if(obj.value(QString("ObjType")).toString() == "ShapePayload")
                if(obj.value(QString("ObjVersion")).toInt() == 1)
                    emit showShape(obj);
        }

        QJsonObject userPayload;
        userPayload["ObjOwnerUuid"] = m_RootID.toString();
        userPayload["ObjType"] = QString("UserPayload");
        userPayload["ObjUuid"] = objID.toString();
        m_UserID = objID.toString();
        //emit sendUserId(objID);
        userPayload["ObjVersion"] = objV;
        userPayload["UserName"] = m_UserName;
        userPayload["UserAuthorizationLevel"] = int(2);

        SendMsgHandler * handler = new SendMsgHandler;
        connect(this, &UserGO::sendObj, handler, &SendMsgHandler::sendPayload);
        connect(this, &UserGO::rcvUserPayload, handler, &SendMsgHandler::rcvPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &UserGO::addUser);
        connect(handler, &SendMsgHandler::completePayload, this, &UserGO::rcvUser);

        emit sendObj(userPayload);
        //emit addUser(userPayload);

        m_UserState = UserStates::InitPayloadRcvd;
    }
}

void UserGO::rcvUser(QJsonObject user)
{
    if(m_UserState == UserStates::InitPayloadRcvd){
        emit initPayload(m_InitPayload);
        m_UserState = UserStates::FirstUserRcvd;
        emit sendUserNameToInfo(user["UserName"].toString());
        emit sendUserId(QUuid(user["ObjUuid"].toString()));
        emit initUserRcvd();
        if(user.value("ObjVersion").toInt() == 1)
            emit sendUserName(QUuid(user.value(QString("ObjUuid")).toString()),user.value(QString("UserName")).toString());
    }
}
