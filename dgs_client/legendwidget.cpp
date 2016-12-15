#include "legendwidget.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QRectF>
#include <QPointF>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QLabel>
#include <QFile>

LegendWidget::LegendWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *horiLayout = new QHBoxLayout;

    QPixmap image("legend.png");
    QLabel *imageLabel = new QLabel();
    imageLabel->setPixmap(image);
    horiLayout->addWidget(imageLabel);
    QGroupBox *groupBox = new QGroupBox(tr("Legend"));
    groupBox->setLayout(horiLayout);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(groupBox);

    QFile *file = new QFile("instructions.txt");
    QLabel *label = new QLabel();
    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        QString text("Sorry.. instructions.txt file not found!");
        label->setText(text);
    }
    else{
        QByteArray instructions = file->readAll();
        QString text(instructions);
        label->setText(text);
    }

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QGroupBox *labelBox = new QGroupBox(tr("Instructions"));

    hLayout2->addWidget(label);
    labelBox->setLayout(hLayout2);
    hLayout->addWidget(labelBox);

    this->setLayout(hLayout);
}
