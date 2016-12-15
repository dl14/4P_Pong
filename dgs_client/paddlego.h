#ifndef PADDLEGO_H
#define PADDLEGO_H

#include "abstractshapego.h"
#include "sendmsghandler.h"

#include <QObject>
#include <QUuid>
#include <QJsonObject>
#include <QPointF>
#include <QRectF>
#include <QJsonArray>
class PaddleGO : public AbstractShapeGO
{
    Q_OBJECT
public:
    PaddleGO(QObject *parent = 0);
signals:
    void addShape(QJsonObject paddle);

    void rcvShapePayload(QJsonObject paddle);
    void sendObjToHandler(QJsonObject shape);
public slots:
    void setField(QJsonObject initObj);
    //void setPaddleType(int type){m_PaddleType = type;}
    void addPaddle(int playerNum);
    //void storeUserID(QUuid id){m_UserID = id;}
    void moveShape(QJsonObject shape);
    void moveLeft();
    void moveRight();

    void deletePaddle();

    void resetFieldBool(){fieldSet = false;}
    //void storeNumberOfUsers(int numberOfUsers){m_NumOfUsers = numberOfUsers;}
private:
    bool fieldSet;
    QRectF m_Field;
    int m_NumOfUsers;
    QUuid m_PaddleID;
    QUuid m_UserID;
};

#endif // PADDLEGO_H
