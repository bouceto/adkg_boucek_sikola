#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpointfb.h"
#include "edge.h"

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPointFB> a, b;
    std::vector<Edge> res;
    QPointFB res_point;
    bool ab;

public:
    explicit Draw(QWidget *parent = nullptr);
    void changePolygon(){ab = !ab;}
    void setA (std::vector<QPointFB> &a_){a = a_;}
    void setB (std::vector<QPointFB> &b_){b = b_;}
    void setResPoint (QPointFB &res_point_){res_point = res_point_;}
    void setRes (std::vector<Edge> res_){res = res_;}
    std::vector<QPointFB> getA(){return a;}
    std::vector<QPointFB> getB(){return b;}
    std::vector<Edge> getRes(){return res;}
    void clearResults() {res.clear(); res_point.setX(-10.0); res_point.setY(-10.0);}
    void clearAll() {res.clear(); a.clear(); b.clear(); res_point.setX(-10.0); res_point.setY(-10.0);}
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void drawPolygon(QPainter &painter, std::vector<QPointFB> &polygon);
    void loadFile(std::string &path);

signals:

public slots:
};

#endif // DRAW_H
