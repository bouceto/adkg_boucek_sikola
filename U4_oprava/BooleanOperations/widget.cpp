#include "widget.h"
#include "ui_widget.h"
#include "algorithms.h"
#include "types.h"
#include <QFileDialog>

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
    ui->Canvas->changePolygon();
}

void Widget::on_pushButton_2_clicked()
{
    //Get polygons
    std::vector<QPointFB> polA = ui->Canvas->getA();
    std::vector<QPointFB> polB = ui->Canvas->getB();

    //Perform Boolean operation
    TBooleanOperation oper = (TBooleanOperation)(ui->comboBox->currentIndex());
    std::vector<Edge> res = Algorithms::booleanOperations(polA, polB, oper);

    std::vector<QPointFB> singVec;
    if(res.empty()){
        for(int i =0; i < polA.size(); i++){
            for(int j = 0; j<polB.size(); j++){

                if(polA[i] == polB[j]){
                    singVec.push_back(polA[i]);
                }

            }
        }
    }
    if (singVec.size() > 1)
    {
        for (int i = 0; i < singVec.size(); i++)
        {
            Edge e(singVec[i], singVec[i+1]);
            res.push_back(e);
            if (res.size() == singVec.size() - 1)
                break;
        }
    }

    if (singVec.size() == 1)
        ui->Canvas->setResPoint(singVec[0]);


    //Set results and update
    ui->Canvas->setRes(res);
    repaint();
}

void Widget::on_pushButton_3_clicked()
{
    ui->Canvas->clearResults();
    repaint();
}

void Widget::on_pushButton_4_clicked()
{
    ui->Canvas->clearAll();
    repaint();
}

void Widget::on_pushButton_5_clicked()
{
    QString path(QFileDialog::getOpenFileName(this, tr("Open file with polygons"), "../", tr("Text Files (*.txt)")));
    std::string path_std = path.toStdString();
    ui->Canvas->loadFile(path_std);
}
