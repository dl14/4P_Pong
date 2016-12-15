#include "infogo.h"

InfoGO::InfoGO(): m_HasGameObject(false), m_PaddleCreated(false)
{

}

void InfoGO::rcvInit(QJsonObject payload)
{
    QJsonArray initPayload = payload.value(QString("InitArray")).toArray();
    for(int i = 0; i < initPayload.size(); i++){
        QJsonObject obj = initPayload[i].toObject();
        if(obj.value(QString("ObjType")).toString() == "GamePayload"){
            m_Object = obj;
            m_HasGameObject = true;
        }
    }
}

void InfoGO::rcvGamePayload(QJsonObject payload)
{
    //ADD STUFF IF YOU RECEIVE A STOPPED GAME PAYLOAD
    m_Object = payload;
    m_HasGameObject = true;
    QJsonObject cData = payload["ClientData"].toObject();
    if(cData.value("State").toString() == "GameStopped"){
        //DISPLAY WINNER!
        QJsonObject scores = cData["PrevScores"].toObject();
        if(!scores.contains("P1"))
            emit sendP0Score("P0: " + cData["PrevUserNames"].toObject().value("P0").toString() + " " +
                             QString::number(scores["P0"].toInt()) + " W" );
        else if(!scores.contains("P2")){
            if(scores["P0"].toInt() < scores["P1"].toInt())
                emit sendP0Score("P0: " + cData["PrevUserNames"].toObject().value("P0").toString() + " " +
                                 QString::number(scores["P0"].toInt()) + " W");
            else if(scores["P0"].toInt() > scores["P1"].toInt())
                emit sendP1Score("P1: " + cData["PrevUserNames"].toObject().value("P1").toString() + " " +
                                 QString::number(scores["P1"].toInt()) + " W");
            else{
                emit sendP0Score("P0: " + cData["PrevUserNames"].toObject().value("P0").toString() + " " +
                                 QString::number(scores["P0"].toInt()) + " W");
                emit sendP1Score("P1: " + cData["PrevUserNames"].toObject().value("P1").toString() + " " +
                                 QString::number(scores["P1"].toInt()) + " W");
            }
        }

        m_PaddleCreated = false;
        emit restartGame();
    }
    if(cData["State"].toString() == "Init"){
        emit sendP0Score("P0: " + cData["UserNames"].toObject().value("P0").toString());
        emit sendP1Score("P1: " + cData["UserNames"].toObject().value("P1").toString());
        emit sendP2Score("P2: " + cData["UserNames"].toObject().value("P2").toString());
        emit sendP3Score("P3: " + cData["UserNames"].toObject().value("P3").toString());
    }
    if(cData["State"].toString() == "GameStarted"){
        emit sendP0Score("P0: " + cData["UserNames"].toObject().value("P0").toString() + " " +
                         QString::number(cData["Scores"].toObject().value("P0").toInt()));
        emit sendP1Score("P1: " + cData["UserNames"].toObject().value("P1").toString() + " " +
                         QString::number(cData["Scores"].toObject().value("P1").toInt()));
        emit sendP2Score("P2: " + cData["UserNames"].toObject().value("P2").toString() + " " +
                         QString::number(cData["Scores"].toObject().value("P2").toInt()));
        emit sendP3Score("P3: " + cData["UserNames"].toObject().value("P3").toString() + " " +
                         QString::number(cData["Scores"].toObject().value("P3").toInt()));
    }
}

