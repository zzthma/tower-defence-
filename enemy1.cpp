#include "enemy1.h"
#include "route.h"
#include "basictower.h"
#include "effect.h"
#include "dialog.h"
#include <QPainter>
#include <QColor>
#include <QDebug>
#include <QMatrix>
#include <QVector2D>
#include <QtMath>

static const int Health_Bar_Width = 20;
const QSize enemy1::ms_fixedSize(45, 45);

enemy1::enemy1(route *startroute, Dialog *game, const QPixmap &sprite,qreal walkspeed,int maxHP )
    :QObject(0)
  , _active(false)
  , _maxHP(maxHP)
  , _currentHP(maxHP)
  , _walkspeed(walkspeed)
  , _rotationSprite(0.0)
  , _pos(startroute->pos())
  , _destination(startroute->nextroute())
  , _game(game)
  , _sprite(sprite)
{

}

enemy1::~enemy1()
{
    _attackedTowersList.clear();
    _destination = NULL;
    _game = NULL;
}

void enemy1::doActivate()
{
    _active = true;
}

void enemy1::move()
{
    if (!_active)
        return;

    if (collision(_pos, 1, _destination->pos(), 1))
    {
        if (_destination->nextroute())
        {
            _pos = _destination->pos();
            _destination = _destination->nextroute();
        }
        else
        {
            _game->getHPdamage();
            _game->removedenemy(this);
            return;
        }
    }
    QPoint targetPoint = _destination->pos();


    qreal movementSpeed = _walkspeed;
    QVector2D normalized(targetPoint - _pos);
    normalized.normalize();
    _pos = _pos + normalized.toPoint() * movementSpeed;
    _rotationSprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) + 180;
}

void enemy1::draw(QPainter *painter) const
{
    if (!_active)
        return;

    painter->save();

    QPoint healthBarPoint = _pos + QPoint(-Health_Bar_Width / 2 - 5, 2*(-ms_fixedSize.height())/3);

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(healthBarPoint, QSize(Health_Bar_Width, 2));
    painter->drawRect(healthBarBackRect);

    painter->setBrush(Qt::green);
    QRect healthBarRect(healthBarPoint, QSize((double)_currentHP / _maxHP * Health_Bar_Width, 2));
    painter->drawRect(healthBarRect);

    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);
    painter->translate(_pos);
    painter->rotate(_rotationSprite);
    painter->drawPixmap(offsetPoint, _sprite);

    painter->restore();
}

void enemy1::getRemoved()
{
    if (_attackedTowersList.empty())
        return;

    foreach (basictower *attacker, _attackedTowersList)
        attacker->targetKilled();
    _game->removedenemy(this);
}

void enemy1::getDamage(int damage)
{
    _currentHP -= damage;


    if (_currentHP <= 0)
    {
        _game->awardgold(100);
        getRemoved();
    }
}

void enemy1::getAttacked(basictower *attacker)
{
    _attackedTowersList.push_back(attacker);
}


void enemy1::gotLostSight(basictower *attacker)
{
    _attackedTowersList.removeOne(attacker);
}

QPoint enemy1::pos() const
{
    return _pos;
}
