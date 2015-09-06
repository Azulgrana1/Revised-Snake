#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>

#include "gamecontroller.h"
#include "food.h"
#include "snake.h"
#include "constants.h"

GameController::GameController(QGraphicsScene &scene, QObject *parent) :
    QObject(parent),
    scene(scene),
    snake(new Snake(*this))
{
    timer.start( 1000/33 );
    foodTimer.start(FoodTime);
    foodOpqTmr.start(5);
    connect(&foodTimer,SIGNAL(timeout()),this,SLOT(deleteFood()));
    connect(&foodOpqTmr,SIGNAL(timeout()),this,SLOT(setFoodOpacity()));

    scene.addItem(snake);
    scene.installEventFilter(this);

    resume();
}

GameController::~GameController()
{
}

void GameController::snakeAteFood(Snake *snake, Food *food)
{
    scene.removeItem(food);
    delete food;

    addNewFood();
}

void GameController::snakeHitWall(Snake *snake, Wall *wall)
{
}

void GameController::snakeAteItself(Snake *snake)
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::handleKeyPressed(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:
            if(snake->getDirection()!=Snake::MoveRight)
                snake->setMoveDirection(Snake::MoveLeft);
            break;
        case Qt::Key_Right:
            if(snake->getDirection()!=Snake::MoveLeft)
                snake->setMoveDirection(Snake::MoveRight);
            break;
        case Qt::Key_Up:
            if(snake->getDirection()!=Snake::MoveDown)
            snake->setMoveDirection(Snake::MoveUp);
            break;
        case Qt::Key_Down:
            if(snake->getDirection()!=Snake::MoveUp)
            snake->setMoveDirection(Snake::MoveDown);
            break;
    }
}

void GameController::addNewFood()
{
    int x, y;

    do {
        x = (int) (qrand() % 100) / 10;
        y = (int) (qrand() % 100) / 10;

        x *= 10;
        y *= 10;
    } while (snake->shape().contains(snake->mapFromScene(QPointF(x + 5, y + 5))));

    Food *food = new Food(x , y);
    scene.addItem(food);
    foodTimer.start(FoodTime);
}

void GameController::gameOver()
{
    scene.clear();

    snake = new Snake(*this);
    scene.addItem(snake);
    addNewFood();
}

void GameController::deleteFood()
{
    QList<QGraphicsItem*> Items=scene.items();
    foreach(QGraphicsItem* Item, Items)
    {
        if(Item->data(GD_Type)==GO_Food)
            scene.removeItem(Item);
    }
    addNewFood();
}

void GameController::setFoodOpacity()
{
    QList<QGraphicsItem*> Items=scene.items();
    foreach(QGraphicsItem* Item, Items)
    {
        if(Item->data(GD_Type)==GO_Food)
            Item->setOpacity((qreal)(foodTimer.remainingTime())/FoodTime);
    }
}

void GameController::pause()
{
    disconnect(&timer, SIGNAL(timeout()),
               &scene, SLOT(advance()));
}

void GameController::resume()
{
    connect(&timer, SIGNAL(timeout()),
            &scene, SLOT(advance()));
}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}
