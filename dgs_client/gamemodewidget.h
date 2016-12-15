#ifndef GAMEMODEWIDGET_H
#define GAMEMODEWIDGET_H

#include <QWidget>
#include <QPushButton>
class GameModeWidget : public QWidget
{
    Q_OBJECT
public:
    GameModeWidget(QWidget *parent = 0);

signals:
    void joinButtonClicked();
    void startButtonClicked();
    void stopButtonClicked();
public slots:
    void enableJoinButton();
    void disableJoinButton();
    void enableSSButtons();
    void joinClicked();
    void startClicked();
    void stopClicked();
private:
    QPushButton *joinButton;
    QPushButton *startButton;
    QPushButton *stopButton;
};

#endif // GAMEMODEWIDGET_H
