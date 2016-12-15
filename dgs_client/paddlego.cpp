#include "paddlego.h"

#include <QDebug>

PaddleGO::PaddleGO(QObject *parent) : AbstractShapeGO(parent), fieldSet(false), m_NumOfUsers(-1)
{

}

void PaddleGO::setField(QJsonObject initObj)
{
    if(!fieldSet){
        QJsonArray initArray = initObj["InitArray"].toArray();
        for(int i = 0; i < initArray.size(); i++){
            QJsonObject obj = initArray[i].toObject();
            if(obj["ObjType"].toString() == "FieldPayload"){
                QJsonObject fieldObj = obj["FieldBoundary"].toObject();
                QRectF field(QPointF(fieldObj["UpperLeftX"].toDouble(), fieldObj["UpperLeftY"].toDouble()),
                             QPointF(fieldObj["LowerRightX"].toDouble(), fieldObj["LowerRightY"].toDouble()));
                m_Field = field;
                fieldSet = true;
                return;
            }
        }
    }
}

void PaddleGO::addPaddle(int playerNum)
{
    m_NumOfUsers = playerNum;

    QJsonObject shape;
    m_PaddleID = QUuid().createUuid();
    shape["ObjUuid"] = m_PaddleID.toString();
    //shape["ObjOwnerUuid"] = m_UserID.toString();
    shape["ObjType"] = QString("ShapePayload");
    shape["ObjVersion"] = 1;
    shape["ShapeType"] = QString("Rectangle");

    if(m_NumOfUsers == 0){
        QJsonObject bb;
        //bb["LowerRightX"] = double(50);
        //bb["LowerRightY"] = double(290);
        //bb["UpperLeftX"] = double(-50);
        //bb["UpperLeftY"] = double(270);

        bb["LowerRightX"] = m_Field.center().x() + (m_Field.width()/10);
        bb["LowerRightY"] = m_Field.bottomRight().y() - 10;
        bb["UpperLeftX"] = m_Field.center().x() - (m_Field.width()/10);
        bb["UpperLeftY"] =  m_Field.bottomRight().y() - 30;
        shape["ShapeBoundingBox"] = bb;
    }
    else if(m_NumOfUsers == 1){
        QJsonObject bb;
        //bb["LowerRightX"] = double(50);
        //bb["LowerRightY"] = double(-270);
        //bb["UpperLeftX"] = double(-50);
        //bb["UpperLeftY"] = double(-290);

        bb["LowerRightX"] = m_Field.center().x() + (m_Field.width()/10);
        bb["LowerRightY"] = m_Field.topLeft().y() + 30;
        bb["UpperLeftX"] = m_Field.center().x() - (m_Field.width()/10);
        bb["UpperLeftY"] = m_Field.topLeft().y() + 10;
        shape["ShapeBoundingBox"] = bb;
    }
    else if(m_NumOfUsers == 2){
        QJsonObject bb;
        //bb["LowerRightX"] = double(-270);
        //bb["LowerRightY"] = double(50);
        //bb["UpperLeftX"] = double(-290);
        //bb["UpperLeftY"] = double(-50);

        bb["LowerRightX"] = m_Field.topLeft().x() + 30;
        bb["LowerRightY"] = m_Field.center().y() + (m_Field.height()/10);
        bb["UpperLeftX"] = m_Field.topLeft().x() + 10;
        bb["UpperLeftY"] = m_Field.center().y() - (m_Field.height()/10);
        shape["ShapeBoundingBox"] = bb;
    }
    else if(m_NumOfUsers == 3){
        QJsonObject bb;
        //bb["LowerRightX"] = double(290);
        //bb["LowerRightY"] = double(50);
        //bb["UpperLeftX"] = double(270);
        //bb["UpperLeftY"] = double(-50);

        bb["LowerRightX"] = m_Field.bottomRight().x() - 10;
        bb["LowerRightY"] = m_Field.center().y() + (m_Field.height()/10);
        bb["UpperLeftX"] = m_Field.bottomRight().x() - 30;
        bb["UpperLeftY"] = m_Field.center().y() - (m_Field.height()/10);
        shape["ShapeBoundingBox"] = bb;
    }

    m_Object = shape;

    SendMsgHandler *handler = new SendMsgHandler;
    connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
    connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
    connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
    connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);

    emit sendObjToHandler(shape);
}

