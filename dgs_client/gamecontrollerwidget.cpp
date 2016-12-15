#include "gamecontrollerwidget.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QTime>
#include <QDebug>

GameControllerWidget::GameControllerWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    leftButton = new QPushButton(tr("Left"));
    rightButton = new QPushButton(tr("Right"));
    leftButton->setEnabled(false);
    rightButton->setEnabled(false);
    leftButton->setShortcut(QKeySequence(Qt::Key_A));
    rightButton->setShortcut(QKeySequence(Qt::Key_D));
    connect(leftButton, &QAbstractButton::clicked, this, &GameControllerWidget::leftClicked);
    connect(rightButton, &QAbstractButton::clicked, this, &GameControllerWidget::rightClicked);

    hLayout->addWidget(leftButton);
    hLayout->addWidget(rightButton);
    this->setLayout(hLayout);
}

void GameControllerWidget::enableButtons()
{
    leftButton->setEnabled(true);
    rightButton->setEnabled(true);
}

void GameControllerWidget::disableButtons()
{
    leftButton->setEnabled(false);
    rightButton->setEnabled(false);
}

void GameControllerWidget::leftClicked()
{
    qDebug() << QTime().currentTime().toString();
    qDebug() << "Left Button clicked!";
    emit leftButtonClicked();
}

void GameControllerWidget::rightClicked()
{
    qDebug() << QTime().currentTime().toString();
    qDebug() << "Right Button clicked!";
    emit rightButtonClicked();
}
