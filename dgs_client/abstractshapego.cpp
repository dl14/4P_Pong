#include "abstractshapego.h"

AbstractShapeGO::AbstractShapeGO(QObject *parent) : QObject(parent)
{

}

AbstractShapeGO::~AbstractShapeGO()
{

}

QJsonObject AbstractShapeGO::updateShape(double LRx, double LRy, double ULx, double ULy)
{
    QJsonObject shape;
    shape["ObjUuid"] = m_Object["ObjUuid"].toString();
    //shape["ObjOwnerUuid"] = m_UserID.toString();
    shape["ObjType"] = QString("ShapePayload");
    shape["ObjVersion"] = m_Object["ObjVersion"].toInt() + 1;
    shape["ShapeType"] = QString("Rectangle");
    QJsonObject bb;
    bb["LowerRightX"] = LRx;
    bb["LowerRightY"] = LRy;
    bb["UpperLeftX"] = ULx;
    bb["UpperLeftY"] = ULy;
    shape["ShapeBoundingBox"] = bb;

    return shape;
}

void AbstractShapeGO::storeUserID(QUuid id)
{
    m_UserID = id;
}
