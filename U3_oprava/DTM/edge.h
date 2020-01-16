#ifndef EDGE_H
#define EDGE_H
#include "qpoint3d.h"


class Edge
{
private:
    QPoint3D s,e;
public:
    Edge():s(0,0,0), e(0,0,0){}
    Edge(QPoint3D &start, QPoint3D &end):s(start), e(end){}

    QPoint3D & getStart(){return s;}
    void setStart(QPoint3D &s){this->s = s;}
    QPoint3D & getEnd(){return e;}
    void setEnd(QPoint3D &e){this->e = e;}


    void changeOrientation(){
        QPoint3D help = s;
        s = e;
        e = help;
    }

    bool operator == (const Edge &h) const {
        return (h.s == s) && (h.e == e);
    }

};

#endif // EDGE_H
