#ifndef BASICTOWER1_H
#define BASICTOWER1_H
#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include <QPainter>
#include <QTime>
class enemy1;
class Dialog;


class basictower : public QObject
{
    Q_OBJECT
public:
    basictower(QPoint pos, Dialog *game,const QPixmap &sprite);
    ~basictower();

    void draw(QPainter *painter) const;
    void targetKilled();
    void chooseEnemyForAttack(enemy1 *enemy);
    void lostSightOfEnemy();
    virtual void attackEnemy();
    virtual void checkEnemyInRange();


    bool			_attacking;
    qreal			_rotationsprite;

    enemy1 *			_chooseenemy;
    Dialog *	_game;
    QTimer *		_fireRateTimer;

    const QPoint	_pos;
    const QPixmap	_sprite;

    static const QSize ms_fixedSize;
public slots:
    virtual void shootWeapon();
signals:

};

class tower1:public basictower
{
    Q_OBJECT
public:
    tower1(QPoint pos, Dialog *game,const QPixmap &sprite);
    ~tower1();
    void attackEnemy();
    void checkEnemyInRange();
    void upgrade();
public slots:
    void shootWeapon();
private:
    int				_attackrange;
    int				_damage;
    int				_firerate;


};

class tower2:public basictower
{
    Q_OBJECT
public:
    tower2(QPoint pos, Dialog *game,const QPixmap &sprite);
    ~tower2();
    void attackEnemy();
    void checkEnemyInRange();
    void upgrade();
public slots:
    void shootWeapon();
private:
    int				_attackrange;
    int				_damage;
    int				_firerate;

};

#endif // BASICTOWER_H
