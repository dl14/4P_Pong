#ifndef DATASENDER_H
#define DATASENDER_H
#include <QObject>
#include <QTcpSocket>
#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDataStream>
#include <QJsonArray>

class DataSender : public QObject
{
    Q_OBJECT
public:
    DataSender(QTcpSocket *socket, QObject *parent = 0);
public slots:
    void receiveRootID(QString id){m_RootID = id;}
    void sendInitData(QJsonArray initArray, QString rootId);
    void sendResponse(QString msgId, QString errorMessage);
    void sendServerChange(QJsonObject obj);
    void disconnectHost();
    //void sendRejectResponse(QString errorMsg, QString msgId);
private:
    QString m_RootID;
    bool initComplete;
    QTcpSocket *m_Socket;
};

#endif // DATASENDER_H
