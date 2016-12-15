#ifndef SHAPEGO_H
#define SHAPEGO_H

#include "abstractshapego.h"
#include "globalvalues.h"
#include "sendmsghandler.h"

#include <QObject>
#include <QUuid>
#include <QJsonObject>
#include <QPointF>
#include <QRectF>

class ShapeGO : public AbstractShapeGO
{
    Q_OBJECT
public:
    ShapeGO(QObject *parent = 0);
signals:
    void addShape(QJsonObject shape);
    void shapeAdded();

    void rcvShapePayload(QJsonObject obj);

    void sendObjToHandler(QJsonObject shape);

    void updateScore(int score, int userNum);
public slots:
    void startShape();
    //void storeUserID(QUuid id){m_UserID = id;}
    void shapeRejected(QJsonObject response);
    void moveShape(QJsonObject shape);
    void changePos();
    void setField(QJsonObject initObj);

    void removeShape();

    void setNumberOfUsers(int num){m_NumOfUsers = num;}

    void resetFieldBool(){fieldSet = false;}
private:
    bool isStarted;
    bool fieldSet;
    QPointF oldPos1;
    QPointF oldPos2;
    int XDir;
    int YDir;
    QRectF m_Field;
    QRectF m_Ball;
    int m_ObjV;
    //QUuid m_UserID;
    QUuid m_ShapeID;
    //QTimer m_UpdateTimer;

    int m_NumOfUsers;
    int m_Score0;
    int m_Score1;
    int m_Score2;
    int m_Score3;
};

#endif // SHAPEGO_H
