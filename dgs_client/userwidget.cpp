#include "userwidget.h"
#include <QLabel>

UserWidget::UserWidget(QWidget *parent) : QWidget(parent),
    m_Box(new QGroupBox("Players")),
    m_VLayout(new QVBoxLayout()),
    m_GLayout(new QGridLayout()),
    m_P0Score(0),
    m_P1Score(0),
    m_P2Score(0),
    m_P3Score(0),
    label0(new QLabel(tr("P0:"))),
    label1(new QLabel(tr("P1: "))),
    label2(new QLabel(tr("P2: "))),
    label3(new QLabel(tr("P3: ")))
{
    QVBoxLayout *layout = new QVBoxLayout;
    //QGridLayout *layout = new QGridLayout;

    connect(this, &UserWidget::setP0NameScore, label0, &QLabel::setText);
    connect(this, &UserWidget::setP1NameScore, label1, &QLabel::setText);
    connect(this, &UserWidget::setP2NameScore, label2, &QLabel::setText);
    connect(this, &UserWidget::setP3NameScore, label3, &QLabel::setText);
    m_GLayout->addWidget(label0, 0, 0);
    m_GLayout->addWidget(label1, 1, 0);
    m_GLayout->addWidget(label2, 2, 0);
    m_GLayout->addWidget(label3, 3, 0);

    m_Box->setLayout(m_GLayout);
    //m_Box->setLayout(m_VLayout);
    layout->addWidget(m_Box);
    this->setLayout(layout);
}

void UserWidget::showPlayer(QUuid id, QString playerNum)
{
    Q_UNUSED(id);
    Q_UNUSED(playerNum);
    /*if(!m_UserList.contains(id))
        return;
    QString name = m_UserList.value(id);
    QLabel *label = new QLabel(playerNum + name + " 0");
    m_GLayout->addWidget(label, m_GLayout->rowCount(), 0, 1, -1);*/
}

void UserWidget::updateList(QUuid id, QString name)
{
    Q_UNUSED(id);
    Q_UNUSED(name);
    //m_UserList.insert(id, name);
}



void UserWidget::eraseList()
{
    label0->setText("P0: ");
    label1->setText("P1: ");
    label2->setText("P2: ");
    label3->setText("P3: ");
}

void UserWidget::showList()
{
    /*QVBoxLayout *layout = new QVBoxLayout;
    for(int i = 0; i < m_Users.size(); i++){
        QLabel *label = new QLabel(m_Users[i] + " 0");
        //m_VLayout->addWidget(label);
        m_GLayout->addWidget(label, m_GLayout->rowCount(), 0, 1, -1);
        //m_GLayout->addWidget(score, m_GLayout->rowCount() - 1, 1, 1, -1);
    }
    //m_Box->setLayout(m_VLayout);
    m_Box->setLayout(m_GLayout);
    this->setLayout(layout);*/
}

void UserWidget::updateScore(int score, int userNum)
{
    Q_UNUSED(score);
    Q_UNUSED(userNum);
    /*QString scoreTxt = QString::number(score);
    if(userNum == 0){
        QLabel *label = (QLabel *)m_GLayout->itemAt(0)->widget();
        QString x = label->text();
        x.append(" " + QString::number(score));
        label->setText(x);
        //QLabel *label = m_GLayout->findChild<QLabel *>("label");
    }*/
}
