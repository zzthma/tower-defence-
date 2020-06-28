#ifndef ROUTE_H
#define ROUTE_H

#include <QPoint>
#include <QPainter>

class route
{
public:
    route(QPoint position);
    void setnextroute(route *nextpoint);
    route* nextroute() const;
    const QPoint pos() const;

    void draw(QPainter *painter) const;

private:
    const QPoint		_pos;
    route *			_nextroute;
};

#endif // ROUTE_H
