#include "draw.h"
//#include "widget.h"
#include <QtGui>
#include <fstream>
#include <QWidget>


void Draw::loadFile(std::string &path)
{
    polygons.clear();
    result.clear();
    repaint();
    int id;
    double x;
    double y;
    QPolygonF polygon;

    //Load data from .txt file
    std::ifstream data(path);

    if(data.is_open())
    {
        //Fill the vectors
        while(data >> id >> x >> y)
        {
            //New polygon
            if (id == 1)
            {
                if (polygon.isEmpty() == FALSE)
                {
                    polygons.push_back(polygon);
                }
                polygon.clear();
                polygon << QPointF(x, y);
            }
            else
                polygon << QPointF(x, y);
        }

        //Add last polygon into vector
        polygons.push_back(polygon);

        data.close();
    }
    repaint();
}

Draw::Draw(QWidget *parent) : QWidget(parent)
{
    //Initialization
    q.setX(-10.0);
    q.setY(-10.0);
}

void Draw::mousePressEvent(QMouseEvent *e)
{
    //Get coordinates of the mouse
    double x = e->x();
    double y = e->y();

    //Set coordinate of q
    q.setX(x);
    q.setY(y);

    repaint();
}

void Draw::paintEvent(QPaintEvent *e)
{
    QPainter qp(this);
    qp.begin(this);
    int r = 10;

    //set color of highlighted polygon
    QBrush brush;
    brush.setColor(Qt::yellow);
    brush.setStyle(Qt::SolidPattern);
    QPainterPath path;

    QPolygonF highlighted_polygon;

    //Draw polygon with point inside
    for(unsigned int i = 0; i < result.size(); i++)
    {
        if(result[i] == 1 || result[i] == -1)
        {
            highlighted_polygon << polygons[i];
            path.addPolygon(highlighted_polygon);
            qp.fillPath(path, brush);
            qp.drawPolygon(highlighted_polygon);
            highlighted_polygon.clear();
        }

    }

    //Draw loaded polygons
    for(unsigned int i = 0; i < polygons.size(); i++)
    {
        QPolygonF poly_in_polygons = polygons[i];
        qp.drawPolygon(poly_in_polygons);
    }

    //Draw point Q
    qp.drawEllipse(q.x() - r/2,q.y() - r/2, r, r);
    qp.end();
}