void InfoGO::requestToJoin()
{
    if(m_HasGameObject){
        QJsonObject cData = m_Object["ClientData"].toObject();
        if(cData["State"] != "GameStarted"){
            QJsonObject users = cData["Users"].toObject();
            QJsonObject userNames = cData["UserNames"].toObject();
            QJsonObject scores = cData["Scores"].toObject();
            if(users.contains("P0")){
                if(users.contains("P1")){
                    if(users.contains("P2")){
                        if(users.contains("P3")){
                            qDebug() << "Please wait!";
                            return;
                        }
                        else{
                            users["P3"] = m_UserID.toString();
                            userNames["P3"] = m_UserName;
                            scores["P3"] = 0;
                        }
                    }
                    else{
                        users["P2"] = m_UserID.toString();
                        userNames["P2"] = m_UserName;
                        scores["P2"] = 0;
                    }
                }
                else{
                    users["P1"] = m_UserID.toString();
                    userNames["P1"] = m_UserName;
                    scores["P1"] = 0;
                }
            }
            else{
                users["P0"] = m_UserID.toString();
                userNames["P0"] = m_UserName;
                scores["P0"] = 0;
            }

            QJsonObject game;
            game["ObjUuid"] = m_Object["ObjUuid"].toString();
            game["ObjOwnerUuid"] = m_UserID.toString();
            game["ObjType"] = QString("GamePayload");
            game["ObjVersion"] = this->getObjV() + 1;
            QJsonObject clientData;
            clientData["State"] = QString("Init");
            clientData["Users"] = users;
            clientData["UserNames"] = userNames;
            clientData["Scores"] = scores;
            game["ClientData"] = clientData;

            SendMsgHandler *handler = new SendMsgHandler;
            connect(this, &InfoGO::sendToHandler, handler, &SendMsgHandler::sendPayload);
            connect(this, &InfoGO::rcvGameObjectForHandler, handler, &SendMsgHandler::rcvPayload);
            connect(handler, &SendMsgHandler::sendObj, this, &InfoGO::sendGameObject);
            connect(handler, &SendMsgHandler::completePayload, this, &InfoGO::rcvObjectResponse);
            emit sendToHandler(game);
        }
    }
    else{
        QJsonObject game;
        game["ObjUuid"] = QUuid().createUuid().toString();
        game["ObjOwnerUuid"] = m_UserID.toString();
        game["ObjType"] = QString("GamePayload");
        game["ObjVersion"] = 1;
        QJsonObject clientData;
        clientData["State"] = QString("Init");
        QJsonObject users;
        users["P0"] = m_UserID.toString();
        QJsonObject scores;
        scores["P0"] = int(0);
        clientData["Users"] = users;
        clientData["Scores"] = scores;
        QJsonObject userNames;
        userNames["P0"] = m_UserName;
        clientData["UserNames"] = userNames;
        game["ClientData"] = clientData;

        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &InfoGO::sendToHandler, handler, &SendMsgHandler::sendPayload);
        connect(this, &InfoGO::rcvGameObjectForHandler, handler, &SendMsgHandler::rcvPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &InfoGO::sendGameObject);
        connect(handler, &SendMsgHandler::completePayload, this, &InfoGO::rcvObjectResponse);
        emit sendToHandler(game);
    }
}

void InfoGO::rcvObjectResponse(QJsonObject obj)
{
    m_Object = obj;
    QJsonObject cData = m_Object["ClientData"].toObject();
    QJsonObject users = cData["Users"].toObject();

    if(!m_PaddleCreated){
        QString x = users["P0"].toString();
        if(users["P0"].toString() == m_UserID.toString()){
            emit createPaddle(0);
            m_PaddleCreated = true;
        }
        else if(users["P1"].toString() == m_UserID.toString()){
            emit createPaddle(1);
            m_PaddleCreated = true;
        }
        else if(users["P2"].toString() == m_UserID.toString()){
            emit createPaddle(2);
            m_PaddleCreated = true;
        }
        else if(users["P3"].toString() == m_UserID.toString()){
            emit createPaddle(3);
            m_PaddleCreated = true;
        }
    }
    QString state = cData["State"].toString();
    if(cData["State"].toString() == "Init"){
        emit createBall();
    }
    //USELESS
    /*if(m_Object.contains("Scores")){
        QJsonObject scores = m_Object["Scores"].toObject();
        if(scores.contains("P0"))
            emit sendUpdatedScore(scores["P0"].toInt(), 0);
        if(scores.contains("P1"))
            emit sendUpdatedScore(scores["P1"].toInt(), 1);
        if(scores.contains("P2"))
            emit sendUpdatedScore(scores["P2"].toInt(), 2);
        if(scores.contains("P3"))
            emit sendUpdatedScore(scores["P3"].toInt(), 3);
    }*/
}

