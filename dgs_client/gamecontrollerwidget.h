#ifndef GAMECONTROLLERWIDGET_H
#define GAMECONTROLLERWIDGET_H

#include <QWidget>
#include <QPushButton>
class GameControllerWidget : public QWidget
{
    Q_OBJECT
public:
    GameControllerWidget(QWidget *parent = 0);
signals:
    void leftButtonClicked();
    void rightButtonClicked();
public slots:
    void enableButtons();
    void disableButtons();
    void leftClicked();
    void rightClicked();
private:
    QPushButton *leftButton;
    QPushButton *rightButton;
};

#endif // GAMECONTROLLERWIDGET_H
