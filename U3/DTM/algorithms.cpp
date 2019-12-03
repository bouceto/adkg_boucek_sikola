#include "algorithms.h"

Algorithms::Algorithms()
{

}

int Algorithms::getPointLinePosition(QPoint3D &q,QPoint3D &p1,QPoint3D &p2)
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
    if (t>0)
        return 1;
    if (t<0)
        return 0;
    return -1;
}


double Algorithms::getCircleRadius(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &c){

    //Return radius of the circle given by 3 points
    double x1 = p1.x();
    double y1 = p1.y();

    double x2 = p2.x();
    double y2 = p2.y();

    double x3 = p3.x();
    double y3 = p3.y();

    //Calculate coeficients k1-k12
    double k1 = x1*x1 + y1*y1;
    double k2 = x2*x2 + y2*y2;
    double k3 = x3*x3 + y3*y3;
    double k4 = y1 - y2;
    double k5 = y1 - y3;
    double k6 = y2 - y3;
    double k7 = x1 - x2;
    double k8 = x1 - x3;
    double k9 = x2 - x3;
    double k10 = x1 * x1;
    double k11 = x2 * x2;
    double k12 = x3 * x3;

    //Midpoint of the circle
    double m_numerator = k12 * (-k4) + k11 * k5 - (k10 + k4*k5)*k6;
    double m_denominator = x3 * (-k4) + x2 * k5 + x1 * (-k6);
    double m = 0.5 * m_numerator / m_denominator;

    double n_numerator = k1 * (-k9) + k2 * k8 + k3 * (-k7);
    double n_denominator = y1 * (-k9) + y2 * k8 + y3 * (-k7);
    double n = 0.5 * n_numerator / n_denominator;

    //Set circle center
    c.setX(m);
    c.setY(n);

    //Radius of the circle
    return sqrt((x1 - m) * (x1 - m) + (y1 - n) * (y1 - n));
}

double Algorithms::distance2Points(QPoint3D &p1, QPoint3D &p2)
{
     // Get distance of two points
     double dx = (p1.x() - p2.x());
     double dy = (p1.y() - p2.y());

     return sqrt(dx*dx + dy*dy);
}

int Algorithms::getNearestpoint(QPoint3D &p, std::vector<QPoint3D> &points)
{
    // Find nearest point
    int i_min = 1;
    double d_min = distance2Points(p, points[1]);

    //Browses all points
    for (unsigned int i = 2; i < points.size(); i++)
    {
        double d = distance2Points(p, points[i]);

        //Actualize minimum i and distance
        if (d < d_min)
        {
            d_min=d;
            i_min=i;
        }
    }

    return i_min;
}

int Algorithms::getDelaunayPoint(QPoint3D &s, QPoint3D &e, std::vector<QPoint3D> &points)
{
    //Find optimal Delaunay point
    int i_min = -1;
    double r_min = INFINITY;

    //Browse all points
    for (unsigned int i = 0; i < points.size(); i++ )
    {
        //Test if points[i] is different from start, end
        if((s != points[i]) && (e != points[i]))
        {
            //Points[i] in the left half plane
            if(getPointLinePosition(points[i], s, e) == 1)
            {
                //Get circle radius
                QPoint3D c;
                double r = getCircleRadius(s, e, points[i], c);

                //Circle center in the right half plane
                if (getPointLinePosition(c, s, e) == 0)
                    r = -r;

                //Actualize minimum
                if(r < r_min)
                {
                    r_min = r;
                    i_min = i;
                }
            }
        }
    }

    return i_min;
}

