#include "plotkmldata.h"
#include "ui_plotkmldata.h"

PlotKmlData::PlotKmlData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotKmlData)
{
    ui->setupUi(this);
    ui->widget->axisRect()->setBackground(QBrush(Qt::black));
    ui->widget->setBackground(QBrush(Qt::black));
    ui->widget->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->widget->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->widget->yAxis2->setBasePen(QPen(Qt::white, 1));
    ui->widget->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->widget->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->widget->yAxis2->setTickPen(QPen(Qt::white, 1));
    ui->widget->xAxis->setTickLabelColor(Qt::white);
    ui->widget->yAxis2->setTickLabelColor(Qt::green);
    ui->widget->yAxis->setTickLabelColor(QColor(255, 100, 0));
    ui->widget->yAxis->setLabelColor(QColor(255, 100, 0));
    ui->widget->yAxis2->setLabelColor(Qt::green);
    ui->widget->setVisible(false);
}

PlotKmlData::~PlotKmlData()
{
    delete ui;

}

void PlotKmlData::plotData(QStringList speed, QStringList alt)

{
    ui->widget->clearGraphs();

    ui->widget->setVisible(true);
    ui->widget->addGraph(ui->widget->xAxis,ui->widget->yAxis);
    ui->widget->graph(0)->setPen(QPen(QColor(255, 100, 0)));
    ui->widget->graph(0)->setLineStyle(QCPGraph::lsLine);

    ui->widget->addGraph(ui->widget->xAxis,ui->widget->yAxis2);
    ui->widget->graph(1)->setPen(QPen(QColor(Qt::green)));
    ui->widget->graph(1)->setLineStyle(QCPGraph::lsLine);


    QVector<double> spd,ele,x;
    double spdMax=0,eleMax=0;
    for (int s=0;s<speed.length();s++)
    {
        spd.append(speed.at(s).toDouble()*3.6);
        spdMax = (spdMax>spd.at(s))?spdMax:spd.at(s);
        ele.append(alt.at(s).toDouble());
        eleMax = (eleMax>ele.at(s))?eleMax:ele.at(s);
        x.append((double)s);
    }

    ui->widget->graph(0)->setData(x,spd);
    ui->widget->graph(1)->setData(x,ele);
    ui->widget->yAxis->setVisible(true);
    ui->widget->yAxis2->setVisible(true);
    ui->widget->xAxis->setVisible(true);
    ui->widget->xAxis2->setVisible(false);
    ui->widget->xAxis->setTickLabels(false);
    ui->widget->yAxis->setTickLabels(true);
    ui->widget->yAxis2->setTickLabels(true);
    ui->widget->xAxis->setAutoTicks(true);


    ui->widget->yAxis->setLabel("Speed (km/h)");
    ui->widget->yAxis2->setLabel("Elevation (m)");

    ui->widget->yAxis->setRange(0,1.1*spdMax);
    ui->widget->yAxis2->setRange(0,1.1*eleMax);
    ui->widget->xAxis->setRange(0,speed.length());

    ui->widget->replot();
}
