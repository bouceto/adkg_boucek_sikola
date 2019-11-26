#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QPointF>
#include <QPolygonF>
#include <vector>


class Algorithms
{
public:
    Algorithms();
    static int getPointLinePosition(QPointF &q,QPointF &p1,QPointF &p2);
    static double getAngle2Vectors(QPointF &p1,QPointF &p2,QPointF &p3,QPointF &p4);
    static int positionPointPolygonWinding(QPointF &q, std::vector<QPointF> &polygons);
    static int positionPointPolygonRayCrossing(QPointF &q, std::vector<QPointF> &polygons);
};

#endif // ALGORITHMS_H
