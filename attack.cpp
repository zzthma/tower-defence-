#include "attack.h"
#include "enemy1.h"
#include "dialog.h"
#include <QPainter>
#include <QPropertyAnimation>

const QSize attack::ms_fixedSize(10, 10);

attack::attack(QPoint startPos, QPoint targetPoint, int damage, enemy1 *target,
               Dialog *game, const QPixmap &sprite)
    : _startPos(startPos)
    , _targetPos(targetPoint)
    , _sprite(sprite)
    , _currentPos(startPos)
    , _target(target)
    , _game(game)
    , _damage(damage)
{
}

void attack::draw(QPainter *painter) const
{
    painter->drawPixmap(_currentPos, _sprite);
}

void attack::move()
{
    static const int duration = 100;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "_currentPos");
    animation->setDuration(duration);
    animation->setStartValue(_startPos);
    animation->setEndValue(_targetPos);
    connect(animation, SIGNAL(finished()), this, SLOT(hitTarget()));

    animation->start();
}

void attack::hitTarget()
{

    if (_game->enemyList().indexOf(_target) != -1)
        _target->getDamage(_damage);
    _game->removedbullet(this);
}

void attack::setCurrentPos(QPoint pos)
{
    _currentPos = pos;
}

QPoint attack::currentPos() const
{
    return _currentPos;
}

