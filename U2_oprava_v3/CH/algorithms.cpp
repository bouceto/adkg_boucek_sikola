#include "algorithms.h"
#include <cmath>
#include "sortbyy.h"
#include "sortbyx.h"
#include <QPolygon>
#include "draw.h"
#include "sortbyangle.h"

Algorithms::Algorithms()
{

}

double Algorithms::getPointToPointDistance(QPoint &p1, QPoint &p2)
{
    //Calculate distance between two points
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double dist = sqrt(dx * dx + dy * dy);

    return dist;
}

double Algorithms::getPointLineDistance(QPoint &q, QPoint &p1, QPoint &p2)
{
    //Calculate distance between point and line
    double numerator = q.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - q.y()) + p2.x() * (q.y() - p1.y());
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double denumenator = sqrt(dx * dx + dy * dy);
    double dist = fabs(numerator)/denumenator;

    return dist;
}

int Algorithms::getPointLinePosition(QPoint &q,QPoint &p1,QPoint &p2)
{
    //Analyze point and line position
    //1 point in the left half plane
    //0 point in the right half plane
    //-1 point on the line
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = q.x() - p1.x();
    double vy = q.y() - p1.y();

    double t = ux * vy - uy * vx;

    //Point in the left half plane
    if (t > 0)
        return 1;

    //Point in the right half plane
    if (t < 0)
        return 0;

    //Point on the line
    return -1;
    }

double Algorithms::getAngle2Vectors(QPoint &p1, QPoint &p2, QPoint &p3, QPoint &p4)
{
    //Calculate angle betwen two vectors.
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();

    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();

    //Norms
    double nu = sqrt(ux * ux + uy * uy);
    double nv = sqrt(vx * vx + vy * vy);

    //Dot product
    double scalar = ux * vx + uy * vy;

    double angle = fabs(acos(scalar/(nu*nv)));
    return angle;
}

QPolygon Algorithms::jarvisScan(QPolygon &points)
{
    //Convex hull
    QPolygon ch;

    //Sort points by Y
    std::sort(points.begin(), points.end(), SortbyY());
    QPoint q=points[0];
    QPoint r(q.x()-1,q.y());

    //Initialize points pj, pjj
    QPoint pj = q;
    QPoint pjj = r;

    //Add to CH
    ch.push_back(q);

    //Find points of CH
    do
    {
        double om_max = 0;
        int i_max = -1;

        //Find point of CH
        for (int i = 0; i < points.size(); i++)
        {
            double omega = getAngle2Vectors(pj, pjj, pj, points[i]);

            //Actualize maximum.
            if (omega > om_max)
            {
                om_max = omega;
                i_max = i;
            }
        }

        //Add point to the convex hull
        ch.push_back(points[i_max]);

        //Change index
        pjj = pj;
        pj = points[i_max];

    } while (!(pj == q));

    return ch;

}

QPolygon Algorithms::qHull(QPolygon &points)
{
    //Convex Hull by quick hull method
    QPolygon ch;
    QPolygon upoints, lpoints;

    //Sort by X.
    std::sort(points.begin(), points.end(), SortbyY());
    QPoint q1 = points[0];
    QPoint q3 = points.back();

    //Add q1, q3 to upoints, lpoints
    upoints.push_back(q1);
    upoints.push_back(q3);

    lpoints.push_back(q1);
    lpoints.push_back(q3);

    // Split to upoints / lpoints
    for (int i = 0; i < points.size(); i++){

        //Add to the upper part
        if (getPointLinePosition(points[i], q1, q3) == 1)
            upoints.push_back(points[i]);

        //Add to the lower part
        else if (getPointLinePosition(points[i], q1, q3) == 0)
            lpoints.push_back(points[i]);
    }

    //Call recursive function
    ch.push_back(q3);
    qh(1, 0, upoints, ch);
    ch.push_back(q1);
    qh(0, 1, lpoints, ch);

    return ch;
}

void Algorithms::qh(int s, int e, QPolygon &points, QPolygon &ch){

    //Recursive procedure
    int i_max = -1;
    double d_max = 0;

    //Browse all points
    for (int i = 2; i < points.size(); i++)
    {
       //Is the point in the right half plane?
        if (getPointLinePosition(points[i], points[s], points[e]) == 0)
        {
            double d = getPointLineDistance(points[i], points[s], points[e]);

            //Actualization of d_max
            if (d > d_max){
                d_max = d;
                i_max = i;
            }
        }
    }

    //Suitable point has been found
    if (i_max != -1)
    {
        qh(s, i_max, points, ch);
        ch.push_back(points[i_max]);
        qh(i_max, e, points, ch);
    }

}

