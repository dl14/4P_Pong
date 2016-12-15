#include <QApplication>
#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QTcpSocket>
#include <QObject>
#include <QDataStream>
#include <QtTest/QTest>
//PARSER WILL HANDLE ONLY A BYTE ARRAY
//it won't care how it got it (tcpSocket, file, etc.)
//parser's job isn't to read the data... it's more about handling the QByteArray (JSON Doc.)
//and converting it to JSON Object

//fromJason gives you QJasonDocument
#include "dataparser.h"
#include "dialog.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    /*QFile file("json.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return 0;

    QByteArray line = file.readAll();
    QTcpSocket *socket = new QTcpSocket();
    DataParser *parser = new DataParser(socket);
    QObject::connect(socket, &QIODevice::readyRead, parser, &DataParser::readByteArray);
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_0);
    QString x = "hi";
    out << x;
    socket->write(block);*/

    /*QJsonDocument doc = QJsonDocument::fromJson(line);
    if(!doc.isObject())
        qDebug() << "not object";

    QJsonObject jsonObj = doc.object();

    QJsonValue item = jsonObj.value("FirstName");
    qDebug() << item.toString();*/



    return a.exec();
}
