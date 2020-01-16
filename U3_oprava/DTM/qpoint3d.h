#ifndef QPOINT3D_H
#define QPOINT3D_H

#include <QtGui>

class QPoint3D:public QPointF
{

private:
    double z;

public:
    QPoint3D():QPointF(0, 0),z(0){}
    QPoint3D(double x, double y, double z_):QPointF(x, y), z(z_){}

    double getZ(){return z;}
    void setZ(double z) {this ->z = z;}

    //bool operator== (QPoint3D &p1);

};

#endif // QPOINT3D_H