std::vector<Edge> Algorithms::DT(std::vector<QPoint3D> &points)
{
    //Delaunay triangulation
    std::vector<Edge> dt;
    std::list<Edge> ael;

    //Sort points by X
    sort(points.begin(), points.end(), SortbyX());

    //Pivot
    QPoint3D q = points[0];

    //Find nearest point
    int index = getNearestpoint(q, points);
    QPoint3D qn = points[index];

    //First Delaunay edge
    Edge e1(q, qn);

    //Find optimal Delaunay point
    int i_o = getDelaunayPoint(q, qn, points);

    //No suitable point
    if (i_o == -1)
    {
        //Change orientation
        e1.changeOrientation();

        //Find optimal Delaunay point
        i_o = getDelaunayPoint(e1.getStart(), e1.getEnd(), points);
    }

    //Third point of the triangle
    QPoint3D q3 = points[i_o];

    //Create initial triangle
    Edge e2(e1.getEnd(), q3);
    Edge e3(q3, e1.getStart());

    //Add first triangle to DT
    dt.push_back(e1);
    dt.push_back(e2);
    dt.push_back(e3);

    //Add edges to active edges list
    ael.push_back(e1);
    ael.push_back(e2);
    ael.push_back(e3);

    //Repeat until any triangle exists
    while (!ael.empty())
    {
        Edge ed1=ael.back();
        ael.pop_back();

        //Change orientation of edge
        ed1.changeOrientation();

        //Find optinal Delaunay point
        int i2 = getDelaunayPoint(ed1.getStart(), ed1.getEnd(), points);

        if (i2 != -1)
        {
            // Third point of the triangle
            QPoint3D q3n = points[i2];

            //Create new triangle
            Edge ed2(ed1.getEnd(), q3n);
            Edge ed3(q3n, ed1.getStart());

            //Add triangle to DT
            dt.push_back(ed1);
            dt.push_back(ed2);
            dt.push_back(ed3);

            //Change orientation
            ed2.changeOrientation();
            ed3.changeOrientation();

            //Are ed2 and ed3 in active edges list
            std::list<Edge>::iterator ie2 = find(ael.begin(), ael.end(), ed2);
            std::list<Edge>::iterator ie3 = find(ael.begin(), ael.end(), ed3);

            //Add edge ed2 to active edge list
            if (ie2 == ael.end())
            {
                ed2.changeOrientation();
                ael.push_back(ed2);
            }

            //Edge ed2 already in active edge list, erase
            else ael.erase(ie2);

            //Add edge ed3 to active edge list
            if (ie3 == ael.end())
            {
                ed3.changeOrientation();
                ael.push_back(ed3);
            }

            //Edge ed3 already in active edge list, erase
            else ael.erase(ie3);
        }
    }

    return  dt;
}

QPoint3D Algorithms::getContourPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    //Compute contour point
    double x = (p2.x() - p1.x())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.x();
    double y = (p2.y() - p1.y())/(p2.getZ() - p1.getZ()) * (z - p1.getZ()) + p1.y();
    QPoint3D A(x, y, z);

    return A;
}

std::vector<Edge> Algorithms::createContourLines(std::vector<Edge> &dt, double z_min, double z_max, double dz)
{
    //Generate contour lines
    std::vector<Edge> contours;

    //Browse all triangles
    for (unsigned int i=0; i<dt.size(); i+=3)
    {
        //Get triangle vertices
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        // Find all plane - triangle intersections.
        for (double z = z_min; z <= z_max; z += dz)
        {
            // Get height differences
            double dz1 = z - p1.getZ();
            double dz2 = z - p2.getZ();
            double dz3 = z - p3.getZ();

            //Test criteria
            double t12 = dz1 * dz2;
            double t23 = dz2 * dz3;
            double t31 = dz3 * dz1;

            //Triangle in the plane(coplanar)
            if((dz1 == 0) && (dz2 == 0) && (dz3 == 0))
                continue;

            //Edge e12 of the triangle in the plane
            else if ((dz1 == 0) && (dz2 == 0))
                contours.push_back(dt[i]);

            //Edge e23 of the triangle in the plane
            else if ((dz2 == 0) && (dz3 == 0))
                contours.push_back(dt[i+1]);

            //Edge e31 of the triangle in the plane
            else if ((dz3 == 0) && (dz1 == 0))
                contours.push_back(dt[i+2]);

            //Egdes e12, e13 are intersected
            else if ((t12 < 0 && t23 <= 0) || (t12 <= 0 && t23 < 0))
            {
                //Compute intersection points
                QPoint3D a = getContourPoint(p1, p2, z);
                QPoint3D b = getContourPoint(p2, p3, z);

                // Create fragment of contour line.
                Edge e(a, b);
                contours.push_back(e);
            }

            //Egdes e23, e31 are intersected
            else if ((t23 < 0 && t31 <= 0) || (t23 <= 0 && t31 < 0))
            {
                //Compute intersection points
                QPoint3D a = getContourPoint(p2, p3, z);
                QPoint3D b = getContourPoint(p3, p1, z);

                // Create fragment of contour line.
                Edge e(a, b);
                contours.push_back(e);
            }

            //Egdes e31, e12 are intersected
            else if ((t31 < 0 && t12 <= 0) || (t31 <= 0 && t12 < 0))
            {
                //Compute intersection points
                QPoint3D a = getContourPoint(p3, p1, z);
                QPoint3D b = getContourPoint(p1, p2, z);

                // Create fragment of contour line
                Edge e(a, b);
                contours.push_back(e);
            }
        }
    }

    return contours;
}

double Algorithms::calculateSlope(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Calculate slope of the triangle
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy * vz - uz * vy;
    double ny = -(ux * vz - vx * uz);
    double nz = ux * vy - uy * vx;
    double nt = sqrt(nx * nx + ny * ny + nz * nz);

    return acos(nz / nt) / M_PI * 180;
}

