#include "datasender.h"
#include "globalvalues.h"

#include <QFile>
QString createId();

DataSender::DataSender(QTcpSocket *socket, QObject *parent) : QObject(parent),
    m_Socket(socket), initComplete(false)
{

}

void DataSender::sendInitData(QJsonArray initArray, QString rootId)
{
    if(!initComplete){
        m_RootID = rootId;
        QJsonObject initArrayResp;
        initArrayResp["MsgId"] = createId();
        QJsonObject payload;
        payload["InitArray"] = initArray;
        payload["ObjOwnerUuid"] = m_RootID;
        payload["ObjTpye"] = QString("ArrayPayload");
        payload["ObjUuid"] = createId();
        payload["ObjVersion"] = int(1);
        initArrayResp["MsgPayload"] = payload;
        initComplete = true;

        QJsonDocument doc(initArrayResp);
        m_Socket->write(doc.toJson());
    }
}

void DataSender::sendResponse(QString errorMsg, QString msgId)
{
    if(initComplete){
        QJsonObject response;
        response["MsgId"] = createId();
        QJsonObject payload;
        payload["ErrorMessage"] = errorMsg;
        payload["ObjOwnerUuid"] = m_RootID;
        payload["ObjType"] = QString("ResponsePayload");
        payload["ObjUuid"] = createId();
        payload["ObjVersion"] = qint32(1);
        payload["ResponseToMsgId"] = msgId;
        if(errorMsg == "")
            payload["Result"] = ShapeGlobals::MessageProcessed();
        else
            payload["Result"] = ShapeGlobals::MessageRejected();
        response["MsgPayload"] = payload;
        QJsonDocument responseDoc(response);
        m_Socket->write(responseDoc.toJson());
    }
}

void DataSender::sendServerChange(QJsonObject obj)
{
    if(initComplete){
        QJsonObject message;
        message["MsgId"] = createId();
        message["MsgPayload"] = obj;
        QJsonDocument messageDoc(message);
        m_Socket->write(messageDoc.toJson());
    }
}

void DataSender::disconnectHost()
{
    if(initComplete)
        m_Socket->disconnectFromHost();
}

QString createId()
{
    QUuid ID;
    return ID.createUuid().toString();
}
