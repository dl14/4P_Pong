#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QTcpSocket>

#include "client.h"
#include "graphicswidget.h"
#include "userwidget.h"
#include "gamecontrollerwidget.h"
#include "gamemodewidget.h"

#include "dataparser.h"
#include "datasender.h"
#include "gsm.h"
#include "infogo.h"
#include "usergo.h"
#include "shapego.h"
#include "paddlego.h"

#include "logger.h"

class Game : public QObject
{
    Q_OBJECT
public:
    Game(Client *client, GraphicsWidget *gWidget, UserWidget *uWidget, GameControllerWidget *gcWidget, GameModeWidget *gmWidget, Logger *logger, QObject *parent = 0);
signals:

public slots:
    void connectToServer(QString ip, quint16 port);
    void disconnectServer(){m_Socket->disconnectFromHost();}
    void deleteObjects();
private:
    QTcpSocket *m_Socket;
    DataParser *m_Parser;
    DataSender *m_Sender;
    GSM        *m_Gsm;
    InfoGO     *m_GameInfo;
    UserGO     *m_Users;
    ShapeGO    *m_Shape;
    PaddleGO   *m_Paddle;

    Logger     *m_Logger;
};

#endif // GAME_H
