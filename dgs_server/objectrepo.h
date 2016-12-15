#ifndef OBJECTREPO_H
#define OBJECTREPO_H

#include "globalvalues.h"

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMap>
#include <QRectF>
#include <QUuid>

class ObjectRepo : public QObject
{
    Q_OBJECT
public:
    ObjectRepo(QObject *parent = 0);

public slots:
    void initData();
    void insertObj(QJsonObject obj, QUuid msgId);
    void removeObj(QJsonObject obj, QUuid msgId);
    void changeObj(QJsonObject obj, QUuid msgId);
signals:
    void sendInit(QJsonArray data, QUuid rootID);
    void firstUserAdded(QUuid msgId);
    void invalidRequest(QString errorMsg, QUuid msgId);
    void validRequest(QString errorMsg, QUuid msgId);
    void messageProcessed(QJsonObject obj);
    void disconnectClient();
private:
    QMap<QUuid, QJsonObject> m_Map;
    QUuid m_RootID;
    QUuid m_GMID;
    QRectF m_Field;

    //bool isProperPayload(QJsonObject payload);
    bool isDuplicateName(QString fieldCheck, QString checkName);
    bool isDuplicateNameChange(QString fieldCheck, QString checkName, QUuid objId);
    bool isWithinField(double topLeftX, double topLeftY, double bottomRightX, double bottomRightY);
    bool isIntersection(double topLeftX, double topLeftY, double bottomRightX, double bottomRightY, QUuid ObjId);
    //PERMISSIONS
    bool isRoot(QUuid objID);
    bool isGM(QUuid objID);
    bool isUserInRepo(QString objID);
};

#endif // OBJECTREPO_H
