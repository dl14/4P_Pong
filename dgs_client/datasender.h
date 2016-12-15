#ifndef DATASENDER_H
#define DATASENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUuid>

#include <QByteArray>

class DataSender : public QObject
{
    Q_OBJECT
public:
    DataSender(QTcpSocket * tcpSocket, QObject *parent = 0);
    ~DataSender();
signals:
    void sendingData(QByteArray data);
public slots:
    void sendObj(QJsonObject obj);
    //USE WITH MESSAGE HANDLER
    void sendObject(QJsonObject obj);
    void storeUserID(QUuid id){m_UserID = id;}
private:
    QUuid m_UserID;
    QTcpSocket * m_TcpSocket;

};

#endif // DATASENDER_H
