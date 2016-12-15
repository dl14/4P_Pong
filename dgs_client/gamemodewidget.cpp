#include "gamemodewidget.h"
#include <QHBoxLayout>
GameModeWidget::GameModeWidget(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *hLayout = new QHBoxLayout;
    joinButton = new QPushButton(tr("Join"));
    startButton = new QPushButton(tr("Start"));
    stopButton = new QPushButton(tr("Stop"));
    joinButton->setEnabled(false);
    startButton->setEnabled(false);
    stopButton->setEnabled(false);
    connect(startButton, &QAbstractButton::clicked, this, &GameModeWidget::startClicked);
    connect(stopButton, &QAbstractButton::clicked, this, &GameModeWidget::stopClicked);
    connect(joinButton, &QAbstractButton::clicked, this, &GameModeWidget::joinClicked);
    hLayout->addWidget(joinButton);
    hLayout->addWidget(startButton);
    hLayout->addWidget(stopButton);
    this->setLayout(hLayout);
}

void GameModeWidget::enableJoinButton()
{
    joinButton->setEnabled(true);
}

void GameModeWidget::enableSSButtons()
{
    startButton->setEnabled(true);
    stopButton->setEnabled(false);
}

void GameModeWidget::joinClicked()
{
    startButton->setEnabled(true);
    emit joinButtonClicked();
}

void GameModeWidget::startClicked()
{
    startButton->setEnabled(false);
    stopButton->setEnabled(true);
    emit startButtonClicked();
}

void GameModeWidget::stopClicked()
{
    stopButton->setEnabled(false);
    startButton->setEnabled(true);
    emit stopButtonClicked();
}

void GameModeWidget::disableJoinButton()
{
    joinButton->setEnabled(false);
}
