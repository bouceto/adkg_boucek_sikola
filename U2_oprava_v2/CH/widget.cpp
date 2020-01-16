#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "draw.h"
#include <QElapsedTimer>
#include <QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit_2->setReadOnly(true);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    //Create convex hull
    QPolygon ch;
    QPolygon points = ui->Canvas->getPoints();

    //Create CH
    QElapsedTimer timer;
    timer.start();
    if (ui->comboBox->currentIndex() == 0)
        ch = Algorithms::jarvisScan(points);
    if (ui->comboBox->currentIndex() == 1)
        ch = Algorithms::qHull(points);
    if (ui->comboBox->currentIndex() == 2)
        ch = Algorithms::sweepLine(points);
    if (ui->comboBox->currentIndex() == 3)
        ch = Algorithms::grahamScan(points);

    ui->lineEdit_2->setText(QString::number(timer.elapsed()) + " ms");

    //Draw
    if (ui->checkBox->isChecked())
    {
        QPolygon strict_ch = Algorithms::strictlyConvexHull(ch);
        ui->Canvas->setCH(strict_ch);
        repaint();
    }
    else
    {
        ui->Canvas->setCH(ch);
        repaint();
    }
}

void Widget::on_pushButton_4_clicked()
{
    QSize size = ui->Canvas->size();
    int height = size.height();
    int width = size.width();
    QPolygon random_points;
    int point_number = ui->lineEdit->text().toInt();

    if (ui->comboBox_2->currentIndex() == 0)
        random_points = ui->Canvas->generateGrid(point_number, height, width);
    if (ui->comboBox_2->currentIndex() == 1)
        random_points = ui->Canvas->generateCircle(point_number, height, width);
    if (ui->comboBox_2->currentIndex() == 2)
        random_points = ui->Canvas->generateRandomPoints(point_number, height, width);
    if (ui->comboBox_2->currentIndex() == 3)
        random_points = ui->Canvas->generateEllipse(point_number, height, width);
    if (ui->comboBox_2->currentIndex() == 4)
        random_points = ui->Canvas->generateStarShape(point_number, height, width);
    if (ui->comboBox_2->currentIndex() == 5)
        random_points = ui->Canvas->generateSquare(point_number, height, width);


    ui->Canvas->setPoints(random_points);
    ui->Canvas->clearCH();
    ui->Canvas->repaint();

}

void Widget::on_pushButton_2_clicked()
{
    if((ui->Canvas->getPoints().size()) < 150000 || !(ui->comboBox->currentIndex() == 3))
    ui->Canvas->clearPoints();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearCH();
}
