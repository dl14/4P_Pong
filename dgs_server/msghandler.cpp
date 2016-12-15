#include "msghandler.h"

QString createIdString();

MsgHandler::MsgHandler(QTcpSocket *socket, QObject *parent) : QObject(parent),
    m_Socket(socket)
{
    m_State = States::Init;
}

void MsgHandler::checkPayload(QJsonObject obj, QUuid msgId)
{
    if(m_State == States::Init){
        m_Messages.append(msgId);
        sendResponse(ShapeGlobals::Rule4_2(), msgId.toString());
        m_State = States::Error;
        disconnectClient();
    }
    else if(m_State == States::InitArraySent){
        m_Messages.append(msgId);
        if(obj.value(QString("ObjType")).toString() == "UserPayload"){
            if(obj.value(QString("ObjVersion")).toInt() == 1)
                emit insertObject(obj, msgId);
            else if(obj.value(QString("ObjVersion")).toInt() > 1)
                emit changeObject(obj, msgId);
        }
        else{
            sendResponse(ShapeGlobals::Rule4_3(), msgId);
        }
    }
    else if(m_State != States::Error){
        m_Messages.append(msgId);
        if(obj.value(QString("ObjVersion")).toInt() == 1){
            emit insertObject(obj, msgId);
        }
        else if(obj.value(QString("ObjVersion")).toInt() == 0){
            emit removeObject(obj, msgId);
        }
        else{
            emit changeObject(obj, msgId);
        }
        /*else{
            sendResponse(ShapeGlobals::Rule0_1(), msgId);
            m_State = States::Error;
            disconnectClient();
        }*/
    }
}

void MsgHandler::sendInitData(QJsonArray initArray, QUuid rootId)
{
    if(m_State == States::Init){
        m_RootID = rootId;

        QJsonObject initArrayResp;
        initArrayResp["MsgId"] = createIdString();
        QJsonObject payload;
        payload["InitArray"] = initArray;
        payload["ObjOwnerUuid"] = rootId.toString();
        payload["ObjType"] = QString("ArrayPayload");
        payload["ObjUuid"] = createIdString();
        payload["ObjVersion"] = int(1);
        initArrayResp["MsgPayload"] = payload;
        m_State = States::InitArraySent;

        QJsonDocument doc(initArrayResp);
        m_Socket->write(doc.toJson());

        emit sendToLogger(doc.toJson(), m_Socket->socketDescriptor());
    }
}



void MsgHandler::sendResponse(QString errorMsg, QUuid msgId)
{
    int index  = m_Messages.indexOf(msgId);
    if(index != -1){
        QJsonObject response;
        response["MsgId"] = createIdString();
        QJsonObject payload;
        payload["ErrorMessage"] = errorMsg;
        payload["ObjOwnerUuid"] = m_RootID.toString();
        payload["ObjType"] = QString("ResponsePayload");
        payload["ObjUuid"] = createIdString();
        payload["ObjVersion"] = qint32(1);
        payload["ResponseToMsgId"] = msgId.toString();
        if(errorMsg == "")
            payload["Result"] = ShapeGlobals::MessageProcessed();
        else
            payload["Result"] = ShapeGlobals::MessageRejected();
        response["MsgPayload"] = payload;
        QJsonDocument responseDoc(response);
        m_Socket->write(responseDoc.toJson());
        m_Messages.removeAt(index);

        emit sendToLogger(responseDoc.toJson(), m_Socket->socketDescriptor());
    }
    if(m_State == States::Error || m_State == States::Init || m_State == States::InitArraySent){
        disconnectClient();
        //qDebug() << "Disconnecting a client";
    }
}

void MsgHandler::sendBadResponse(QString errorMsg, QString msgId)
{
    QUuid msg(msgId);
    m_Messages.append(msg);
    m_State = States::Error;
    sendResponse(errorMsg, msg);
}

void MsgHandler::sendRepoChange(QJsonObject obj)
{
    QJsonObject message;
    message["MsgId"] = createIdString();
    message["MsgPayload"] = obj;
    QJsonDocument messageDoc(message);
    m_Socket->write(messageDoc.toJson());

    emit sendToLogger(messageDoc.toJson(), m_Socket->socketDescriptor());
}

void MsgHandler::initUserAdded(QUuid msgId)
{
    if(m_Messages.contains(msgId) && (m_State == States::InitArraySent))
        m_State = States::UserPayloadRecvd;
}

void MsgHandler::disconnectClient()
{
    emit sendToLogger(QString("Disconnecting from client").toUtf8(), m_Socket->socketDescriptor());
    m_Socket->disconnectFromHost();
}

QString createIdString()
{
    QUuid id;
    return id.createUuid().toString();
}
