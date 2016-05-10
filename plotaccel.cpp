#include "plotaccel.h"
#include "ui_plotaccel.h"

PlotAccel::PlotAccel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotAccel)
{
    ui->setupUi(this);
}

PlotAccel::~PlotAccel()
{
    delete ui;
}

void PlotAccel::plotAxis()
{

    graph = ui->widget->addGraph();
    graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 9));
    graph->setPen(QPen(QColor(120, 120, 120), 2));
    QVector <double> xInit,yInit;
    ui->widget->axisRect()->setupFullAxesBox();
    ui->widget->axisRect()->setAutoMargins(QCP::msNone);
    ui->widget->setBackground(QBrush(Qt::black));
    QMargins margin(2,2,2,2);
    ui->widget->axisRect()->setMargins(margin);
    ui->widget->axisRect()->axis(QCPAxis::atLeft)->setTicks(false);
    ui->widget->axisRect()->axis(QCPAxis::atRight)->setTicks(false);
    ui->widget->axisRect()->axis(QCPAxis::atTop)->setTicks(false);
    ui->widget->axisRect()->axis(QCPAxis::atBottom)->setTicks(false);
    graph->valueAxis()->setRange(-5,5);
    graph->keyAxis()->setRange(-5,5);
    //ui->widget->axisRect()->axis(QCPAxis::atLeft)->grid()->setVisible(false);
    //ui->widget->axisRect()->axis(QCPAxis::atBottom)->grid()->setVisible(false);
    xInit.append(0);
    yInit.append(0);
    graph->setData(xInit,xInit);
}

void PlotAccel::plotAcceleration(QStringList accelData)
{
    QString xAccelStr = accelData.at(0);
    QString yAccelStr = accelData.at(1);
    //QString zAccelStr = accelData.at(2);
    double xAccel = xAccelStr.toDouble();
    double yAccel = yAccelStr.toDouble();
    QVector<double> x,y;
    x.append(xAccel);
    y.append(yAccel);
    graph->setData(x,y);
}
