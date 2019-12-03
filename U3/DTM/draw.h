#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>
#include <QtGui>

#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"
#include "widget.h"

class Draw : public QWidget
{
    Q_OBJECT
private:
    std::vector<QPoint3D> points;
    std::vector<Edge> dt;
    std::vector<Edge> contours, label_contours, main_contours;
    std::vector<Triangle> dtm, aspect_dtm;
    int ch;
    std::vector<QPoint3D> polygon;

public:
    explicit Draw(QWidget *parent = nullptr);
    void setPoints(std::vector<QPoint3D> &points_){points = points_;}
    std::vector<Edge>& getDt() {return dt;}
    void setDt(std::vector<Edge> &dt_){dt=dt_;}
    void setContours(std::vector<Edge> &contours_){contours = contours_;}
    void setMainContours(std::vector<Edge> &main_contours_){main_contours = main_contours_;}
    void setLabelContours(std::vector<Edge> &label_contours_){label_contours = label_contours_;}
    void setCheck(int ch_){ch = ch_;}
    int getCheck(){return ch;}
    std::vector<QPoint3D> getBoundaryPolygon(){return polygon;}
    void setBoundaryPolygon(std::vector<QPoint3D> &polygon_){polygon = polygon_;}
    std::vector<Edge>& getMainContours() {return main_contours;}
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    std::vector<QPoint3D> getPoints(){return points;}
    void clearPoints(){points.clear(); }
    void clearDT(){dt.clear();}
    void clearSlope(){dtm.clear();}
    void clearAspect(){aspect_dtm.clear();}
    void clearContours(){contours.clear(); label_contours.clear(); main_contours.clear();}
    void clearPolygon(){polygon.clear();}
    int getDtSize(){return dt.size();}
    void setDTM(std::vector<Triangle> &dtm_){dtm = dtm_;}
    void setAspectDTM(std::vector<Triangle> &aspect_dtm_){aspect_dtm = aspect_dtm_;}
signals:

public slots:
};

#endif // DRAW_H
