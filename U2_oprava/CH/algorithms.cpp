#include "algorithms.h"
#include <cmath>
#include "sortbyy.h"
#include "sortbyx.h"
#include <QPolygon>
#include "draw.h"

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

    //Accuracy for comparing double values
    double eps = 1e-6;

    //Points sorted by omega
    QPolygon pbo;

    //Add pivot to points sorted by omega
    pbo.push_back(q);

    //Initialization of omega_min, i_min and dist_max
    double omega_min = 2 * M_PI;
    int i_min = -1;
    double dist_max = 0;

    //Remove pivot from points
    points.removeFirst();

    int n = points.size();

    //Auxiliary vector for points with same omega
    std::vector<int> index_vec;

    //Sort by omega
    while (n != 0)
    {
        for (int i = 0; i < n; i++)
        {
            double omega = getAngle2Vectors(q, r, q, points[i]);
            double dist = getPointToPointDistance(q, points[i]);

            //Check if new min omega was found
            if (omega < omega_min)
            {
                //Actualization of omega min and dist max
                omega_min = omega;
                dist_max = dist;
                i_min = i;
                index_vec.clear();
            }

            //Check if point with same min omega was found
            if (fabs(omega - omega_min) <= eps)
            {
                //Check if its distance from pivot is greater then max distance
                if (dist > dist_max)
                {
                    //Actualization of dist max
                    dist_max = dist;
                    index_vec.push_back(i_min);
                    i_min = i;
                }
            }
        }

        //Add point with min omega to points sorted by omega
        pbo.push_back(points[i_min]);

        //Check if there is more points with same omega min
        if (index_vec.empty() == true)

            //Remove added point
            points.remove(i_min);
        else
        {
            //Remove all points with same omega
            for (unsigned int i = 0; i < index_vec.size(); i++)
            {
                points.remove(index_vec[i] - i);
            }
            points.remove(i_min - index_vec.size());
        }

        //Actualization for new loop
        omega_min = 2 * M_PI;
        dist_max = 0;
        i_min = -1;
        n = points.size();
        index_vec.clear();
    }

    //Add point with minimum angle to convex hull
    ch.push_back(pbo[1]);
    int j = 2;

    //Process all points
    while (j < pbo.size())
    {
        //Check if the new point is in the left half plane
        if (getPointLinePosition(pbo[j], ch[ch.size() - 2], ch.back()) == 1)
        {
            ch.push_back(pbo[j]);
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