double Algorithms::calculateAspect(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3)
{
    //Calculate slope of the triangle
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double uz = p2.getZ() - p1.getZ();

    double vx = p3.x() - p1.x();
    double vy = p3.y() - p1.y();
    double vz = p3.getZ() - p1.getZ();

    //Calculate normal vector and its norm
    double nx = uy * vz - uz * vy;
    double ny = -(ux * vz - vx * uz);

    double aspect = atan2(nx, ny) / M_PI * 180;
    if (aspect < 0)
        aspect = aspect + 360;

    return aspect;
}

std::vector<Triangle> Algorithms:: analyzeDTM(std::vector<Edge> & dt)
{
    //Analyze DTM compute slope and aspect
    std::vector<Triangle> triangles;

    //Browse triangulation one by one
    for (int i = 0; i < dt.size(); i += 3)
    {
        //Vertices of the triangle
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        //Slope and aspect
        double slope = calculateSlope(p1, p2, p3);
        double aspect = calculateAspect(p1, p2, p3);

        //Create triangle
        Triangle t(p1, p2, p3, slope, aspect);

        //Add triangle to vector
        triangles.push_back(t);
    }

    return triangles;
}

double Algorithms::getPointLineDistance(QPoint3D &q, QPoint3D &p1, QPoint3D &p2)
{
    //Calculate point and line distance
    double numerator = q.x() * (p1.y() - p2.y()) + p1.x() * (p2.y() - q.y()) + p2.x() * (q.y() - p1.y());
    double dx = p2.x() - p1.x();
    double dy = p2.y() - p1.y();
    double denumenator = sqrt(dx * dx + dy * dy);

    double dist = fabs(numerator)/denumenator;
    return dist;
}

std::vector<QPoint3D> Algorithms::generateRandomPoints(int n, int height, int width)
{
    std::vector<QPoint3D> points;

     for (int i =0; i < n; i++)
     {
         QPoint3D point;
         double x = QRandomGenerator::global()->bounded(10, width - 10);
         double y = QRandomGenerator::global()->bounded(10, height - 10);
         point.setX(x);
         point.setY(y);
         double random = std::rand() * 200.0 / RAND_MAX;
         point.setZ(random);
         points.push_back(point);
     }

     return points;
}

std::vector<QPoint3D> Algorithms::generateKupa(int n, int height, int width)
{
    //Generate random points
    std::vector<QPoint3D> points = generateRandomPoints(n, height, width);

    //Initialize center
    double sum_x = 0;
    double sum_y = 0;

    //Count center of points
    for (int i = 0; i < points.size(); i++)
    {
        sum_x = sum_x + points[i].x();
        sum_y = sum_y + points[i].y();
    }

    QPoint3D point;
    point.setX(sum_x/points.size());
    point.setY(sum_y/points.size());
    point.setZ(1000);
    points.push_back(point);

    //Giving z to points by distance
    for (int i = 0; i < (points.size()-1); i++)
    {
        double d = distance2Points(points[n], points[i]);
        points[i].setZ(1000 - d + (std::rand() * 10 / RAND_MAX));
    }

    return points;
}

std::vector<QPoint3D> Algorithms::generateRidge(int n, int height, int width)
{
    std::vector<QPoint3D> points = generateRandomPoints(n, height, width);
    //Make vertical or horizontal ridge
    int w = QRandomGenerator::global()->bounded(2);

    qDebug() << w;

    if (w == 0)
        std::sort(points.begin(), points.end(), SortbyX());
    else
        std::sort(points.begin(), points.end(), SortbyY());

    //Set z of ridge
    points[0].setZ(1100);
    points.back().setZ(1050);

    //Initialize center
    double sum_x = 0;
    double sum_y = 0;

    //Count center of points
    for (int i = 0; i < points.size(); i++)
    {
        sum_x = sum_x + points[i].x();
        sum_y = sum_y + points[i].y();
    }

    QPoint3D point;
    point.setX(sum_x/points.size());
    point.setY(sum_y/points.size());
    point.setZ(1000);
    points.push_back(point);

    for (int i = 1; i < points.size() - 1; i++)
    {
        double d1 = distance2Points(points[i], points[0]);
        double d2 = distance2Points(points[i], points[points.size()-2]);

        if (d1 < d2)
            points[i].setZ(1000 - getPointLineDistance(points[i], points[0], points.back()) + (std::rand() * 10 / RAND_MAX));
        else
            points[i].setZ(1000 - getPointLineDistance(points[i], points[points.size()-2], points.back()) + (std::rand() * 10 / RAND_MAX));

    }
    return points;
}