void PaddleGO::moveShape(QJsonObject shape)
{
    if(shape["ObjUuid"].toString() == m_Object["ObjUuid"].toString()){
        m_Object = shape;
    }
}

void PaddleGO::moveLeft()
{
    if(m_NumOfUsers == 0){
        //qDebug() << "Request Move Left";
        QJsonObject shape = updateShape(m_Object["ShapeBoundingBox"].toObject().value("LowerRightX").toDouble() - 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("LowerRightY").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftX").toDouble() - 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftY").toDouble());
        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
        connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
        connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);
        emit sendObjToHandler(shape);
        qDebug() << "Move Left (0) : " << m_Object["ObjUuid"];
    }
    else if(m_NumOfUsers == 1){
        //qDebug() << "Request Move Left";
        QJsonObject shape = updateShape(m_Object["ShapeBoundingBox"].toObject().value("LowerRightX").toDouble() - 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("LowerRightY").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftX").toDouble() - 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftY").toDouble());
        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
        connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
        connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);
        emit sendObjToHandler(shape);

        qDebug() << "Move Left (1) : " << m_Object["ObjUuid"];
    }
    else if(m_NumOfUsers == 2){
        QJsonObject shape = updateShape(m_Object["ShapeBoundingBox"].toObject().value("LowerRightX").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("LowerRightY").toDouble() - 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftX").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftY").toDouble() - 15);
        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
        connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
        connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);
        emit sendObjToHandler(shape);
    }
    else if(m_NumOfUsers == 3){
        QJsonObject shape = updateShape(m_Object["ShapeBoundingBox"].toObject().value("LowerRightX").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("LowerRightY").toDouble() + 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftX").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftY").toDouble() + 15);
        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
        connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
        connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);
        emit sendObjToHandler(shape);
    }
}

void PaddleGO::moveRight()
{
    if(m_NumOfUsers == 0 || m_NumOfUsers == 1){
        //qDebug() << "Request Move Right";
        QJsonObject shape = updateShape(m_Object["ShapeBoundingBox"].toObject().value("LowerRightX").toDouble() + 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("LowerRightY").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftX").toDouble() + 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftY").toDouble());
        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
        connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
        connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);
        emit sendObjToHandler(shape);
    }
    else if(m_NumOfUsers == 2){
        QJsonObject shape = updateShape(m_Object["ShapeBoundingBox"].toObject().value("LowerRightX").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("LowerRightY").toDouble() + 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftX").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftY").toDouble() + 15);
        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
        connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
        connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);
        emit sendObjToHandler(shape);
    }
    else if(m_NumOfUsers == 3){
        QJsonObject shape = updateShape(m_Object["ShapeBoundingBox"].toObject().value("LowerRightX").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("LowerRightY").toDouble() - 15,
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftX").toDouble(),
                                        m_Object["ShapeBoundingBox"].toObject().value("UpperLeftY").toDouble() - 15);
        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
        connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
        connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);
        emit sendObjToHandler(shape);
    }
}

void PaddleGO::deletePaddle()
{
    QJsonObject shape = m_Object;
    shape["ObjVersion"] = 0;
    SendMsgHandler *handler = new SendMsgHandler;
    connect(this, &PaddleGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
    connect(this, &PaddleGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
    connect(handler, &SendMsgHandler::sendObj, this, &PaddleGO::addShape);
    connect(handler, &SendMsgHandler::completePayload, this, &PaddleGO::moveShape);
    emit sendObjToHandler(shape);
}