void InfoGO::ballAdded()
{
    QJsonObject obj = m_Object;
    obj["ObjVersion"] = this->getObjV() + 1;
    QJsonObject cData = m_Object["ClientData"].toObject();
    cData["State"] = QString("ReadyToPlay");
    obj["ClientData"] = cData;

    SendMsgHandler *handler = new SendMsgHandler;
    connect(this, &InfoGO::sendToHandler, handler, &SendMsgHandler::sendPayload);
    connect(this, &InfoGO::rcvGameObjectForHandler, handler, &SendMsgHandler::rcvPayload);
    connect(handler, &SendMsgHandler::sendObj, this, &InfoGO::sendGameObject);
    connect(handler, &SendMsgHandler::completePayload, this, &InfoGO::rcvObjectResponse);
    emit sendToHandler(obj);
}

void InfoGO::gameStarted()
{
    QJsonObject obj = m_Object;
    obj["ObjVersion"] = this->getObjV() + 1;
    QJsonObject cData = m_Object["ClientData"].toObject();
    cData["State"] = QString("GameStarted");
    obj["ClientData"] = cData;

    SendMsgHandler *handler = new SendMsgHandler;
    connect(this, &InfoGO::sendToHandler, handler, &SendMsgHandler::sendPayload);
    connect(this, &InfoGO::rcvGameObjectForHandler, handler, &SendMsgHandler::rcvPayload);
    connect(handler, &SendMsgHandler::sendObj, this, &InfoGO::sendGameObject);
    connect(handler, &SendMsgHandler::completePayload, this, &InfoGO::rcvObjectResponse);
    emit sendToHandler(obj);
}

void InfoGO::gameStopped()
{
    m_PaddleCreated = false;

    QJsonObject obj = m_Object;
    obj["ObjVersion"] = this->getObjV() + 1;
    QJsonObject cData = m_Object["ClientData"].toObject();
    cData["State"] = QString("GameStopped");
    QJsonObject users;
    cData["Users"] = users;
    QJsonObject prevScores = cData["Scores"].toObject();
    cData["PrevScores"] = prevScores;
    QJsonObject scores;
    cData["Scores"] = scores;
    QJsonObject prevUN = cData["UserNames"].toObject();
    cData["PrevUserNames"] = prevUN;
    QJsonObject userNames;
    cData["UserNames"] = userNames;
    obj["ClientData"] = cData;

    /*emit sendP0Score("P0: ");
    emit sendP1Score("P1: ");
    emit sendP2Score("P2: ");
    emit sendP3Score("P3: ");*/

    SendMsgHandler *handler = new SendMsgHandler;
    connect(this, &InfoGO::sendToHandler, handler, &SendMsgHandler::sendPayload);
    connect(this, &InfoGO::rcvGameObjectForHandler, handler, &SendMsgHandler::rcvPayload);
    connect(handler, &SendMsgHandler::sendObj, this, &InfoGO::sendGameObject);
    connect(handler, &SendMsgHandler::completePayload, this, &InfoGO::rcvObjectResponse);
    emit sendToHandler(obj);
}

void InfoGO::updateScores(int score, int playerNum)
{
    bool update = false;
    QJsonObject obj = m_Object;
    QJsonObject cData = obj["ClientData"].toObject();
    QJsonObject scores = cData["Scores"].toObject();
    if(playerNum == 0 && scores.contains("P0")){
        update = true;
        scores["P0"] = score;
    }
    else if(playerNum == 1 && scores.contains("P1")){
        update = true;
        scores["P1"] = score;
    }
    else if(playerNum == 2 && scores.contains("P2")){
        update = true;
        scores["P2"] = score;
    }
    else if(playerNum == 3 && scores.contains("P3")){
        update = true;
        scores["P3"] = score;
    }

    if(update){
        obj["ObjVersion"] = this->getObjV() + 1;
        cData["Scores"] = scores;
        obj["ClientData"] = cData;

        SendMsgHandler *handler = new SendMsgHandler;
        connect(this, &InfoGO::sendToHandler, handler, &SendMsgHandler::sendPayload);
        connect(this, &InfoGO::rcvGameObjectForHandler, handler, &SendMsgHandler::rcvPayload);
        connect(handler, &SendMsgHandler::sendObj, this, &InfoGO::sendGameObject);
        connect(handler, &SendMsgHandler::completePayload, this, &InfoGO::rcvObjectResponse);
        emit sendToHandler(obj);
    }
}

