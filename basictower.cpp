#include "basictower.h"
#include "enemy1.h"
#include "dialog.h"
#include "attack.h"
#include "effect.h"
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QVector2D>
#include <QtMath>

const QSize basictower::ms_fixedSize(64, 64);

basictower::basictower(QPoint pos, Dialog *game,const QPixmap &sprite)
    : _attacking(false)
    , _rotationsprite(0.0)
    , _chooseenemy(NULL)
    , _game(game)
    , _pos(pos)
    ,_sprite(sprite)
{
    _fireRateTimer = new QTimer(this);
}

basictower::~basictower()
{
    delete _fireRateTimer;
    _fireRateTimer = NULL;
}

void basictower::checkEnemyInRange()
{
}

void basictower::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::white);
    //painter->drawEllipse(_pos, _attackrange, _attackrange);

    static const QPoint offsetPoint(-ms_fixedSize.width() / 2, -ms_fixedSize.height() / 2);

    painter->translate(_pos);
    painter->rotate(_rotationsprite);
    painter->drawPixmap(offsetPoint, _sprite);
    painter->restore();
}


void basictower::attackEnemy()
{

}

void basictower::chooseEnemyForAttack(enemy1 *enemy)
{
    _chooseenemy = enemy;
    attackEnemy();
    _chooseenemy->getAttacked(this);
}

void basictower::shootWeapon()
{
}

void basictower::targetKilled()
{
    if (_chooseenemy)
        _chooseenemy = NULL;

    _fireRateTimer->stop();
    _rotationsprite = 0.0;
}

void basictower::lostSightOfEnemy()
{
    _chooseenemy->gotLostSight(this);
    if (_chooseenemy)
        _chooseenemy = NULL;

    _fireRateTimer->stop();
    _rotationsprite = 0.0;
}

tower1::tower1(QPoint pos, Dialog *game, const QPixmap &sprite):
    basictower(pos,game,sprite)
   ,_attackrange(150)
   ,_damage(15)
   ,_firerate(1500)
   {
    connect(_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}
tower1::~tower1(){
   this->~basictower();
}

void tower1::checkEnemyInRange()
{
    if (_chooseenemy)
    {
        QVector2D normalized(_chooseenemy->pos() - _pos);
        normalized.normalize();
        _rotationsprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;

        if (!collision(_pos, _attackrange, _chooseenemy->pos(), 1))
            lostSightOfEnemy();
    }
    else
    {
        QList<enemy1 *> enemyList = _game->enemyList();
        foreach (enemy1 *enemy, enemyList)
        {
            if (collision(_pos, _attackrange, enemy->pos(), 1))
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}

void tower1::upgrade(){
    this->_attackrange+=50;
    this->_damage+=15;
    this->_firerate+=100;
}

void tower1::attackEnemy(){
    _fireRateTimer->start(_firerate);
}

void tower1::shootWeapon()
{
    attack *Attack = new attack(_pos, _chooseenemy->pos(), _damage, _chooseenemy, _game,QPixmap(":/attack.png"));
    Attack->move();
    _game->addbullet(Attack);
}

tower2::tower2(QPoint pos, Dialog *game, const QPixmap &sprite):
    basictower(pos,game,sprite)
   ,_attackrange(180)
   ,_damage(30)
   ,_firerate(1600)
   {
    connect(_fireRateTimer, SIGNAL(timeout()), this, SLOT(shootWeapon()));
}
tower2::~tower2(){
   this->~basictower();
}

void tower2::checkEnemyInRange()
{
    if (_chooseenemy)
    {
        QVector2D normalized(_chooseenemy->pos() - _pos);
        normalized.normalize();
        _rotationsprite = qRadiansToDegrees(qAtan2(normalized.y(), normalized.x())) - 90;

        if (!collision(_pos, _attackrange, _chooseenemy->pos(), 1))
            lostSightOfEnemy();
    }
    else
    {
        QList<enemy1 *> enemyList = _game->enemyList();
        foreach (enemy1 *enemy, enemyList)
        {
            if (collision(_pos, _attackrange, enemy->pos(), 1))
            {
                chooseEnemyForAttack(enemy);
                break;
            }
        }
    }
}
void tower2::upgrade(){
    this->_attackrange+=50;
    this->_damage+=15;
    this->_firerate+=100;
}

void tower2::attackEnemy(){
    _fireRateTimer->start(_firerate);
}

void tower2::shootWeapon()
{
    attack *Attack = new attack(_pos, _chooseenemy->pos(), _damage, _chooseenemy, _game,QPixmap(":/attack2.png"));
    Attack->move();
    _game->addbullet(Attack);
}