std::vector<QPoint3D> Algorithms::generateValley(int n, int height, int width)
{
    std::vector<QPoint3D> points = generateRandomPoints(n, height, width);
    //Make vertical or horizontal ridge
    int w = QRandomGenerator::global()->bounded(2);

    qDebug() << w;

    if (w == 0)
        std::sort(points.begin(), points.end(), SortbyX());
    else
        std::sort(points.begin(), points.end(), SortbyY());

    //Set z of ridge
    points[0].setZ(100);
    points.back().setZ(150);

    //Initialize center
    double sum_x = 0;
    double sum_y = 0;

    //Count center of points
    for (int i = 0; i < points.size(); i++)
    {
        sum_x = sum_x + points[i].x();
        sum_y = sum_y + points[i].y();
    }

    QPoint3D point;
    point.setX(sum_x/points.size());
    point.setY(sum_y/points.size());
    point.setZ(120);
    points.push_back(point);

    for (int i = 1; i < points.size() - 1; i++)
    {
        double d1 = distance2Points(points[i], points[0]);
        double d2 = distance2Points(points[i], points[points.size()-2]);

        if (d1 < d2)
            points[i].setZ(150 + getPointLineDistance(points[i], points[0], points.back()) + (std::rand() * 10 / RAND_MAX));
        else
            points[i].setZ(150 + getPointLineDistance(points[i], points[points.size()-2], points.back()) + (std::rand() * 10 / RAND_MAX));

    }
    return points;

}

std::vector<QPoint3D> Algorithms::generateSettling(int n, int height, int width)
{
    std::vector<QPoint3D> points = generateRandomPoints(n, height, width);
    //Make vertical or horizontal ridge
    int w = QRandomGenerator::global()->bounded(2);

    if (w == 0)
        std::sort(points.begin(), points.end(), SortbyX());
    else
        std::sort(points.begin(), points.end(), SortbyY());

    //Set z of ridge
    points[0].setZ(1000);
    points[1].setZ(1000);

    for (int i = 2; i < points.size(); i++)
    {
        double d = distance2Points(points[i], points[0]);
        points[i].setZ(1000 - getPointLineDistance(points[i], points[0], points[1]) + (std::rand() * 10 / RAND_MAX));
    }

    double z1 = points[points.size()/2].getZ();
    double z2 = points[points.size()/2 + n/4].getZ();
    double dz = z1 - z2;

    for (int i = points.size()/2; i < (points.size()/2 + n/4); i++)
    {
        points[i].setZ(z1);
    }

    for (int i = (points.size()/2 + n/4); i < points.size(); i++)
    {
        double z3 = points[i].getZ();
        points[i].setZ(z3 + dz);
    }
    return points;
}

int Algorithms::positionPointPolygonWinding(QPoint3D &q, std::vector<QPoint3D> &pol)
{
    // Analyze Position of the Point and the Polygon
    double wn = 0.0;

    // Tolerance
    double eps = 1.0e-6;

    // The size of polygon
    int n = pol.size();

    //Browse all points of polygon
    for (int i = 0; i < n; i++){

        //Measure angle
        double omega = getAngle2Vectors(pol[i], q, pol[(i+1)%n], q);

        //Get orientation of the point and the polygon edge
        int orient = getPointLinePosition(q, pol[i], pol[(i+1)%n]);

        //Point in the left half plane
        if (orient == 1)
            wn += omega;

        //Point in the right half plane
        else
            wn -= omega;
    }

    //Point inside polygon
    if (fabs(fabs(wn) - 2 * M_PI) <= eps)
        return 1;

    //Point outside polygon
    return 0;
}

double Algorithms::getAngle2Vectors(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &p4)
{
    // Calculate Vector betwen 2 vectors.
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

int Algorithms::intersectOfTwoLinesExists(QPoint3D &p1, QPoint3D &p2, QPoint3D &p3, QPoint3D &p4)
{
    double ux = p2.x() - p1.x();
    double uy = p2.y() - p1.y();
    double vx = p4.x() - p3.x();
    double vy = p4.y() - p3.y();
    double wx = p1.x() - p3.x();
    double wy = p1.y() - p3.y();

    double k1 = vx * wy - vy * wx;
    double k2 = ux * wy - uy * wx;
    double k3 = vy * ux - vx * uy;

    double alfa = k1 / k3;
    double beta = k2 / k3;

    if ((alfa > 0 && alfa < 1) && (beta > 0 && beta < 1))
        //Intersect exists
        return 1;
    else
        //Intersect doesn't exist
        return 0;
}



