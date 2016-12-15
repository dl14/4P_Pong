#include "datasender.h"
#include <QUuid>
DataSender::DataSender(QTcpSocket *tcpSocket, QObject *parent) : QObject(parent), m_TcpSocket(tcpSocket)
{

}

DataSender::~DataSender()
{

}

void DataSender::sendObj(QJsonObject obj)
{
    QJsonObject message;
    message["MsgId"] = QUuid().createUuid().toString();
    if(obj["ObjType"].toString() == "ShapePayload")
        obj["ObjOwnerUuid"] = m_UserID.toString();
    message["MsgPayload"] = obj;
    QJsonDocument doc(message);
    //m_TcpSocket->write(doc.toJson());

}

void DataSender::sendObject(QJsonObject obj)
{
    QJsonObject msg = obj["MsgPayload"].toObject();
    if(msg.value("ObjType").toString() == "ShapePayload"){
        msg["ObjOwnerUuid"] = m_UserID.toString();
        obj["MsgPayload"] = msg;
    }
    QJsonDocument doc(obj);
    m_TcpSocket->write(doc.toJson());
    emit sendingData(doc.toJson());
}
