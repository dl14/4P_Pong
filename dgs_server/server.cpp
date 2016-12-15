#include "server.h"
#include "serverthread.h"
#include <stdlib.h>

#include <QDebug>
#include <QDataStream>
#include <QtNetwork>

//! [0]
Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    m_Logger = new Logger();
    m_Logger->init();
    m_ObjectRepo = new ObjectRepo();
}

Server::~Server()
{

}

void Server::incomingConnection(qintptr socketDescriptor)
{
    ServerThread *thread = new ServerThread(m_Logger, m_ObjectRepo, socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}



