#include "serverthread.h"
#include "dataparser.h"
#include "datasender.h"
#include "msghandler.h"

#include <QtNetwork>
#include <QDebug>

#include <QDataStream>
#include <QtNetwork>

ServerThread::ServerThread(Logger *logger, ObjectRepo *repo, int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor), m_ObjRepo(repo), m_Logger(logger)//, m_TcpSocket(new QTcpSocket)
{

}

ServerThread::~ServerThread()
{
    quit();
    wait();
}

void ServerThread::run()
{
    m_TcpSocket = new QTcpSocket();
    if (!m_TcpSocket->setSocketDescriptor(socketDescriptor)) {
        emit error(m_TcpSocket->error());
        return;
    }

    DataParser *parser = new DataParser(m_TcpSocket);
    MsgHandler *handler = new MsgHandler(m_TcpSocket);

    connect(m_TcpSocket, &QAbstractSocket::disconnected, this, &ServerThread::deleteLater);
    connect(m_TcpSocket, &QIODevice::readyRead, parser, &DataParser::readByteArray);

    connect(parser, &DataParser::sendToHandler, handler, &MsgHandler::checkPayload);
    connect(parser, &DataParser::rejectObject, handler, &MsgHandler::sendBadResponse);

    connect(handler, &MsgHandler::insertObject, m_ObjRepo, &ObjectRepo::insertObj);
    connect(handler, &MsgHandler::removeObject, m_ObjRepo, &ObjectRepo::removeObj);
    connect(handler, &MsgHandler::changeObject, m_ObjRepo, &ObjectRepo::changeObj);
    connect(m_ObjRepo, &ObjectRepo::invalidRequest, handler, &MsgHandler::sendResponse);
    connect(m_ObjRepo, &ObjectRepo::validRequest, handler, &MsgHandler::sendResponse);
    connect(m_ObjRepo, &ObjectRepo::messageProcessed, handler, &MsgHandler::sendRepoChange);
    connect(m_ObjRepo, &ObjectRepo::firstUserAdded, handler, &MsgHandler::initUserAdded);
    connect(m_ObjRepo, &ObjectRepo::sendInit, handler, &MsgHandler::sendInitData);

    connect(parser, &DataParser::sendToLogger, m_Logger, &Logger::writeInput);
    connect(handler, &MsgHandler::sendToLogger, m_Logger, &Logger::writeOutput);

    connect(this, &ServerThread::initShapes, m_ObjRepo, &ObjectRepo::initData);
    emit initShapes();
    disconnect(this, &ServerThread::initShapes, m_ObjRepo, &ObjectRepo::initData);
    exec();
}
