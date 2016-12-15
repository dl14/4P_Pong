#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QStringList>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

#include "objectrepo.h"

#include "logger.h"

//! [0]
class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);
    ~Server();
protected:
    void incomingConnection(qintptr socketDescriptor) Q_DECL_OVERRIDE;
private:
    QTcpSocket *m_TcpSocket;
    QDataStream out;
    ObjectRepo *m_ObjectRepo;

    Logger *m_Logger;
};

#endif // SERVER_H
