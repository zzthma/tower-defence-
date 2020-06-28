#include "route.h"
#include <QPainter>
#include <QColor>

route::route(QPoint position): _pos(position)
  , _nextroute(NULL)
{
}

void route::setnextroute(route *nextpoint)
{
    _nextroute = nextpoint;
}

route* route::nextroute() const
{
    return _nextroute;
}

const QPoint route::pos() const
{
    return _pos;
}

void route::draw(QPainter *painter) const
{
    painter->save();
    painter->setPen(Qt::green);
    painter->drawEllipse(_pos, 6, 6);
    painter->drawEllipse(_pos, 2, 2);

    if (_nextroute)
    painter->restore();
}
