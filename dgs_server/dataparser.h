#ifndef DATAPARSER_H
#define DATAPARSER_H
#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonObject>
#include <QUuid>
#include <QByteArray>

class DataParser : public QObject
{
    Q_OBJECT
public:
    DataParser(QTcpSocket *socket, QObject *parent = 0);
    bool checkJSON(QByteArray data);
public slots:
    void readByteArray();
signals:
    void sendToLogger(QByteArray, int socket);

    //send to handler
    void sendToHandler(QJsonObject obj, QUuid msgId);
    //send to data sender
    void rejectObject(QString errorMsg, QString msgId);
private:
    bool isProperPayload(QJsonObject payload);
    QTcpSocket *m_Socket;
};

#endif // DATAPARSER_H
