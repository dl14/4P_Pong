#ifndef CLIENT_H
#define CLIENT_H

#include "dataparser.h"
#include "datasender.h"

#include <QDialog>
#include <QTcpSocket>
#include <QDataStream>

class QComboBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = Q_NULLPTR);
    ~Client();
public slots:
    void connectToServer();
    void displayError(QAbstractSocket::SocketError socketError);
    void enableGetConnected();
    void sessionOpened();
    void setStatusTextConnected();
    void setStatusTextDisconnected();
signals:
    void clearShapes();
    void connectionRequested(QString ip, quint16 port);
    void userName(QString name);
    void disconnectionRequested();

    void quitGame();
private:
    QComboBox *hostCombo;
    QLineEdit *portLineEdit;
    QLineEdit *userNameEdit;
    QLabel *statusLabel;
    QPushButton *getConnected;
    QPushButton *userButton;
    QPushButton *getDisconnected;

    //QTcpSocket *tcpSocket;
    //QDataStream in;
    QString currentFortune;

    QNetworkSession *networkSession;

    DataParser *parser;
    DataSender *sender;
};

#endif
