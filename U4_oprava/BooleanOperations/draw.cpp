#include "draw.h"
#include "qpointfb.h"
#include <QtGui>
#include <fstream>

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    ab = true;

    //Initialization
    res_point.setX(-10.0);
    res_point.setY(-10.0);
}

void Draw::mousePressEvent(QMouseEvent *event)
{
     QPointFB q(event->x(), event->y());

     //Add to A
     if(ab)
         a.push_back(q);

     //Add to B
     else
         b.push_back(q);

     repaint();
}

void Draw::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.begin(this);

    //Draw A
    QPen pA(Qt::green);
    painter.setPen(pA);
    drawPolygon(painter, a);

    //Draw B
    QPen pB(Qt::blue);
    painter.setPen(pB);
    drawPolygon(painter, b);

    //Draw edges
    QPen pE(Qt::red);
    pE.setWidth(2);
    painter.setPen(pE);
    for(int i = 0; i < res.size(); i++)
    {
        painter.drawLine(res[i].getStart(), res[i].getEnd());
    }

    //Draw point
    QPen pP(Qt::red);
    painter.drawEllipse(res_point.x() - 3, res_point.y() - 3, 6, 6);

    painter.end();
}

void Draw::drawPolygon(QPainter &painter, std::vector<QPointFB> &polygon)
{
    //Draw polygon
    QPolygon polyg;
    for(int i = 0; i < polygon.size(); i++)
    {
        polyg.append(QPoint (polygon[i].x(), polygon[i].y()));
    }

    painter.drawPolygon(polyg);
}

void Draw::loadFile(std::string &path)
{
    a.clear();
    b.clear();
    res.clear();
    int id;
    double x;
    double y;
    QPointFB p;

    //Load data from .txt file
    std::ifstream data(path);

    if(data.is_open())
    {
        //Fill the vectors
        while(data >> id >> x >> y)
        {
            //Polygon A
            if (id == 1)
            {
                p.setX(x);
                p.setY(y);
                a.push_back(p);
            }

            if (id == 2)
            {
                p.setX(x);
                p.setY(y);
                b.push_back(p);
            }
        }
    }

    repaint();
}