QPolygon Algorithms::sweepLine(QPolygon &points)
{
    //Increment method, sweep line
    QPolygon ch;

    int m = points.size();

    //List of predecessors
    std::vector<int> p(m);

    //List of successors
    std::vector<int> n(m);

    //Sort points by X
    std::sort(points.begin(),points.end(),SortbyX());

    // Create initial aproximation
    n[0] = 1;
    n[1] = 0;
    p[0] = 1;
    p[1] = 0;

    // Process all points according to x coordinates
    for (int i = 2; i < m; i++)
    {
        //Point i lies in the upper half plane
        if (points[i].y() > points[i-1].y()){

            //Link i and its predecessor and successor
            p[i] = i-1;
            n[i] = n[i-1];
        }

        //Point i lies in the lower half plane
        else
        {
            //Link i and its predecessor and successor
            p[i] = p[i-1];
            n[i] = i - 1;
        }

        //Link predecessor and successor and i
        p[n[i]] = i;
        n[p[i]] = i;

        //Fix upper tangent
        while (getPointLinePosition(points[n[n[i]]], points[i], points[n[i]]) == 0)
        {
            //Change predecessor and successor
            p[n[n[i]]] = i;
            n[i] = n[n[i]];
        }

        //Fix lower tangent
        while (getPointLinePosition(points[p[p[i]]], points[i], points[p[i]]) == 1)
        {
            //Change predecessor and successor
            n[p[p[i]]] = i;
            p[i] = p[p[i]];
        }
    }

    //Convert successors to ch
    ch.push_back(points[0]);

    //Second point of CH
    int index = n[0];

    //Repeat until the first point is found
    while (index != 0)
    {
        //Add to the polygon
        ch.push_back(points[index]);

        //Find its successor
        index = n[index];
    }

    return ch;
}


QPolygon Algorithms::grahamScan(QPolygon &points)
{
    //Convex hull
    QPolygon ch;

    //Sort points by Y
    std::sort(points.begin(), points.end(), SortbyY());

    //Finding pivot q
    QPoint q = points[0];

    //Add pivot to convex hull
    ch.push_back(q);

    //Get point r needed for calculating angles
    QPoint r(q.x()+1,q.y());

    //Points sorted by omega
    std::vector<QPointO> pointso;
    QPointO pointq;
    pointq.p.setX(points[0].x());
    pointq.p.setY(points[0].y());
    pointq.omega = 0;
    pointq.length = 0;
    pointso.push_back(pointq);

    int n = points.size();

    //Auxiliary vector for points with same omega
    std::vector<int> index_vec;


    //Sort by omega
    for (int i = 1; i < n; i++)
    {
        double omega = getAngle2Vectors(q, r, q, points[i]);
        double dist = getPointToPointDistance(q, points[i]);
        QPointO point;
        point.p.setX(points[i].x());
        point.p.setY(points[i].y());
        point.omega = omega;
        point.length = dist;
        pointso.push_back(point);
    }

    std::sort(pointso.begin(), pointso.end(), sortbyangle());
    points.clear();
    points.push_back(q);
    std::vector<int> ipot;

    //Remove points with same omega
    for (int i = 1; i < pointso.size(); i++)
    {
        if (fabs(pointso[i].omega - pointso[(i+1)%n].omega) < 10e-6)
        {
            ipot.push_back(i);
        }
        else if (!((fabs(pointso[i].omega - pointso[(i+1)%n].omega) < 10e-6)) && (ipot.empty()))
        {
            QPoint point;
            point.setX(pointso[i].p.x());
            point.setY(pointso[i].p.y());
            points.push_back(point);
        }
        else
        {
            QPoint point;
            point.setX(pointso[ipot.back()+1].p.x());
            point.setY(pointso[ipot.back()+1].p.y());
            points.push_back(point);
            ipot.clear();

        }
    }

    /*
    QPolygon pointsop;
    points.clear();
    for (int i = 0; i < n; i++)
    {
        QPoint point;
        point.setX(pointso[i].p.x());
        point.setY(pointso[i].p.y());
        points.push_back(point);
        qDebug()<< i;
    }
*/


    //Add point with minimum angle to convex hull
    ch.push_back(points[1]);
    int j = 2;

    //Process all points
    while (j < points.size())
    {
        //Check if the new point is in the left half plane
        if (getPointLinePosition(points[j], ch[ch.size() - 2], ch.back()) == 1)
        {
            ch.push_back(points[j]);
            j += 1;
        }
        else
            ch.removeLast();
    }

    return ch;
}

QPolygon Algorithms::strictlyConvexHull(QPolygon &ch)
{
    //Strictly Convex Hull
    QPolygon strict_ch;

    //Check if the first and the last points are the same in convex hull
    if (ch[0] == ch.back())
        ch.removeLast();

    int n = ch.size();

    //Process all points in convex hull
    for (int i = 0; i < n; i++)
    {
        //Check if three points are not collinear
        if (getPointLinePosition(ch[(i+2)%n], ch[i], ch[(i+1)%n]) != -1)
            strict_ch.push_back(ch[(i+1)%n]);
    }

    return strict_ch;
}

QPolygon Algorithms::removeDuplicite(QPolygon &points)
{
    QPolygon points_s;
    std::sort(points.begin(), points.end(), SortbyX());
    int n = points.size();
    int i = 0;
    while (i < n)
    {
        int x_diff = points[i].x();
        while ((x_diff == points[i%n].x()) && (i < n))
        {
            if ((points[i].y() != points[(i+1)%n].y()))
                points_s.push_back(points[i]);
            i++;
        }
    }
    return points_s;
}

