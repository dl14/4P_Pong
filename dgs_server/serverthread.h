#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QTcpSocket>

#include "objectrepo.h"
#include "logger.h"

//! [0]
class ServerThread : public QThread
{
    Q_OBJECT

public:
    ServerThread(Logger *logger, ObjectRepo *repo, int socketDescriptor, QObject *parent);
    ~ServerThread();
    void run() Q_DECL_OVERRIDE;
signals:
    void error(QTcpSocket::SocketError socketError);
    void initShapes();
private:
    int socketDescriptor;
    QTcpSocket *m_TcpSocket;
    ObjectRepo *m_ObjRepo;
    Logger     *m_Logger;
};
#endif // SERVERTHREAD_H
