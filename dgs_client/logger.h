#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>
#include <QByteArray>
#include <QTime>

class Logger : public QObject
{
    Q_OBJECT
public:
    Logger(QObject *parent = 0);
    ~Logger();
    void init();
signals:

public slots:
    void writeInput(QByteArray data);
    void writeOutput(QByteArray data);
private:
    QFile *m_File;
};

#endif // LOGGER_H
