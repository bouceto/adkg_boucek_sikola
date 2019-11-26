#ifndef DRAW_H
#define DRAW_H
#include <QtGui>
#include <vector>
#include <QWidget>


class Draw : public QWidget
{    
    Q_OBJECT

private:
    std::vector<QPolygonF> polygons;
    QPointF q;
    std::vector<int> result;


public:
    void loadFile(std::string &path);
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    QPointF getPoint(){return q;}
    std::vector<QPolygonF> getPolygons(){return polygons;}
    void setResult(std::vector<int> res){result = res;}
};

#endif // DRAW_H
