#include "draw.h"
#include <QtGui>
#include <QDebug>
#include "widget.h"
#include <cmath>
#include <QRandomGenerator>
#include "algorithms.h"


Draw::Draw(QWidget *parent) : QWidget(parent)
{

}

 void Draw::mousePressEvent(QMouseEvent *event)
 {
     //Get coordinates
     int x = event->x();
     int y = event->y();

     //Add to the list
     QPoint q(x, y);
     points.push_back(q);

     //Repaint screen
     repaint();


 }

 void Draw::paintEvent(QPaintEvent *e)
 {
     QPainter qp(this);
     qp.begin(this);

     // Draw points
     for(int i = 0; i< points.size(); i++)
     {
         qp.drawEllipse(points[i].x() - 3, points[i].y() - 3, 6, 6);
     }

     // Draw Ch
     qp.drawPolygon(ch);
     qp.end();
     }


QPolygon Draw::generateGrid(int n, int height, int width)
{
    QPolygon random_points;
    QPoint q;
    q.setX((width - height)/2);
    q.setY(10);
    n = ceil(sqrt(n));
    for (int i = 0; i < n; i++)
    {
        for (int i = 0; i < n; i++)
        {
            q.setX((q.x()) + (height - 10)/n);
            random_points.push_back(q);
        }
        q.setX((width - height)/2);
        q.setY((q.y()) + (height - 10)/n);
    }

    random_points = Algorithms::removeDuplicite(random_points);
    qDebug()<< random_points.size();

    return random_points;
}

QPolygon Draw::generateCircle(int n, int height, int width)
{
    QPolygon random_points;
    QPoint s, q;
    s.setX(width/2);
    s.setY(height/2);
    double fi = 2 * M_PI / n;

    for (int i = 0; i < n; i++)
    {
         q.setX(s.x() + (height/2 - 10) * cos(fi*i));
         q.setY(s.y() + (height/2 - 10) * sin(fi*i));
         random_points.push_back(q);
    }

    random_points = Algorithms::removeDuplicite(random_points);
    qDebug()<< random_points.size();

    return random_points;
}

QPolygon Draw::generateRandomPoints(int n, int height, int width)
{
    QPolygon random_points;
    QPoint q;

    for (int i = 0; i < n; i++)
    {
        q.setX(QRandomGenerator::global()->bounded(10, width - 10));
        q.setY(QRandomGenerator::global()->bounded(10, height - 10));
        random_points.push_back(q);
    }
    random_points = Algorithms::removeDuplicite(random_points);
    qDebug()<< random_points.size();

    return random_points;
}

QPolygon Draw::generateEllipse(int n, int height, int width)
{
    QPolygon random_points;
    QPoint s, q;
    s.setX(width/2);
    s.setY(height/2);

    double fi = 2 * M_PI / n;

    for (int i = 0; i < n; i++)
    {
         q.setX(s.x() + (width/2 - 10) * cos(fi*i));
         q.setY(s.y() + (height/2 - 10) * sin(fi*i));
         random_points.push_back(q);
    }
    random_points = Algorithms::removeDuplicite(random_points);
    qDebug()<< random_points.size();

    return random_points;
}

QPolygon Draw::generateStarShape(int n, int height, int width)
{
    QPolygon random_points;
    QPoint s, q;
    s.setX(width/2);
    s.setY(height/2);

    double fi = 2 * M_PI / n;

    for (int i = 0; i < n; i++)
    {

             if ((i+1)%2 == 0)
             {
                 q.setX(s.x() + (height/2 - 10) * cos(fi*i));
                 q.setY(s.y() + (height/2 - 10) * sin(fi*i));
                 random_points.push_back(q);
             }
             else
             {
                 q.setX(s.x() + (height/2 - 10)/4 * cos(fi*i));
                 q.setY(s.y() + (height/2 - 10)/4 * sin(fi*i));
                 random_points.push_back(q);
             }
    }
    random_points = Algorithms::removeDuplicite(random_points);
    qDebug()<< random_points.size();

    return random_points;
}

QPolygon Draw::generateSquare(int n, int height, int width)
{
    QPolygon random_points;
    QPoint q;
    q.setX((width - height)/2);
    q.setY(10);
    random_points.push_back(q);

    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < n-1; i++)
        {
            q.setX((q.x()) + round(height - 10)/n);
            random_points.push_back(q);
        }
        q.setY(q.y()+(n-1)*round((height - 10)/n));
        q.setX((width - height)/2);
    }

    q.setX((width - height)/2);
    q.setY(10);

    for (int j = 0; j < 2; j++)
    {
        for (int i = 0; i < n-1; i++)
        {
            q.setY((q.y())+round((height - 10)/n));
            bool is_equal = false;
            for (int k = 0; k < random_points.size(); k++)
            {
                if (q == random_points[k])
                {
                    is_equal = true;
                    break;
                }
            }
            if (is_equal == false)
                random_points.push_back(q);
        }
        q.setX(q.x()+(n-1)*round((height - 10)/n));
        q.setY(10);
    }
    random_points = Algorithms::removeDuplicite(random_points);
    qDebug()<< random_points.size();

    return random_points;
}
