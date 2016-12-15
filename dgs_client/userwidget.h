#ifndef USERWIDGET_H
#define USERWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QMap>
#include <QUuid>
#include <QLabel>

class UserWidget : public QWidget
{
    Q_OBJECT
public:
    UserWidget(QWidget *parent = 0);
signals:
    void setP0NameScore(QString nameAndScore);
    void setP1NameScore(QString nameAndScore);
    void setP2NameScore(QString nameAndScore);
    void setP3NameScore(QString nameAndScore);
public slots:
    void updateList(QUuid id, QString name);
    void eraseList();
    void showPlayer(QUuid id, QString playerNum);
    void updateScore(int score, int userNum);
private:
    void showList();

    QMap<QUuid, QString> m_UserList;

    QList<QString> m_Users;
    QGroupBox *m_Box;
    QVBoxLayout *m_VLayout;

    QGridLayout *m_GLayout;

    QLabel *label0;
    QLabel *label1;
    QLabel *label2;
    QLabel *label3;


    int m_P0Score;
    int m_P1Score;
    int m_P2Score;
    int m_P3Score;
};

#endif // USERWIDGET_H
