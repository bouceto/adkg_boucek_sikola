#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include <vector>

class Draw : public QWidget
{
    Q_OBJECT

private:
    QPolygon points;
    QPolygon ch;

public:
    explicit Draw(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *e);
    void clearCH(){ch.clear(); repaint();}
    void clearPoints(){points.clear(); repaint();}
    QPolygon getPoints(){return points;}
    void setCH(QPolygon &hull){ch = hull;}
    void setPoints(QPolygon Points){points = Points;}
    QPolygon generateGrid(int n, int height, int width);
    QPolygon generateCircle(int n, int height, int width);
    QPolygon generateRandomPoints(int n, int height, int width);
    QPolygon generateEllipse(int n, int height, int width);
    QPolygon generateStarShape(int n, int height, int width);
    QPolygon generateSquare(int n, int height, int width);
    QPolygon getCH(){return ch;}


signals:

public slots:
};

#endif // DRAW_H
