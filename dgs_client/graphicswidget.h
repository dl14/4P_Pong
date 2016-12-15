#ifndef GRAPHICSWIDGET_H
#define GRAPHICSWIDGET_H

#include <QObject>
#include <QWidget>

//////////////////////////////////////////
// Place any include files that you need here
//////////////////////////////////////////
#include <QGraphicsView>
#include <QJsonObject>
#include <QMap>
#include <QUuid>

class GraphicsWidget : public QWidget
{
    Q_OBJECT
public:
    GraphicsWidget(QRectF sceneSize, QWidget *parent = 0);

public slots:
    void setField(QJsonObject initObj);
    void shapeAdded(QJsonObject addShape);

    void storeUserID(QUuid id){m_UserID = id;}

    void removeShapes();

    void checkUpdate(QList<QRectF> region);
private:
    //AbstractPlayground *m_Model;
    QUuid m_UserID;
    QGraphicsView *graphicView;
    QGraphicsScene *graphicScene;
    QMap<QUuid, QGraphicsRectItem*> rectItems;
    //QList<QGraphicsRectItem*> rectItems;
};

#endif // GRAPHICSWIDGET_H
