#include <QtWidgets>
#include <QtNetwork>

#include "client.h"

Client::Client(QWidget *parent)
    : QDialog(parent)
    , hostCombo(new QComboBox)
    , portLineEdit(new QLineEdit)
    , userNameEdit(new QLineEdit)
    , getConnected(new QPushButton(tr("Get Connected")))
    , getDisconnected(new QPushButton(tr("Disconnect")))
    , networkSession(Q_NULLPTR)
    //, parser(new DataParser(tcpSocket))
    //, sender(new DataSender(tcpSocket))
{
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    hostCombo->setEditable(true);
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        hostCombo->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            hostCombo->addItem(name + QChar('.') + domain);
    }
    if (name != QLatin1String("localhost"))
        hostCombo->addItem(QString("localhost"));
    // find out IP addresses of this machine
    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // add non-localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (!ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }
    // add localhost addresses
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i).isLoopback())
            hostCombo->addItem(ipAddressesList.at(i).toString());
    }

    portLineEdit->setValidator(new QIntValidator(1, 65535, this));

    QLabel *hostLabel = new QLabel(tr("&Server name:"));
    hostLabel->setBuddy(hostCombo);
    QLabel *portLabel = new QLabel(tr("S&erver port:"));
    portLabel->setBuddy(portLineEdit);
    QLabel *userLabel = new QLabel(tr("User Name:"));
    userLabel->setBuddy(userNameEdit);
    statusLabel = new QLabel(tr("Not Connected to Server"));

    //userNameEdit->setValidator(new );
    getConnected->setDefault(true);
    getConnected->setEnabled(false);

    QPushButton *quitButton = new QPushButton(tr("Quit"));

    connect(userNameEdit, &QLineEdit::textChanged, this, &Client::enableGetConnected);
    //userButton = new QPushButton(tr("Add User"));
    //userButton->setEnabled(false);
    //connect(userButton, &QAbstractButton::clicked, this, &Client::sendUser);
    //connect(jsonButton, &QAbstractButton::clicked, this, &Client::)
    /*QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(getConnected);
    buttonLayout->addWidget(quitButton);
    QGroupBox *buttonBox = new QGroupBox;
    buttonBox->setLayout(buttonLayout);*/
    //buttonBox->addButton(getConnected, QDialogButtonBox::ActionRole);
    //buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole)

    connect(hostCombo, &QComboBox::editTextChanged,
            this, &Client::enableGetConnected);
    connect(portLineEdit, &QLineEdit::textChanged,
            this, &Client::enableGetConnected);
    connect(getConnected, &QAbstractButton::clicked,
            this, &Client::connectToServer);
    connect(quitButton, SIGNAL(clicked(bool)), this, SIGNAL(quitGame()));
    connect(quitButton, &QAbstractButton::clicked, this, &QWidget::close);
    connect(quitButton, &QAbstractButton::clicked, this, &QObject::deleteLater);

    connect(getDisconnected, SIGNAL(clicked(bool)), this, SIGNAL(disconnectionRequested()));

    typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
    //connect(tcpSocket, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error),
            //this, &Client::displayError);

    QGridLayout *mainLayout = Q_NULLPTR;
    if (QGuiApplication::styleHints()->showIsFullScreen() || QGuiApplication::styleHints()->showIsMaximized()) {
        QVBoxLayout *outerVerticalLayout = new QVBoxLayout(this);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
        QHBoxLayout *outerHorizontalLayout = new QHBoxLayout;
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        QGroupBox *groupBox = new QGroupBox(QGuiApplication::applicationDisplayName());
        mainLayout = new QGridLayout(groupBox);
        outerHorizontalLayout->addWidget(groupBox);
        outerHorizontalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Ignored));
        outerVerticalLayout->addLayout(outerHorizontalLayout);
        outerVerticalLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Ignored, QSizePolicy::MinimumExpanding));
    } else {
        mainLayout = new QGridLayout(this);
    }
    mainLayout->addWidget(hostLabel, 0, 0);
    mainLayout->addWidget(hostCombo, 0, 1);
    mainLayout->addWidget(portLabel, 1, 0);
    mainLayout->addWidget(portLineEdit, 1, 1);
    mainLayout->addWidget(userLabel, 2, 0);
    mainLayout->addWidget(userNameEdit, 2, 1);
    mainLayout->addWidget(statusLabel, 3, 0, 1, 2);
    //mainLayout->addWidget(buttonBox, 3, 0, 1, 2);
    mainLayout->addWidget(getConnected, 4, 0, 1, 2, Qt::AlignLeft);
    mainLayout->addWidget(getDisconnected, 5, 0, 1, 2, Qt::AlignLeft);
    mainLayout->addWidget(quitButton, 4, 0, 1, 2, Qt::AlignRight);
    //mainLayout->addWidget(userButton, 5, 0, 1, 2, Qt::AlignLeft);
    //mainLayout->addWidget(userLabel, 4, 0);
    //mainLayout->addWidget(userNameEdit, 4, 1);
    setWindowTitle(QGuiApplication::applicationDisplayName());
    portLineEdit->setFocus();

    QNetworkConfigurationManager manager;
    if (manager.capabilities() & QNetworkConfigurationManager::NetworkSessionRequired) {
        // Get saved network configuration
        QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
        settings.beginGroup(QLatin1String("QtNetwork"));
        const QString id = settings.value(QLatin1String("DefaultNetworkConfiguration")).toString();
        settings.endGroup();

        // If the saved network configuration is not currently discovered use the system default
        QNetworkConfiguration config = manager.configurationFromIdentifier(id);
        if ((config.state() & QNetworkConfiguration::Discovered) !=
            QNetworkConfiguration::Discovered) {
            config = manager.defaultConfiguration();
        }

        networkSession = new QNetworkSession(config, this);
        connect(networkSession, &QNetworkSession::opened, this, &Client::sessionOpened);

        getConnected->setEnabled(false);
        statusLabel->setText(tr("Opening network session."));
        networkSession->open();
    }
}

Client::~Client()
{

}

void Client::connectToServer()
{
    /*getConnected->setEnabled(false);
    tcpSocket->abort();
    tcpSocket->connectToHost(hostCombo->currentText(),
                             portLineEdit->text().toInt());
    //wrong place...
    userButton->setEnabled(true);*/
    emit connectionRequested(hostCombo->currentText(), portLineEdit->text().toInt());
    emit userName(userNameEdit->text());
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Client"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Client"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    //default:
        //QMessageBox::information(this, tr("Client"),
                                 //tr("The following error occurred: %1.")
                                 //.arg(tcpSocket->errorString()));
    }

    getConnected->setEnabled(true);
}

void Client::enableGetConnected()
{
    getConnected->setEnabled((!networkSession || networkSession->isOpen()) &&
                                 !hostCombo->currentText().isEmpty() &&
                                 !portLineEdit->text().isEmpty() &&
                                 !userNameEdit->text().isEmpty());

}

void Client::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice)
        id = networkSession->sessionProperty(QLatin1String("UserChoiceConfiguration")).toString();
    else
        id = config.identifier();

    QSettings settings(QSettings::UserScope, QLatin1String("QtProject"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();

    statusLabel->setText(tr("Connected To Server"));
    emit clearShapes();
    enableGetConnected();
    //userButton->setEnabled(true);
}

void Client::setStatusTextConnected()
{
    qDebug() << "TRANSCRIPT, CONNECTED";
    statusLabel->setText(tr("Connected To Server"));
}

void Client::setStatusTextDisconnected()
{
    statusLabel->setText(tr("Not Connected To Server"));
    emit clearShapes();
}
