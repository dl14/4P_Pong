#include "shapego.h"
#include <QUuid>
#include <QJsonArray>
#include <QDebug>
#include <QtMath>
#include <math.h>

const qreal changeSpeed = 100;
const int msUpdateTime = 50;
const qreal msPerSec = 1000.0;

ShapeGO::ShapeGO(QObject *parent) : AbstractShapeGO(parent),
    m_ObjV(0),
    oldPos2(QPointF(0,0)),
    oldPos1(QPointF(0,0)),
    isStarted(false),
    XDir(1),
    YDir(1),
    fieldSet(false),
    m_Score0(0),
    m_Score1(0),
    m_Score2(0),
    m_Score3(0),
    m_NumOfUsers(-1)
{

}

void ShapeGO::setField(QJsonObject initObj)
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

void ShapeGO::startShape()
{
    QJsonObject shape;
    shape["ObjUuid"] = QUuid().createUuid().toString();
    //shape["ObjOwnerUuid"] = m_UserID.toString();
    shape["ObjType"] = QString("ShapePayload");
    shape["ObjVersion"] = 1;
    shape["ShapeType"] = QString("Rectangle");
    QJsonObject bb;
    //bb["LowerRightX"] = double(15);
    //bb["LowerRightY"] = double(15);
    //bb["UpperLeftX"] = double(-15);
    //bb["UpperLeftY"] = double(-15);
    double LRx = m_Field.center().x() + (m_Field.width()/40);
    double LRy = m_Field.center().y() + (m_Field.height()/40);
    double ULx = m_Field.center().x() - (m_Field.width()/40);
    double ULy = m_Field.center().y() - (m_Field.height()/40);

    m_Ball = QRectF(QPointF(ULx, ULy), QPointF(LRx, LRy));
    bb["LowerRightX"] = m_Field.center().x() + (m_Field.width()/40);
    bb["LowerRightY"] = m_Field.center().y() + (m_Field.height()/40);
    bb["UpperLeftX"] = m_Field.center().x() - (m_Field.width()/40);
    bb["UpperLeftY"] = m_Field.center().y() - (m_Field.height()/40);
    shape["ShapeBoundingBox"] = bb;

    m_Object = shape;

    SendMsgHandler *handler = new SendMsgHandler;
    connect(this, &ShapeGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
    connect(this, &ShapeGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
    connect(handler, &SendMsgHandler::sendObj, this, &ShapeGO::addShape);
    connect(handler, &SendMsgHandler::completePayload, this, &ShapeGO::moveShape);
    //emit addShape(shape);
    emit sendObjToHandler(shape);
}

void ShapeGO::shapeRejected(QJsonObject response)
{
    if(response["ErrorMessage"].toString() == ShapeGlobals::Rule3_1_2()){
        double LRx = m_Object["ShapeBoundingBox"].toObject().value(QString("LowerRightX")).toDouble();
        double ULx = m_Object["ShapeBoundingBox"].toObject().value(QString("UpperLeftX")).toDouble();
        double LRy = m_Object["ShapeBoundingBox"].toObject().value(QString("LowerRightY")).toDouble();
        double ULy = m_Object["ShapeBoundingBox"].toObject().value(QString("UpperLeftY")).toDouble();

        QPointF rectCenter = QRectF(QPointF(ULx, ULy), QPointF(LRx, LRy)).center();
        QPointF difference = rectCenter - m_Field.center();
        //double angle = fmod(qRadiansToDegrees(atan2(difference.y(), difference.x())), 90);
        double angle = qRadiansToDegrees(atan2(difference.y(), difference.x()));
        qDebug() << angle;
        if((angle >= 44 && angle <= 46) || (angle >= -46 && angle <= -44) || (angle >= 134 && angle <= 136) || (angle >= -136 && angle <= -134)){
            XDir *= -1;
            YDir *= -1;
        }
        else if((angle >= 0 && angle < 44) || (angle < 0 && angle > -44) || (angle > 136 && angle <= 180) || (angle >= -180 && angle < -136))
            XDir *= -1;
        else if((angle > 46 && angle < 134) || (angle > -134 && angle < -46))
            YDir *= -1;

        double angle2 = qRadiansToDegrees(atan2(difference.y(), difference.x()));
        if((angle2 >= 0 && angle <= 45) || (angle2 >= -45 && angle2 < 0)){
            m_Score3++;
            emit updateScore(m_Score3, 3);
        }
        else if(angle2 >= 45 && angle2 <= 135){
            m_Score0++;
            emit updateScore(m_Score0, 0);
        }
        else if((angle2 >= 135 && angle2 <= 180) || (angle2 >= -180 && angle2 <= -135)){
            m_Score2++;
            emit updateScore(m_Score2, 2);
        }
        else if(angle2 >= -135 && angle2 <= -45){
            m_Score1++;
            emit updateScore(m_Score1, 1);
        }
    }
    if(response["ErrorMessage"].toString() == ShapeGlobals::Rule3_2_2_Rectangle()){
        double LRx = m_Object["ShapeBoundingBox"].toObject().value(QString("LowerRightX")).toDouble();
        double ULx = m_Object["ShapeBoundingBox"].toObject().value(QString("UpperLeftX")).toDouble();
        double LRy = m_Object["ShapeBoundingBox"].toObject().value(QString("LowerRightY")).toDouble();
        double ULy = m_Object["ShapeBoundingBox"].toObject().value(QString("UpperLeftY")).toDouble();

        QPointF rectCenter = QRectF(QPointF(ULx, ULy), QPointF(LRx, LRy)).center();
        QPointF difference = rectCenter - m_Field.center();
        double angle2 = qRadiansToDegrees(atan2(difference.y(), difference.x()));

        //first two for side of paddles, second two for top of paddle
        if((LRy >= m_Field.bottomRight().y() - 29) || (ULy <= m_Field.topLeft().y() + 29))
            XDir *= -1;
        else if((ULx <= m_Field.topLeft().x() + 29) || (LRx >= m_Field.bottomRight().x() - 29))
            YDir *= -1;
        else if((angle2 >= 45 && angle2 <= 135) || (angle2 >= -135 && angle2 <= -45))
            YDir *= -1;
        else if((angle2 >= 0 && angle2 < 45) || (angle2 > -45 && angle2 < 0) || (angle2 >= -180 && angle2 < -135) || (angle2 > 135 && angle2 <= 180))
            XDir *= -1;
    }
}

void ShapeGO::moveShape(QJsonObject shape)
{
    if(shape["ObjUuid"].toString() == m_Object["ObjUuid"].toString()){
        if(shape["ObjVersion"].toInt() == 1){
            m_Object = shape;
            emit shapeAdded();
        }
        else{
            m_Object = shape;
        }
    }
}

void ShapeGO::changePos()
{
    QPointF curPos1 = QPointF(m_Object["ShapeBoundingBox"].toObject().value(QString("LowerRightX")).toDouble(),
                              m_Object["ShapeBoundingBox"].toObject().value(QString("LowerRightY")).toDouble());
    QPointF curPos2 = QPointF(m_Object["ShapeBoundingBox"].toObject().value(QString("UpperLeftX")).toDouble(),
                              m_Object["ShapeBoundingBox"].toObject().value(QString("UpperLeftY")).toDouble());

    curPos1.setX(curPos1.x()+XDir*changeSpeed*(msUpdateTime/msPerSec));
    curPos2.setX(curPos2.x()+XDir*changeSpeed*(msUpdateTime/msPerSec));
    curPos1.setY(curPos1.y()+YDir*changeSpeed*(msUpdateTime/msPerSec));
    curPos2.setY(curPos2.y()+YDir*changeSpeed*(msUpdateTime/msPerSec));
    QJsonObject shape = updateShape(curPos1.x(), curPos1.y(), curPos2.x(), curPos2.y());

    SendMsgHandler *handler = new SendMsgHandler;
    connect(this, &ShapeGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
    connect(this, &ShapeGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
    connect(handler, &SendMsgHandler::sendObj, this, &ShapeGO::addShape);
    connect(handler, &SendMsgHandler::completePayload, this, &ShapeGO::moveShape);
    connect(handler, &SendMsgHandler::responseRejected, this, &ShapeGO::shapeRejected);

    emit sendObjToHandler(shape);
}

void ShapeGO::removeShape()
{
    QJsonObject shape = m_Object;
    shape["ObjVersion"] = 0;

    SendMsgHandler *handler = new SendMsgHandler;
    connect(this, &ShapeGO::rcvShapePayload, handler, &SendMsgHandler::rcvPayload);
    connect(this, &ShapeGO::sendObjToHandler, handler, &SendMsgHandler::sendPayload);
    connect(handler, &SendMsgHandler::sendObj, this, &ShapeGO::addShape);
    connect(handler, &SendMsgHandler::completePayload, this, &ShapeGO::moveShape);
    connect(handler, &SendMsgHandler::responseRejected, this, &ShapeGO::shapeRejected);

    emit sendObjToHandler(shape);
}
