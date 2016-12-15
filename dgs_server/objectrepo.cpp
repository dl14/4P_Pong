#include "objectrepo.h"
#include "globalvalues.h"

#include <QUuid>
#include <QDebug>
ObjectRepo::ObjectRepo(QObject *parent) : QObject(parent)
{
    QJsonObject root;
    QUuid uuid;
    m_RootID = uuid.createUuid();
    qDebug() << "Root ID: " << m_RootID;
    root["ObjOwnerUuid"] = m_RootID.toString();
    root["ObjType"] = QString("UserPayload");
    root["ObjUuid"] = m_RootID.toString();
    root["ObjVersion"] = int(1);
    root["UserAuthorizationLevel"] = ShapeGlobals::AllPermissions();
    root["UserName"] = QString("root");

    QJsonObject GMObj;
    GMObj["ObjOwnerUuid"] = m_RootID.toString();
    GMObj["ObjType"] = QString("UserPayload");
    m_GMID = uuid.createUuid();
    GMObj["ObjUuid"] = m_GMID.toString();
    GMObj["ObjVersion"] = int(1);
    GMObj["UserAuthorizationLevel"] = ShapeGlobals::GMPermissions();
    GMObj["UserName"] = "GameMaster";

    QJsonObject fieldObj;
    QJsonObject field;
    /*field["LowerRightX"] = double(300);
    field["LowerRightY"] = double(300);
    field["UpperLeftX"] = double(-300);
    field["UpperLeftY"] = double(-300);
    m_Field = QRectF(QPointF(double(-300), double(-300)), QPointF(double(300), double(300)));*/

    field["LowerRightX"] = double(0);
    field["LowerRightY"] = double(1000);
    field["UpperLeftX"] = double(-500);
    field["UpperLeftY"] = double(500);
    m_Field = QRectF(QPointF(double(-500), double(500)), QPointF(double(0), double(1000)));

    fieldObj["FieldBoundary"] = field;
    fieldObj["FieldName"] = QString("MyField");
    fieldObj["ObjOwnerUuid"] = m_GMID.toString();
    fieldObj["ObjType"] = QString("FieldPayload");
    QUuid fieldID = uuid.createUuid();
    fieldObj["ObjUuid"] = fieldID.toString();
    fieldObj["ObjVersion"] = int(1);

    m_Map.insert(m_RootID, root);
    m_Map.insert(m_GMID, GMObj);
    m_Map.insert(fieldID, fieldObj);
}

void ObjectRepo::initData()
{
    QJsonArray data;
    QMap<QUuid, QJsonObject>::iterator it = m_Map.begin();
    while(it != m_Map.end()){
        data.append(it.value());
        it++;
    }
    emit sendInit(data, m_RootID);
}

