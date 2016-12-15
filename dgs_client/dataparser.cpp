#include "dataparser.h"
//#include "globalvalues.h"

#include <QJsonDocument>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <QtMath>

DataParser::DataParser(QTcpSocket *socket, QObject *parent) : QObject(parent),
    m_Socket(socket), initDataSent(false)
{

}

DataParser::~DataParser()
{

}

void DataParser::readByteArray()
{
    while(1){
        m_Socket->startTransaction();
        QByteArray data = m_Socket->readAll();
        //emit sendToLogger(data);
        if(!checkJSON(data)){
            return;
        }
    }
}

bool DataParser::checkJSON(QByteArray data)
{
    QJsonParseError pError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &pError);
    if(pError.error != QJsonParseError::NoError){
        if((pError.error == QJsonParseError::DeepNesting) || (pError.error == QJsonParseError::DocumentTooLarge)){
            //send the JSON parse error
            //emit rejectObject(ShapeGlobals::Rule0_2(pError.errorString()), "");
            return false;
        }
        int offset = pError.offset;
        //int x = data.size();
        if(pError.error == QJsonParseError::UnterminatedString){
            offset -= 1;
        }
        //in the middle of a message
        if(offset == data.size()){
            m_Socket->rollbackTransaction();
            return false;
        }
        //read too many objects
        else{
            //read the first and continue
            m_Socket->rollbackTransaction();
            data = m_Socket->read(offset);
            doc = QJsonDocument::fromJson(data, &pError);
        }
    }
    else if(pError.error == QJsonParseError::NoError){
        m_Socket->commitTransaction();
    }
    if(!doc.isObject()){
        //emit rejectObject(ShapeGlobals::Rule0_1(), "");
        return true;
    }
    QJsonObject jsonObj = doc.object();
    if(!jsonObj.contains("MsgId")){
        //emit rejectObject(ShapeGlobals::Rule0_1(), "");
        return true;
    }
    QJsonValue item = jsonObj.value("MsgId");
    if(!item.isString()){
        //emit rejectObject(ShapeGlobals::Rule0_1(), "");
        return true;
    }
    else
    {
        QString msgId = item.toString();
        QUuid id(msgId);
        if(id.isNull()){
            //emit rejectObject(ShapeGlobals::Rule0_1(), "");
            return true;
        }
    }
    QString msgId = item.toString();
    if(!jsonObj.contains("MsgPayload")){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    //MESSAGE PAYLOAD
    item = jsonObj.value("MsgPayload");
    if(!item.isObject()){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    jsonObj = item.toObject();
    if(!jsonObj.contains("ObjType")){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    item = jsonObj.value("ObjType");
    if(!item.isString()){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    if(!jsonObj.contains("ObjUuid")){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    item = jsonObj.value("ObjUuid");
    if(!item.isString()){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    else{
        QString objId = item.toString();
        QUuid id(objId);
        if(id.isNull()){
            //emit rejectObject(ShapeGlobals::Rule0_1(), "");
            return true;
        }
    }
    if(!jsonObj.contains("ObjOwnerUuid")){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    item = jsonObj.value("ObjOwnerUuid");
    if(!item.isString()){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    else{
        QString objId = item.toString();
        QUuid id(objId);
        if(id.isNull()){
            //emit rejectObject(ShapeGlobals::Rule0_1(), "");
            return true;
        }
    }
    if(!jsonObj.contains("ObjVersion")){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    item = jsonObj.value("ObjVersion");
    if(item.toDouble() != floor(item.toDouble())){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    if(item.toInt() < 0){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }
    if(!isProperPayload(jsonObj)){
        //emit rejectObject(ShapeGlobals::Rule0_1(), msgId);
        return true;
    }


    QJsonDocument document(jsonObj);
    emit sendToLogger(document.toJson());

    if(jsonObj.value(QString("ObjType")).toString() == QString("ArrayPayload")){
        if(!initDataSent){
            emit initArrayRcvd(jsonObj);
            initDataSent = true;
            return true;
        }
    }

    emit sendToHandler(jsonObj);

    if(jsonObj.value(QString("ObjType")).toString() == QString("GamePayload")){
        emit gameObjectRcvd(jsonObj);
        return true;
    }

    if(jsonObj.value(QString("ObjType")).toString() == QString("UserPayload")){
        emit userRcvd(jsonObj);
        emit showUserName(QUuid(jsonObj.value(QString("ObjUuid")).toString()), jsonObj.value(QString("UserName")).toString());
        return true;
    }
    if(jsonObj.value(QString("ObjType")).toString() == QString("ShapePayload")){
        emit shapeRcvd(jsonObj);
        return true;
    }
    if(jsonObj.value(QString("ObjType")).toString() == "ResponsePayload"){
        if(jsonObj.value(QString("ErrorMessage")).toString() != ""){
            emit rejectionRcvd(jsonObj);
            return true;
        }
    }
    return true;
}

bool DataParser::isProperPayload(QJsonObject payload)
{
    if(payload.value(QString("ObjType")).toString() == QString("UserPayload")){
        if(!payload.contains(QString("UserName")) || !payload.contains(QString("UserAuthorizationLevel")))
            return false;
        if(!payload.value(QString("UserName")).isString())
            return false;
        if(payload.value(QString("UserAuthorizationLevel")).toInt() < 0)
            return false;
    }
    else if(payload.value(QString("ObjType")).toString() == "FieldPayload"){
        if(!payload.contains(QString("FieldName")))
            return false;
        if(!payload.value(QString("FieldName")).isString())
            return false;
        if(!payload.contains(QString("FieldBoundary")))
            return false;
        if(!payload.value(QString("FieldBoundary")).isObject())
            return false;
        if(!payload.value(QString("FieldBoundary")).toObject().contains(QString("LowerRightX")) ||
           !payload.value(QString("FieldBoundary")).toObject().contains(QString("LowerRightY")) ||
           !payload.value(QString("FieldBoundary")).toObject().contains(QString("UpperLeftX"))  ||
           !payload.value(QString("FieldBoundary")).toObject().contains(QString("UpperLeftY")))
        {
            return false;
        }
        if(!payload.value(QString("FieldBoundary")).toObject().value(QString("LowerRightX")).isDouble() ||
           !payload.value(QString("FieldBoundary")).toObject().value(QString("LowerRightY")).isDouble() ||
           !payload.value(QString("FieldBoundary")).toObject().value(QString("UpperLeftX")).isDouble()  ||
           !payload.value(QString("FieldBoundary")).toObject().value(QString("UpperLeftY")).isDouble())
        {
            return false;
        }
    }
    else if(payload.value(QString("ObjType")).toString() == "ShapePayload"){
        if(!payload.contains(QString("ShapeType")))
            return false;
        if(!payload.value(QString("ShapeType")).isString())
            return false;
        if(!payload.contains(QString("ShapeBoundingBox")))
            return false;
        if(!payload.value(QString("ShapeBoundingBox")).isObject())
            return false;
        if(!payload.value(QString("ShapeBoundingBox")).toObject().contains(QString("LowerRightX")) ||
           !payload.value(QString("ShapeBoundingBox")).toObject().contains(QString("LowerRightY")) ||
           !payload.value(QString("ShapeBoundingBox")).toObject().contains(QString("UpperLeftX"))  ||
           !payload.value(QString("ShapeBoundingBox")).toObject().contains(QString("UpperLeftY")))
        {
            return false;
        }
        if(!payload.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).isDouble() ||
           !payload.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).isDouble() ||
           !payload.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).isDouble()  ||
           !payload.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).isDouble())
        {
            return false;
        }
    }
    return true;
}
