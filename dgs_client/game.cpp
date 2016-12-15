#include "game.h"

Game::Game(Client *client, GraphicsWidget *gWidget, UserWidget *uWidget, GameControllerWidget *gcWidget, GameModeWidget *gmWidget, Logger *logger, QObject *parent) : QObject(parent),
    m_Socket(new QTcpSocket()),
    m_Parser(new DataParser(m_Socket)),
    m_Sender(new DataSender(m_Socket)),
    m_Gsm(new GSM()),
    m_GameInfo(new InfoGO()),
    m_Users(new UserGO()),
    m_Shape(new ShapeGO()),
    m_Paddle(new PaddleGO()),
    m_Logger(logger)
{
    connect(client, &Client::connectionRequested, this, &Game::connectToServer);
    connect(client, &Client::userName, m_Users, &UserGO::rcvUserName);
    connect(client, &Client::disconnectionRequested, this, &Game::disconnectServer);

    connect(gcWidget, &GameControllerWidget::leftButtonClicked, m_Paddle, &PaddleGO::moveLeft);
    connect(gcWidget, &GameControllerWidget::rightButtonClicked, m_Paddle, &PaddleGO::moveRight);

    connect(gmWidget, &GameModeWidget::startButtonClicked, m_Gsm, &GSM::startGame);
    connect(gmWidget, &GameModeWidget::stopButtonClicked, m_Gsm, &GSM::stopGame);
    connect(gmWidget, &GameModeWidget::joinButtonClicked, m_Gsm, &GSM::joinGame);

    connect(m_Users, &UserGO::sendUserName, uWidget, &UserWidget::updateList);

    connect(m_Socket, &QAbstractSocket::connected, client, &Client::setStatusTextConnected);
    connect(m_Socket, &QAbstractSocket::disconnected, m_Parser, &DataParser::resetInitData);
    connect(m_Socket, &QAbstractSocket::disconnected, client, &Client::setStatusTextDisconnected);
    connect(m_Socket, &QAbstractSocket::disconnected, uWidget, &UserWidget::eraseList);
    connect(m_Socket, &QAbstractSocket::disconnected, gWidget, &GraphicsWidget::removeShapes);
    connect(m_Socket, &QAbstractSocket::disconnected, m_Shape, &ShapeGO::resetFieldBool);
    connect(m_Socket, &QAbstractSocket::disconnected, m_Paddle, &PaddleGO::resetFieldBool);
    //connect(m_Socket, &QAbstractSocket::disconnected, m_Paddle, &PaddleGO::deletePaddle);
    connect(m_Socket, &QAbstractSocket::disconnected, m_Users, &UserGO::setStateAfterDisconnection);

    connect(m_Socket, &QIODevice::readyRead, m_Parser, &DataParser::readByteArray);

    //connect(m_Parser, &DataParser::sendToHandler, m_Users, &UserGO::rcvPayload);
    connect(m_Parser, SIGNAL(sendToHandler(QJsonObject)), m_Users, SIGNAL(rcvUserPayload(QJsonObject)));
    connect(m_Parser, SIGNAL(sendToHandler(QJsonObject)), m_Shape, SIGNAL(rcvShapePayload(QJsonObject)));
    connect(m_Parser, SIGNAL(sendToHandler(QJsonObject)), m_Paddle, SIGNAL(rcvShapePayload(QJsonObject)));
    connect(m_Parser, SIGNAL(sendToHandler(QJsonObject)), m_GameInfo, SIGNAL(rcvGameObjectForHandler(QJsonObject)));

    connect(m_Parser, &DataParser::initArrayRcvd, m_Users, &UserGO::checkPayload);
    connect(m_Parser, &DataParser::initArrayRcvd, m_Shape, &ShapeGO::setField);
    connect(m_Parser, &DataParser::initArrayRcvd, m_Paddle, &PaddleGO::setField);
    //connect(m_Parser, &DataParser::initArrayRcvd, m_GameInfo, &InfoGO::rcvInit);

    //connect(m_Parser, &DataParser::userRcvd, m_Users, &UserGO::rcvUser);
    connect(m_Parser, &DataParser::initArrayRcvd, gWidget, &GraphicsWidget::setField);
    connect(m_Parser, &DataParser::shapeRcvd, gWidget, &GraphicsWidget::shapeAdded);
    connect(m_Parser, &DataParser::showUserName, uWidget, &UserWidget::updateList);
    connect(m_Parser, &DataParser::gameObjectRcvd, m_GameInfo, &InfoGO::rcvGamePayload);

    //connect(m_Parser, &DataParser::shapeRcvd, m_Shape, &ShapeGO::moveShape);
    //connect(m_Parser, &DataParser::rejectionRcvd, m_Shape, &ShapeGO::shapeRejected);
    //connect(m_Parser, &DataParser::shapeRcvd, m_Paddle, &PaddleGO::moveShape);

    //connect(m_Users, &UserGO::addUser, m_Sender, &DataSender::sendObj);
    connect(m_Users, &UserGO::addUser, m_Sender, &DataSender::sendObject);

    connect(m_Users, &UserGO::initUserRcvd, m_Gsm, &GSM::userPayloadRcvd);
    //connect(m_Users, SIGNAL(numberOfUsers(int)), m_Gsm, SIGNAL(numberOfUsers(int)));
    //connect(m_Users, &UserGO::numberOfUsers, m_Gsm, &GSM::storeNumUsers);
    connect(m_Users, &UserGO::sendUserId, m_GameInfo, &InfoGO::storeUserId);
    connect(m_Users, &UserGO::sendUserId, m_Sender, &DataSender::storeUserID);
    connect(m_Users, &UserGO::sendUserId, gWidget, &GraphicsWidget::storeUserID);

    connect(m_Users, &UserGO::showShape, gWidget, &GraphicsWidget::shapeAdded);
    connect(m_Users, &UserGO::initPayload, m_GameInfo, &InfoGO::rcvInit);

    connect(m_Users, &UserGO::sendUserNameToInfo, m_GameInfo, &InfoGO::storeUserName);
    //connect(m_Users, &UserGO::sendUserId, m_Shape, &AbstractShapeGO::storeUserID);
    //connect(m_Users, &UserGO::sendUserId, m_Paddle, &PaddleGO::storeUserID);

    //connect(m_Gsm, &GSM::startShape, m_Shape, &ShapeGO::startShape);
    connect(m_Gsm, &GSM::updateTimer, m_Shape, &ShapeGO::changePos);
    connect(m_Gsm, &GSM::enableLRButtons, gcWidget, &GameControllerWidget::enableButtons);
    //connect(m_Gsm, &GSM::disableLRButtons, gcWidget, &GameControllerWidget::disableButtons);
    connect(m_Gsm, &GSM::enableJoinButton, gmWidget, &GameModeWidget::enableJoinButton);
    connect(m_Gsm, &GSM::enableSSButtons, gmWidget, &GameModeWidget::enableSSButtons);
    connect(m_Gsm, &GSM::startPaddle, m_Paddle, &PaddleGO::addPaddle);
    connect(m_Gsm, &GSM::startPaddle, m_Shape, &ShapeGO::setNumberOfUsers);
    //connect(m_Gsm, &GSM::numberOfUsers, m_Paddle, &PaddleGO::storeNumberOfUsers);
    //connect(m_Gsm, &GSM::numberOfUsers, m_Shape, &ShapeGO::setNumberOfUsers);

    connect(m_Gsm, &GSM::requestToJoin, m_GameInfo, &InfoGO::requestToJoin);
    connect(m_Gsm, &GSM::ballCreated, m_GameInfo, &InfoGO::ballAdded);
    connect(m_Gsm, &GSM::gameStarted, m_GameInfo, &InfoGO::gameStarted);

    connect(m_Gsm, &GSM::gameStopped, m_GameInfo, &InfoGO::gameStopped);
    connect(m_Gsm, &GSM::gameStopped, m_Shape, &ShapeGO::removeShape);
    connect(m_Gsm, &GSM::gameStopped, m_Paddle, &PaddleGO::deletePaddle);

    //connect(m_Gsm, &GSM::gameStopped, gWidget, &GraphicsWidget::removeShapes);

    //connect(m_Shape, &ShapeGO::addShape, m_Sender, &DataSender::sendObj);
    connect(m_Shape, &ShapeGO::addShape, m_Sender, &DataSender::sendObject);
    connect(m_Shape, &ShapeGO::shapeAdded, m_Gsm, &GSM::initShapeAdded);
    connect(m_Shape, &ShapeGO::updateScore, m_GameInfo, &InfoGO::updateScores);

    //connect(m_Paddle, &PaddleGO::addShape, m_Sender, &DataSender::sendObj);
    connect(m_Paddle, &PaddleGO::addShape, m_Sender, &DataSender::sendObject);

    connect(m_GameInfo, &InfoGO::sendGameObject, m_Sender, &DataSender::sendObject);
    connect(m_GameInfo, &InfoGO::createPaddle, m_Gsm, &GSM::playerJoined);
    connect(m_GameInfo, &InfoGO::createBall, m_Shape, &ShapeGO::startShape);
    //NEEDS CHANGE
    connect(m_GameInfo, &InfoGO::showUserName, uWidget, &UserWidget::showPlayer);
    connect(m_GameInfo, &InfoGO::sendUpdatedScore, uWidget, &UserWidget::updateScore);
    connect(m_GameInfo, &InfoGO::restartGame, m_Paddle, &PaddleGO::deletePaddle);
    connect(m_GameInfo, &InfoGO::restartGame, m_Gsm, &GSM::gameRestarted);
    connect(m_GameInfo, SIGNAL(sendP0Score(QString)), uWidget, SIGNAL(setP0NameScore(QString)));
    connect(m_GameInfo, SIGNAL(sendP1Score(QString)), uWidget, SIGNAL(setP1NameScore(QString)));
    connect(m_GameInfo, SIGNAL(sendP2Score(QString)), uWidget, SIGNAL(setP2NameScore(QString)));
    connect(m_GameInfo, SIGNAL(sendP3Score(QString)), uWidget, SIGNAL(setP3NameScore(QString)));

    //connect(m_GameInfo, &InfoGO::sendNumPlayers, m_Gsm, &GSM::storeNumUsers);
    //connect(m_GameInfo, &InfoGO::createBall

    connect(client, &Client::quitGame, this, &Game::deleteObjects);

    connect(m_Parser, &DataParser::sendToLogger, m_Logger, &Logger::writeInput);
    connect(m_Sender, &DataSender::sendingData, m_Logger, &Logger::writeOutput);
}

void Game::connectToServer(QString ip, quint16 port)
{
    m_Socket->abort();
    m_Socket->connectToHost(ip, port);
    m_Socket->waitForConnected();
    //int x = m_Socket->state();
}

void Game::deleteObjects()
{
    m_Parser->deleteLater();
    m_Sender->deleteLater();
    m_Gsm->deleteLater();
    m_GameInfo->deleteLater();
    m_Users->deleteLater();
    m_Shape->deleteLater();
    m_Paddle->deleteLater();

    m_Logger->deleteLater();
}