void ObjectRepo::insertObj(QJsonObject obj, QUuid msgId)
{
    if(m_Map.contains(obj.value(QString("ObjUuid")).toString())){
        emit invalidRequest(ShapeGlobals::Rule1_1(), msgId);
        return;
    }
    else{
        if(obj.value("ObjType").toString() == "FieldPayload"){
            //PERMISSION CHECK
            if(!isGM(obj.value("ObjOwnerUuid").toString()) && !isRoot(obj.value("ObjOwnerUuid").toString())){
                emit invalidRequest(ShapeGlobals::Rule2_FieldPayload(), msgId);
                return;
            }
            if(isDuplicateName(QString("ObjType"), QString("FieldPayload"))){
                emit invalidRequest(ShapeGlobals::Rule1_7(), msgId);
                return;
            }
            if(!isUserInRepo(obj.value(QString("ObjOwnerUuid")).toString())){
                emit invalidRequest(ShapeGlobals::Rule1_8(), msgId);
                return;
            }
        }
        else if(obj.value("ObjType").toString() == QString("UserPayload")){
            //PERMISSION CHECK
            /*if((!isGM(obj.value("ObjOwnerUuid").toString())) && (!isRoot(obj.value("ObjOwnerUuid").toString()))){
                emit invalidRequest(ShapeGlobals::Rule2_UserPayload(), msgId);
                return;
            }*/
            if(!isUserInRepo(obj.value(QString("ObjOwnerUuid")).toString())){
                emit invalidRequest(ShapeGlobals::Rule1_8(), msgId);
                return;
            }
            if(isDuplicateName(QString("UserName"), obj.value("UserName").toString())){
                emit invalidRequest(ShapeGlobals::Rule1_4(), msgId);
                return;
            }
            else{
                //qDebug() << "User ID: " << obj.value(QString("ObjUuid")).toString();
                obj["ObjOwnerUuid"] = m_RootID.toString();
                obj["UserAuthorizationLevel"] = ShapeGlobals::DefaultUserPermissions();
                emit firstUserAdded(msgId);
            }
        }
        else if(obj.value("ObjType").toString() == "ShapePayload"){
            if(!isUserInRepo(obj.value(QString("ObjOwnerUuid")).toString())){
                emit invalidRequest(ShapeGlobals::Rule1_8(), msgId);
                return;
            }
            if(!isWithinField(obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).toDouble(),
                          obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).toDouble(),
                          obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).toDouble(),
                          obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).toDouble()))
            {
                emit invalidRequest(ShapeGlobals::Rule3_1_1(), msgId);
                return;
            }
            if(isIntersection(obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).toDouble(),
                              obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).toDouble(),
                              obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).toDouble(),
                              obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).toDouble(),
                              QUuid(obj.value(QString("ObjUuid")).toString())))
            {
                emit invalidRequest(ShapeGlobals::Rule3_2_1(), msgId);
                return;
            }
        }
    }
    emit validRequest(QString(""), msgId);
    m_Map.insert(QUuid(obj.value(QString("ObjUuid")).toString()), obj);
    emit messageProcessed(obj);
}

void ObjectRepo::removeObj(QJsonObject obj, QUuid msgId)
{
    if(!m_Map.contains(obj.value(QString("ObjUuid")).toString())){
        emit invalidRequest(ShapeGlobals::Rule1_2(), msgId);
        return;
    }
    if((obj.value(QString("ObjType")).toString() == "FieldPayload") && !isGM(obj.value(QString("ObjUuid")).toString()))
    {
        emit invalidRequest(ShapeGlobals::Rule2_FieldPayload(), msgId);
        return;
    }
    if(!isUserInRepo(obj.value(QString("ObjOwnerUuid")).toString())){
        emit invalidRequest(ShapeGlobals::Rule1_8(), msgId);
        return;
    }
    emit validRequest(QString(""), msgId);
    m_Map.remove(obj.value(QString("ObjUuid")).toString());
    emit messageProcessed(obj);
}

