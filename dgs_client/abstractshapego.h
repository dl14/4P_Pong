#ifndef ABSTRACTSHAPEGO_H
#define ABSTRACTSHAPEGO_H

#include <QObject>
#include <QUuid>
#include <QJsonObject>

class AbstractShapeGO : public QObject
{
    Q_OBJECT
public:
    explicit AbstractShapeGO(QObject *parent = 0);
    virtual ~AbstractShapeGO();
    int getObjV(){return m_Object["ObjVersion"].toInt();}
signals:
    void shapeCreated();
public slots:
    void storeUserID(QUuid id);
protected:
    QJsonObject updateShape(double LRx, double LRy, double ULx, double ULy);
    QJsonObject m_Object;
    QUuid m_UserID;
};

#endif // ABSTRACTSHAPEGO_H
