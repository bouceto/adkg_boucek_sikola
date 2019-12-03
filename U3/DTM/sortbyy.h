#ifndef SORTBYY_H
#define SORTBYY_H
#include<QtGui>
#include "qpoint3d.h"

class SortbyY
{
public:
    bool operator ()(QPoint3D &p1, QPoint3D &p2)
    {
        return (p1.y() < p2.y()) || (p1.y() == p2.y()) && (p1.x() < p2.x());
    }
};

#endif // SORTBYY_H
