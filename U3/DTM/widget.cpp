#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "triangle.h"
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    z_min = 0;
    z_max = 1000;
    dz = 10;
    n = 100;
    mc = 100;

    //Set initial values
    ui->lineEdit->setText(QString::number(z_min));
    ui->lineEdit_2->setText(QString::number(z_max));
    ui->lineEdit_3->setText(QString::number(dz));
    ui->lineEdit_4->setText(QString::number(n));
    ui->lineEdit_5->setText(QString::number(mc));

    ui->Canvas->setCheck(0);

}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    ui->Canvas->clearDT();
    std::vector<Edge> dtk;
    std::vector<QPoint3D> points_inside;
    std::vector<QPoint3D> polygon = ui->Canvas->getBoundaryPolygon();
    std::vector<QPoint3D> points=ui->Canvas->getPoints();
    if (points.empty())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Insert or generete points first!");
        messageBox.setFixedSize(500,200);
    }
    else
    {
        int k = 0;
        int n = polygon.size();
        int res;

        if (polygon.size() > 0)
        {
            for (int i =0; i < points.size(); i++)
            {
                res = Algorithms::positionPointPolygonWinding(points[i], polygon);
                if (res == 1)
                {
                    points_inside.push_back(points[i]);
                }
            }

            //Construct DT
            std::vector<Edge> dt = Algorithms::DT(points_inside);

            for (int i = 0; i < dt.size(); i+= 3)
            {
                for (int j = 0; j < n; j++)
                {
                 int q1 = Algorithms::intersectOfTwoLinesExists(dt[i].getStart(), dt[i].getEnd(), polygon[j], polygon[(j+1)%n]);
                 int q2 = Algorithms::intersectOfTwoLinesExists(dt[i+1].getStart(), dt[i+1].getEnd(), polygon[j], polygon[(j+1)%n]);
                 int q3 = Algorithms::intersectOfTwoLinesExists(dt[i+2].getStart(), dt[i+2].getEnd(), polygon[j], polygon[(j+1)%n]);
                 k += q1 + q2 + q3;
                }
                if (k == 0)
                {
                 dtk.push_back(dt[i]);
                 dtk.push_back(dt[i+1]);
                 dtk.push_back(dt[i+2]);
                }
                k = 0;
            }
            ui->Canvas->setDt(dtk);
        }
        else
        {
          std::vector<Edge> dt = Algorithms::DT(points);
          ui->Canvas->setDt(dt);
        }


        repaint();
    }
}

void Widget::on_pushButton_2_clicked()
{
    ui->Canvas->clearPoints();
    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearPoints();
    ui->Canvas->clearDT();
    repaint();

}

void Widget::on_pushButton_4_clicked()
{
    std::vector<QPoint3D> points=ui->Canvas->getPoints();

    if (points.empty())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Insert or generete points first!");
        messageBox.setFixedSize(500,200);
    }
    else
    {
        std::vector<Edge> dt;

        //Create triangulation

        dt = Algorithms::DT(points);
        ui->Canvas->setDt(dt);


        //Create contour lines
        std::vector<Edge> contours = Algorithms::createContourLines(dt, z_min, z_max, dz);
        std::vector<Edge> main_contours = Algorithms::createContourLines(dt, z_min, z_max, dz * 5);
        ui->Canvas->setContours(contours);
        ui->Canvas->setMainContours(main_contours);

        repaint();
    }

}

void Widget::on_lineEdit_editingFinished()
{
    //Set z_min
    z_min = ui->lineEdit->text().toDouble();
}

void Widget::on_lineEdit_2_editingFinished()
{
    //Set z_max
    z_max = ui->lineEdit_2->text().toDouble();
}

void Widget::on_lineEdit_3_editingFinished()
{
    //Set dz
    dz = ui->lineEdit_3->text().toDouble();
}

void Widget::on_pushButton_5_clicked()
{
    std::vector<Edge> dt;

    //Create triangulation

    std::vector<QPoint3D> points = ui->Canvas->getPoints();
    if (points.empty())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Insert or generete points first!");
        messageBox.setFixedSize(500,200);
    }
    else
    {
        dt = Algorithms::DT(points);
        ui->Canvas->setDt(dt);
        //Analyze DTM
        std::vector<Triangle> dtm = Algorithms::analyzeDTM(dt);
        ui->Canvas->setDTM(dtm);
        ui->Canvas->clearAspect();
        repaint();
    }
}

void Widget::on_pushButton_7_clicked()
{
    std::vector<Edge> label_contours = ui->Canvas->getMainContours();
    if (label_contours.empty())
    {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Insert or generete points first!");
        messageBox.setFixedSize(500,200);
    }
    else
    {
        ui->Canvas->setLabelContours(label_contours);
        repaint();
    }
}

void Widget::on_pushButton_8_clicked()
{
    std::vector<QPoint3D> points;
    QSize size = ui->Canvas->size();
    int height = size.height();
    int width = size.width();

    if (ui->comboBox->currentIndex() == 0)
        points = Algorithms::generateKupa(n, height, width);
    else if(ui->comboBox->currentIndex() == 1)
        points = Algorithms::generateRidge(n, height, width);
    else if(ui->comboBox->currentIndex() == 2)
        points = Algorithms::generateValley(n, height, width);
    else if(ui->comboBox->currentIndex() == 3)
        points = Algorithms::generateSettling(n, height, width);
    else if(ui->comboBox->currentIndex() == 4)
        points = Algorithms::generateRandomPoints(n, height, width);

    ui->Canvas->setPoints(points);
    repaint();
}

void Widget::on_lineEdit_4_editingFinished()
{
    n = ui->lineEdit_4->text().toDouble();
}

void Widget::on_pushButton_9_clicked()
{
    ui->Canvas->clearPoints();
    ui->Canvas->clearDT();
    ui->Canvas->clearSlope();
    ui->Canvas->clearContours();
    ui->Canvas->clearPolygon();
    ui->Canvas->clearAspect();

    repaint();

}

void Widget::on_checkBox_stateChanged(int arg1)
{

    if (ui->checkBox->isChecked())
        ui->Canvas->setCheck(1);
    else if (!(ui->checkBox->isChecked()))
        ui->Canvas->setCheck(0);

}

void Widget::on_lineEdit_5_editingFinished()
{
    mc = ui->lineEdit_5->text().toDouble();
}

void Widget::on_pushButton_6_clicked()
{
    std::vector<Edge> dt;

    //Create triangulation

    std::vector<QPoint3D> points = ui->Canvas->getPoints();
    if (points.empty())
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Insert or generete points first!");
        messageBox.setFixedSize(500,200);
    }
    else
    {
        dt = Algorithms::DT(points);
        ui->Canvas->setDt(dt);
        //Analyze DTM
        std::vector<Triangle> aspect_dtm = Algorithms::analyzeDTM(dt);
        ui->Canvas->setAspectDTM(aspect_dtm);
        repaint();
    }
}
