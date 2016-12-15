//dgs client:
//GSM game state manager keeps track of where you're at in overall game
//handles connections and emits signals to say where you're at


#include <QApplication>
#include <QtWidgets>
#include <QLayout>
#include <QGridLayout>
#include <QHBoxLayout>

#include "client.h"
#include "graphicswidget.h"
#include "legendwidget.h"
#include "game.h"
#include "userwidget.h"
#include "gamecontrollerwidget.h"
#include "gamemodewidget.h"

#include "logger.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Logger *logger = new Logger();
    logger->init();

    Client *client = new Client();
    UserWidget *users = new UserWidget();
    QRectF rect(QPointF(-250, -250), QPointF(250, 250));
    GraphicsWidget *graphic = new GraphicsWidget(rect);
    LegendWidget *legend = new LegendWidget();
    GameControllerWidget *gcWidget = new GameControllerWidget;
    GameModeWidget *gmWidget = new GameModeWidget;
    Game game(client, graphic, users, gcWidget, gmWidget, logger);
    Q_UNUSED(game);
    QWidget *buttonWidgets = new QWidget();
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(gcWidget);
    hLayout->addWidget(gmWidget);
    buttonWidgets->setLayout(hLayout);

    QWidget *gAnduWidget = new QWidget();
    QHBoxLayout *hLayout2 = new QHBoxLayout;
    hLayout2->addWidget(graphic);
    hLayout2->addWidget(users);
    hLayout2->addWidget(legend);
    gAnduWidget->setLayout(hLayout2);

    QLayout *oldLayout = client->layout();
    QGridLayout *gLayout = qobject_cast<QGridLayout *> (oldLayout);

    if (gLayout == NULL) {
        //oldLayout->addWidget(graphic);
        oldLayout->addWidget(gAnduWidget);
        oldLayout->addWidget(buttonWidgets);
        //oldLayout->addWidget(gcWidget);
        //oldLayout->addWidget(gmWidget);
    } else {
        //gLayout->addWidget(graphic, gLayout->rowCount(),0, 1, -1);
        gLayout->addWidget(gAnduWidget, gLayout->rowCount(), 0, 1, -1);
        gLayout->addWidget(buttonWidgets, gLayout->rowCount(), 0, 1, -1);
        //gLayout->addWidget(gcWidget, gLayout->rowCount(), 0, 1, -1);
        //gLayout->addWidget(gmWidget, gLayout->rowCount(), 0, 1, -1);
    }
    client->show();
    return a.exec();
}
