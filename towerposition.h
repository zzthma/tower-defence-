#ifndef TOWERPOSITION_H
#define TOWERPOSITION_H

#include <QPoint>
#include <QSize>
#include <QPixmap>
#include <QPainter>

class towerposition
{
public:
    towerposition(QPoint pos, const QPixmap &sprite = QPixmap(":/2.png"));

    void setHasTower(bool ht);
    bool hasTower() const;
    const QPoint centerPos() const;
    bool containPoint(const QPoint &pos) const;
    void telltowergrade(bool td);
    void telltowerkind(bool tk);
    bool towerkind() const;
    bool towergrade() const;

    void draw(QPainter *painter) const;

private:
    bool        _tk;
    bool        _td;
    bool		_ht;
    QPoint		_pos;
    QPixmap		_sprite;

    static const QSize ms_fixedSize;
};


#endif // TOWERPOSITION_H

