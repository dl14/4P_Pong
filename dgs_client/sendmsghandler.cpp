#include "sendmsghandler.h"

SendMsgHandler::SendMsgHandler(QObject *parent) : QObject(parent)
{
    m_HandlerStates = States::Init;
}

SendMsgHandler::~SendMsgHandler()
{
    //qDebug() << "MsgHandler deleted";
}

void SendMsgHandler::sendPayload(QJsonObject payload)
{
    if(m_HandlerStates == States::Init){
        QJsonObject message;
        m_MsgId = QUuid().createUuid();
        QString x = m_MsgId.toString();
        message["MsgId"] = m_MsgId.toString();
        m_ObjId = QUuid(payload["ObjUuid"].toString());
        message["MsgPayload"] = payload;
        emit sendObj(message);
        m_HandlerStates = States::MsgSent;
    }
}

void SendMsgHandler::rcvPayload(QJsonObject payload)
{
    if(m_HandlerStates == States::MsgSent){
        if(payload["ObjType"].toString() == "ResponsePayload"){
            if(payload["ResponseToMsgId"].toString() == m_MsgId.toString()){
                if(payload["Result"].toString() == "Rejected"){
                    QString reason = payload["ErrorMessage"].toString();
                    m_HandlerStates = States::Rejected;
                    emit responseRejected(payload);
                    this->deleteLater();
                }
                else if(payload["Result"].toString() == "Processed"){
                    m_HandlerStates = States::Waiting;
                }
            }
        }
    }
    else if(m_HandlerStates == States::Waiting){
        if(payload["ObjType"].toString() != "ResponsePayload"){
            if(payload["ObjUuid"].toString() == m_ObjId.toString()){
                m_HandlerStates = States::MsgComplete;
                emit completePayload(payload);
                this->deleteLater();
            }
        }
    }
}
