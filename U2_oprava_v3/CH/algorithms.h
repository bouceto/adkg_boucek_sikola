#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QtGui>

struct QPointO
{
    QPoint p;
    double omega;
    double length;
};

class Algorithms
{
public:
    Algorithms();
    static double getPointLineDistance(QPoint &q, QPoint &p1, QPoint &p2);
    static int getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2);
    static double getAngle2Vectors(QPoint &p1,QPoint &p2,QPoint &p3,QPoint &p4);
    static double getPointToPointDistance(QPoint &p1, QPoint &p2);
    static QPolygon jarvisScan(QPolygon &points);
    static QPolygon qHull(QPolygon &points);
    static void qh(int s, int e, QPolygon &points, QPolygon &ch);
    static QPolygon sweepLine(QPolygon &points);
    static QPolygon grahamScan(QPolygon &points);
    static QPolygon strictlyConvexHull(QPolygon &ch);
    static QPolygon removeDuplicite(QPolygon &points);

};

#endif // ALGORITHMS_H
