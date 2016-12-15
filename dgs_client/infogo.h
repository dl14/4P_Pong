#ifndef INFOGO_H
#define INFOGO_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>

#include "abstractshapego.h"
#include "sendmsghandler.h"

class InfoGO : public AbstractShapeGO
{
    Q_OBJECT
public:
    InfoGO();
signals:
    void sendGameObject(QJsonObject obj);

    void sendObjToHandler(QJsonObject game);

    void rcvGameObjectForHandler(QJsonObject obj);

    //void sendNumPlayers(int num);

    void createBall();

    void createPaddle(int num);

    void showUserName(QUuid id, QString playerNum);

    void sendUpdatedScore(int score, int playerNum);

    void restartGame();

    void sendP0Score(QString nameAndScore);
    void sendP1Score(QString nameAndScore);
    void sendP2Score(QString nameAndScore);
    void sendP3Score(QString nameAndScore);

    //FROM HANDLER
    void sendToHandler(QJsonObject obj);
    void sendObjectToSender(QJsonObject obj);
public slots:
    void rcvInit(QJsonObject payload);
    void storeUserId(QUuid id){m_UserID = id;}
    void ballAdded();
    void gameStarted();
    void gameStopped();
    void requestToJoin();
    void rcvGamePayload(QJsonObject payload);

    void updateScores(int score, int playerNum);

    void storeUserName(QString name){m_UserName = name;}
    //void gameInfoObject(QJsonObject )

    //FOR HANDLER
    void rcvObjectResponse(QJsonObject obj);
private:
    bool m_PaddleCreated;
    QString m_UserName;
    QUuid m_UserID;
    bool m_HasGameObject;
};

#endif // INFOGO_H
