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
    ~DataParser();
    bool checkJSON(QByteArray data);
public slots:
    void readByteArray();

    void resetInitData(){initDataSent = false;}
signals:
    void sendToLogger(QByteArray data);

    //send to handler
    void sendToHandler(QJsonObject obj);
    //send to data sender
    void rejectObject(QString errorMsg, QString msgId);

    void initArrayRcvd(QJsonObject initArray);
    void userRcvd(QJsonObject user);
    void shapeRcvd(QJsonObject shape);
    void rejectionRcvd(QJsonObject response);

    void showUserName(QUuid id, QString name);

    void gameObjectRcvd(QJsonObject obj);

private:
    bool initDataSent;
    bool isProperPayload(QJsonObject payload);
    QTcpSocket *m_Socket;

};

#endif // DATAPARSER_H