void ObjectRepo::changeObj(QJsonObject obj, QUuid msgId)
{
    if(!m_Map.contains(obj.value("ObjUuid").toString())){
        emit invalidRequest(ShapeGlobals::Rule1_2(), msgId);
        return;
    }
    if(!isUserInRepo(obj.value(QString("ObjOwnerUuid")).toString())){
        emit invalidRequest(ShapeGlobals::Rule1_8(), msgId);
        return;
    }
    else{
        int check = m_Map.value(obj.value("ObjUuid").toString()).value("ObjVersion").toInt() + 1;
        if(obj.value(QString("ObjVersion")).toInt() != check)
        {
            emit invalidRequest(ShapeGlobals::Rule1_3(), msgId);
            return;
        }
        if(obj.value(QString("ObjType")).toString() == "FieldPayload"){
            if(isWithinField(obj.value(QString("FieldBoundary")).toObject().value(QString("UpperLeftX")).toDouble(),
                          obj.value(QString("FieldBoundary")).toObject().value(QString("UpperLeftY")).toDouble(),
                          obj.value(QString("FieldBoundary")).toObject().value(QString("LowerRightX")).toDouble(),
                          obj.value(QString("FieldBoundary")).toObject().value(QString("LowerRightY")).toDouble()))
            {
                emit invalidRequest(ShapeGlobals::Rule1_6(), msgId);
                return;
            }
            else{
                QRectF tempRect(QPointF(obj.value(QString("FieldBoundary")).toObject().value(QString("UpperLeftX")).toDouble(),
                                        obj.value(QString("FieldBoundary")).toObject().value(QString("UpperLeftY")).toDouble()),
                                QPointF(obj.value(QString("FieldBoundary")).toObject().value(QString("LowerRightX")).toDouble(),
                                        obj.value(QString("FieldBoundary")).toObject().value(QString("LowerRightY")).toDouble()));
                emit validRequest(QString(""), msgId);
                m_Field = tempRect;
                m_Map.insert(QUuid(obj.value(QString("ObjUuid")).toString()), obj);
                emit messageProcessed(obj);
                return;
            }

        }
        if(obj.value(QString("ObjType")).toString() == "ShapePayload"){
            if(!isWithinField(obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).toDouble(),
                              obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).toDouble(),
                              obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).toDouble(),
                              obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).toDouble()))
            {
                emit invalidRequest(ShapeGlobals::Rule3_1_2(), msgId);
                return;
            }
            if(obj.value(QString("ShapeType")).toString() == "Rectangle"){
                if(isIntersection(obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).toDouble(),
                                  obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).toDouble(),
                                  obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).toDouble(),
                                  obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).toDouble(),
                                  QUuid(obj.value(QString("ObjUuid")).toString())))
                {
                    emit invalidRequest(ShapeGlobals::Rule3_2_2_Rectangle(), msgId);
                    return;
                }
                else{
                    emit validRequest(QString(""), msgId);
                    m_Map.insert(QUuid(obj.value(QString("ObjUuid")).toString()), obj);
                    emit messageProcessed(obj);
                    return;
                }
            }
            else if(obj.value(QString("ShapeType")).toString() == "Destroyer"){
                QRectF checkRect(QPointF(obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).toDouble(),
                                         obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).toDouble()),
                                 QPointF(obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).toDouble(),
                                         obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).toDouble()));
                QMap<QUuid, QJsonObject>::iterator it = m_Map.begin();
                while(it != m_Map.end()){
                    bool increment = true;
                    if((it.value().value(QString("ObjType")).toString() == "ShapePayload") &&
                       (it.value().value(QString("ObjUuid")) != obj.value(QString("ObjUuid")))){
                        //emit validRequest(QString(""), msgId);
                        QRectF tempRect(QPointF(it.value().value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).toDouble(),
                                                it.value().value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).toDouble()),
                                        QPointF(it.value().value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).toDouble(),
                                                it.value().value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).toDouble()));
                        if(checkRect.intersects(tempRect)){
                            QJsonObject destr = it.value();
                            destr["ObjVersion"] = int(0);
                            emit messageProcessed(destr);
                            it = m_Map.erase(it);
                            increment = false;
                        }
                    }
                    if(increment)
                        ++it;
                }
                emit validRequest(QString(""), msgId);
                m_Map.insert(QUuid(obj.value(QString("ObjUuid")).toString()), obj);
                emit messageProcessed(obj);
                return;
            }
            else{
                if(isIntersection(obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).toDouble(),
                                  obj.value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).toDouble(),
                                  obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).toDouble(),
                                  obj.value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).toDouble(),
                                  QUuid(obj.value(QString("ObjUuid")).toString())))
                {
                    emit invalidRequest(ShapeGlobals::Rule3_2_2_Default(), msgId);
                    return;
                }
                else{
                    emit validRequest(QString(""), msgId);
                    m_Map.insert(QUuid(obj.value(QString("ObjUuid")).toString()), obj);
                    emit messageProcessed(obj);
                    return;
                }
            }
        }
        if(obj.value(QString("ObjType")).toString() == "UserPayload")
        {
            if(isDuplicateNameChange(QString("UserName"), obj.value("UserName").toString(), QUuid(obj.value("ObjUuid").toString()))){
                emit invalidRequest(ShapeGlobals::Rule1_4(), msgId);
                return;
            }
            else{
                emit firstUserAdded(msgId);
                obj["ObjOwnerUuid"] = m_RootID.toString();
                if(isGM(QUuid(obj.value(QString("ObjUuid")).toString())) && (obj.value(QString("UserName")).toString() == QString("GameMaster")))
                    obj["UserAuthorizationLevel"] = ShapeGlobals::GMPermissions();
                else
                    obj["UserAuthorizationLevel"] = ShapeGlobals::DefaultUserPermissions();
                emit validRequest(QString(""), msgId);
                m_Map.insert(QUuid(obj.value(QString("ObjUuid")).toString()), obj);
                emit messageProcessed(obj);
                return;
            }
        }
        emit validRequest(QString(""), msgId);
        m_Map.insert(QUuid(obj.value(QString("ObjUuid")).toString()), obj);
        emit messageProcessed(obj);
        return;
    }
}

