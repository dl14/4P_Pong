#include "gsm.h"

GSM::GSM(QObject *parent) : QObject(parent),
    m_PlayerNum(-1)
{
    m_State = States::Init;
    // Initialize the timer that will drive the "animation"
    m_UpdateTimer.setInterval(msUpdateTime);
    connect(&m_UpdateTimer, &QTimer::timeout, this, &GSM::timerUpdated);
}

void GSM::userPayloadRcvd()
{
    if(m_State == States::Init){
        m_State = States::UserPayloadApprvd;
        emit enableJoinButton();
    }
}

void GSM::playerJoined(int playerNumber)
{
    m_PlayerNum = playerNumber;
    emit startPaddle(m_PlayerNum);
    emit enableLRButtons();
}

void GSM::initShapeAdded()
{
    if(m_State == States::UserPayloadApprvd){
        m_State = States::BallAdded;
        emit ballCreated();
        if(m_PlayerNum == 0)
            emit enableSSButtons();
    }
}

void GSM::joinGame()
{
    if(m_State == States::UserPayloadApprvd){
        emit requestToJoin();
    }
}

void GSM::timerUpdated()
{
    emit updateTimer();
}

void GSM::startGame()
{
    if(m_State == States::BallAdded){
        m_State = States::StartGame;
        emit gameStarted();
        if(m_PlayerNum == 0){
            m_UpdateTimer.start();
        }
    }
}

void GSM::stopGame()
{
    if(m_State == States::StartGame){
        if(m_PlayerNum == 0){
            m_State = States::UserPayloadApprvd;
            emit gameStopped();
            emit disableLRButtons();
            emit enableJoinButton();
            m_UpdateTimer.stop();
        }
    }
}
