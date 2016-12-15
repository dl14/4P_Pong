#include "logger.h"

Logger::Logger(QObject *parent) : QObject(parent)
{

}

Logger::~Logger(){
    m_File->close();
}

void Logger::init()
{
    m_File = new QFile("ServerLog.txt");
    if(!m_File->open(QIODevice::WriteOnly | QIODevice::Text))
        return;
}

void Logger::writeInput(QByteArray data, int socketDescr)
{
    QString line = QString("\n" + QTime().currentTime().toString() + " Rcv From Server at: " + QString::number(socketDescr) + "\n");
    m_File->write(line.toUtf8());
    m_File->write(data);
}

void Logger::writeOutput(QByteArray data, int socketDescr)
{
    QString line = QString("\n" + QTime().currentTime().toString() + " Send To Client at: " + QString::number(socketDescr) + "\n");
    m_File->write(line.toUtf8());
    m_File->write(data);
}
