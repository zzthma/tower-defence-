#ifndef ENEMY1_H
#define ENEMY1_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QObject>
#include <QPainter>

class basictower;
class route;
class Dialog;

class enemy1 : public QObject
{
    Q_OBJECT
public:
    explicit enemy1(QObject *parent = nullptr);
    enemy1(route *startroute, Dialog *game, const QPixmap &sprite,qreal walkspeed,int maxHP);
    ~enemy1();

    void draw(QPainter *painter) const;
    void move();
    void getDamage(int damage);
    void getRemoved();
    void getAttacked(basictower *attacker);
    void gotLostSight(basictower *attacker);
    QPoint pos() const;

public slots:
    void doActivate();

private:
    bool			_active;
    int				_maxHP;
    int				_currentHP;
    qreal			_walkspeed;
    qreal			_rotationSprite;

    QPoint			_pos;
    route *		_destination;
    Dialog *	_game;
    QList<basictower *>	_attackedTowersList;

    const QPixmap	_sprite;
    static const QSize ms_fixedSize;

signals:

};

#endif // ENEMY1_H
