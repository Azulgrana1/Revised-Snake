#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QRectF>

class GameController;

class Snake : public QGraphicsItem
{
public:
    enum Direction {
        NoMove,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown
    };

    Snake(GameController & controller);

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    void setMoveDirection(Direction direction);
    Direction getDirection(){return moveDirection;}

    void increaseSpeed();//提高速度，对应缩短stepInterval
    void decreaseSpeed();//降低速度，对应提高stepInterval

protected:
    void advance(int step);

private:
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    void handleCollisions();

    QPointF        head;
    int            growing;
    int            stepInterval;//每前进一步的时间间隔
    QList<QPointF> tail;
    int            tickCounter;
    Direction      moveDirection;
    GameController &controller;
};

#endif // SNAKE_H
