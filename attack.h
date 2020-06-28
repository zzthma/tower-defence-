#ifndef ATTACK_H
#define ATTACK_H

#include <QObject>
#include <QPoint>
#include <QSize>
#include <QPixmap>

class QPainter;
class enemy1;
class Dialog;

class attack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint _currentPos READ currentPos WRITE setCurrentPos)

public:
    attack(QPoint startPos, QPoint targetPoint, int damage, enemy1 *target,
           Dialog *game, const QPixmap &sprite);

    void draw(QPainter *painter) const;
    void move();
    void setCurrentPos(QPoint pos);
    QPoint currentPos() const;

private slots:
    void hitTarget();

private:
    const QPoint	_startPos;
    const QPoint	_targetPos;
    const QPixmap	_sprite;
    QPoint			_currentPos;
    enemy1 *			_target;
    Dialog *	_game;
    int				_damage;

    static const QSize ms_fixedSize;
};


#endif // ATTACK_H