//PRIVATE FUNCTIONS

//fieldCheck is the field of JSonObject that needs to be checked
//checkName is the name that needs to be checked against
bool ObjectRepo::isDuplicateName(QString fieldCheck, QString checkName)
{
    QMap<QUuid, QJsonObject>::iterator it = m_Map.begin();
    while(it != m_Map.end()){
        if(it.value().value(fieldCheck).toString() == checkName){
            return true;
        }
        it++;
    }
    return false;
}

bool ObjectRepo::isDuplicateNameChange(QString fieldCheck, QString checkName, QUuid objId)
{
    QMap<QUuid, QJsonObject>::iterator it = m_Map.begin();
    while(it != m_Map.end()){
        if((it.value().value(fieldCheck).toString() == checkName) && (it.key() != objId)){
            return true;
        }
        it++;
    }
    return false;
}

bool ObjectRepo::isWithinField(double topLeftX, double topLeftY, double bottomRightX, double bottomRightY)
{
    QRectF tempRect(QPointF(topLeftX, topLeftY), QPointF(bottomRightX, bottomRightY));
    return m_Field.contains(tempRect);
}

bool ObjectRepo::isIntersection(double topLeftX, double topLeftY, double bottomRightX, double bottomRightY, QUuid ObjId)
{
    QRectF checkRect(QPointF(topLeftX, topLeftY), QPointF(bottomRightX, bottomRightY));
    QMap<QUuid, QJsonObject>::iterator it = m_Map.begin();
    while(it != m_Map.end()){
        if((it.value().value(QString("ObjType")).toString() == "ShapePayload") && (it.key() != ObjId)){
            QRectF tempRect(QPointF(it.value().value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftX")).toDouble(),
                                    it.value().value(QString("ShapeBoundingBox")).toObject().value(QString("UpperLeftY")).toDouble()),
                            QPointF(it.value().value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightX")).toDouble(),
                                    it.value().value(QString("ShapeBoundingBox")).toObject().value(QString("LowerRightY")).toDouble()));
            if(checkRect.intersects(tempRect))
                return true;
        }
        it++;
    }
    return false;
}

bool ObjectRepo::isRoot(QUuid objID)
{
    if(objID == m_RootID)
        return true;
    return false;
}

bool ObjectRepo::isGM(QUuid objID)
{
    if(objID == m_GMID)
        return true;
    return false;
}

bool ObjectRepo::isUserInRepo(QString objID)
{
    QMap<QUuid, QJsonObject>::iterator it = m_Map.begin();
    while(it != m_Map.end()){
        if((QUuid(it.value().value("ObjUuid").toString()) == objID) &&
           (it.value().value("ObjType")).toString() == QString("UserPayload"))
            return true;
        it++;
    }
    return false;
}
