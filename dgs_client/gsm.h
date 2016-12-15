#ifndef GSM_H
#define GSM_H

#include <QObject>
#include <QJsonObject>
#include <QTimer>

const int msUpdateTime = 50;
const qreal msPerSec = 1000.0;

class GSM : public QObject
{
    Q_OBJECT
public:
    GSM(QObject *parent = 0);
signals:
    //void startShape();
    void ballCreated();
    void updateTimer();
    void enableLRButtons(); //left right
    void disableLRButtons();
    void enableSSButtons(); //start stop
    void enableJoinButton();
    void startPaddle(int num);
    void requestToJoin();
    void gameStarted();
    void gameStopped();

    void disableJoinButton();
public slots:
    void playerJoined(int playerNumber);
    void userPayloadRcvd();
    void joinGame();
    void startGame();
    void stopGame();
    void initShapeAdded();
    void timerUpdated();
    void gameRestarted(){m_State = States::UserPayloadApprvd;}

    //void storeNumUsers(int number){m_NumUsers = number;}
private:
    int m_PlayerNum;
    QTimer m_UpdateTimer;
    enum States {Init, UserPayloadApprvd, BallAdded, StartGame, StopGame, Error} m_State;
};

#endif // GSM_H
