#include "widget.h"
#include "ui_widget.h"
#include "draw.h"
#include "algorithms.h"
#include <QFileDialog>
#include <QFile>
#include <fstream>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //Analyze point and polygon position
    QPointF q = ui->Canvas->getPoint();
    std::vector<QPolygonF> polygons = ui->Canvas->getPolygons();
    std::vector<int> result;
    std::vector<int> ident_point;
    int p = 0;

    std::vector<QPointF> pol;
    QPolygonF one_polygon;
    int res = 0;

    //Get one polygon
    for (unsigned int j = 0; j < polygons.size(); j++) {
        one_polygon = polygons[j];
        for (int  i= 0; i< one_polygon.size(); i++) {

            pol.push_back(one_polygon[i]);

        }

        for (unsigned int k = 0; k < pol.size(); k++) {
            if (q == pol[k])
                p += 1;
        }

        //Check if point q is on vertex
        if (p < 1)
            {

            //Select method
            if (ui->comboBox->currentIndex() == 0)
            {
                res = Algorithms::positionPointPolygonWinding(q, pol);
            }
            else
                res = Algorithms::positionPointPolygonRayCrossing(q, pol);
                result.push_back(res);
            }
        else result.push_back(1);

        pol.clear();
        p=0;
    }


    ui->Canvas->setResult(result);    
    ui->Canvas->repaint();

}

void Widget::on_pushButton_2_clicked()
{
    QString path(QFileDialog::getOpenFileName(this, tr("Open file with polygons"), "../", tr("Text Files (*.txt)")));
    std::string path_std = path.toStdString();
    ui->Canvas->loadFile(path_std);
}


