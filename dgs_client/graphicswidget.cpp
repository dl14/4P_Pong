#include "graphicswidget.h"
#include <QtWidgets>
#include <QRectF>
#include <QPointF>

#include <QDebug>
#include <QTime>

GraphicsWidget::GraphicsWidget(QRectF sceneSize, QWidget *parent) : QWidget(parent)
  , graphicView(new QGraphicsView)
  , graphicScene(new QGraphicsScene)
{
    QHBoxLayout *horiLayout = new QHBoxLayout;
    graphicScene->setSceneRect(sceneSize);
    graphicView->setScene(graphicScene);
    //QGraphicsView *view = new QGraphicsView(graphicScene, this);
    horiLayout->addWidget(graphicView);
    this->setLayout(horiLayout);

    connect(graphicScene, &QGraphicsScene::changed, this, &GraphicsWidget::checkUpdate);
}

void GraphicsWidget::setField(QJsonObject initObj)
{
    QJsonArray initArray = initObj["InitArray"].toArray();
    for(int i = 0; i < initArray.size(); i++){
        QJsonObject obj = initArray[i].toObject();
        if(obj["ObjType"].toString() == "FieldPayload"){
            QJsonObject fieldObj = obj["FieldBoundary"].toObject();
            QRectF field(QPointF(fieldObj["UpperLeftX"].toDouble(), fieldObj["UpperLeftY"].toDouble()),
                         QPointF(fieldObj["LowerRightX"].toDouble(), fieldObj["LowerRightY"].toDouble()));
            QPen pen(Qt::DashLine);
            pen.setColor(Qt::gray);
            graphicScene->addRect(field, pen);
            graphicScene->setSceneRect(field);
            break;
        }
    }
}

void GraphicsWidget::shapeAdded(QJsonObject addShape)
{
    if(addShape.value(QString("ObjVersion")).toInt() == 1){
        QPen pen2(Qt::SolidLine);
        pen2.setColor(Qt::gray);
        QBrush brush(Qt::gray);
        if(addShape["ObjOwnerUuid"].toString() == m_UserID.toString()){
            pen2.setColor(Qt::blue);
            brush.setColor(Qt::blue);
        }
        QRectF rect(QPointF(addShape["ShapeBoundingBox"].toObject().value(QString("UpperLeftX")).toDouble(),
                            addShape["ShapeBoundingBox"].toObject().value(QString("UpperLeftY")).toDouble()),
                    QPointF(addShape["ShapeBoundingBox"].toObject().value(QString("LowerRightX")).toDouble(),
                            addShape["ShapeBoundingBox"].toObject().value(QString("LowerRightY")).toDouble()));
        //graphicScene->addEllipse(rect);
        rectItems.insert(QUuid(addShape["ObjUuid"].toString()), graphicScene->addRect(rect, pen2, brush));
        //graphicScene->update(rect);
        //qDebug() << "Added a shape";
    }
    else if(addShape.value(QString("ObjVersion")).toInt() == 0){
        graphicScene->removeItem(rectItems.value(QUuid(addShape["ObjUuid"].toString())));
        rectItems.remove(QUuid(addShape["ObjUuid"].toString()));
    }
    else if(addShape.value(QString("ObjVersion")).toInt() > 1){
        //qDebug() << "moving shape";
        QRectF rect(QPointF(addShape["ShapeBoundingBox"].toObject().value(QString("UpperLeftX")).toDouble(),
                            addShape["ShapeBoundingBox"].toObject().value(QString("UpperLeftY")).toDouble()),
                    QPointF(addShape["ShapeBoundingBox"].toObject().value(QString("LowerRightX")).toDouble(),
                            addShape["ShapeBoundingBox"].toObject().value(QString("LowerRightY")).toDouble()));

        if(rectItems.contains(QUuid(addShape["ObjUuid"].toString()))){
            if(addShape["ShapeBoundingBox"].toObject().value(QString("UpperLeftY")).toDouble() == -270){
                qDebug() << QTime().currentTime().toString();
                qDebug() << "updating top paddle!";
            }
            rectItems.value(QUuid(addShape["ObjUuid"].toString()))->setRect(rect);
            //graphicScene->update(rectItems.value(QUuid(addShape["ObjUuid"].toString()))->rect());
        }
        else{
            //qDebug() << "trying to draw shape that DNE";
        }
    }
}

void GraphicsWidget::removeShapes()
{
    QMap<QUuid, QGraphicsRectItem*>::iterator it = rectItems.begin();
    while(it != rectItems.end()){
        graphicScene->removeItem(it.value());
        it++;
    }
    rectItems.clear();
}

void GraphicsWidget::checkUpdate(QList<QRectF> region)
{
    for(int i = 0; i < region.size(); i++){
        QRectF rect = region.at(i);
        //qDebug() << QTime().currentTime().toString();
        //qDebug() << "Change at" << rect.x() << rect.y();
    }
}
