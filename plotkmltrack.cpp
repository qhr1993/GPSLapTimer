#include "plotkmltrack.h"
#include "ui_plotkmltrack.h"

#define ZOOM 1.5


PlotKmlTrack::PlotKmlTrack(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlotKmlTrack)
{
    ui->setupUi(this);



    ui->widget->setVisible(false);
}

PlotKmlTrack::~PlotKmlTrack()
{
    delete ui;
}

void PlotKmlTrack::plotTrack(QStringList coordinateStr, QStringList waypointStr, QStringList desString)
{
    QList <double> lon;
    QList <double> lat;
    QList <double> alt;
    QList <double> lonWay;
    QList <double> latWay;
    QList <double> altWay;
    if (coordinateStr.count()>1000)
    {
        double factor = (double)(coordinateStr.count())/1000;
        qWarning()<<factor;

        QStringList tmp;
        for (int i=0;i<1000;i++)
        {
            tmp = coordinateStr.at(qCeil(i*factor)).split(" ");
            lat.append(tmp.at(1).toDouble());
            lon.append(tmp.at(0).toDouble());
            alt.append(tmp.at(2).toDouble());
        }
    }
      else
        {
            QStringList tmp;
            for (int i=0;i<coordinateStr.count();i++)
            {
                tmp = coordinateStr.at(i).split(" ");
                lat.append(tmp.at(1).toDouble());
                lon.append(tmp.at(0).toDouble());
                alt.append(tmp.at(2).toDouble());
            }
        }

    QStringList tmp;
    for (int i=0;i<waypointStr.count();i++)
    {
        tmp = waypointStr.at(i).split(",");
        latWay.append(tmp.at(1).toDouble());
        lonWay.append(tmp.at(0).toDouble());
        altWay.append(tmp.at(2).toDouble());
    }


    QList <double> lonSorted = lon;
    QList <double> latSorted = lat;
    qSort(lonSorted.begin(),lonSorted.end());
    qSort(latSorted.begin(),latSorted.end());
    double maxLon = lonSorted.last();
    double minLon = lonSorted.first();
    double maxLat = latSorted.last();
    double minLat = latSorted.first();

    double latScale = 6378137/360*3.1415926;
    double lonScale = latScale*qCos(qAbs(maxLat)/180*3.1415926);

    QList<double> lonRelative = lon;
    QList<double> lonWayRelative = lonWay;
    QList<double> latRelative = lat;
    QList<double> latWayRelative = latWay;
    QList<double>::iterator i;
    for (i = lonRelative.begin(); i != lonRelative.end(); ++i)
        (*i) = (*i)-minLon;
    for (i = latRelative.begin(); i != latRelative.end(); ++i)
        (*i) = (*i)-minLat;
    for (i = lonWayRelative.begin(); i != lonWayRelative.end(); ++i)
        (*i) = (*i)-minLon;
    for (i = latWayRelative.begin(); i != latWayRelative.end(); ++i)
        (*i) = (*i)-minLat;

    QList<double> xData = latRelative;
    QList<double> xDataWay = latWayRelative;
    QList<double> yData = lonRelative;
    QList<double> yDataWay = lonWayRelative;
    for (i = xData.begin(); i != xData.end(); ++i)
        (*i) = (*i)* latScale;
    for (i = yData.begin(); i != yData.end(); ++i)
        (*i) = (*i)* lonScale;
    for (i = xDataWay.begin(); i != xDataWay.end(); ++i)
        (*i) = (*i)* latScale;
    for (i = yDataWay.begin(); i != yDataWay.end(); ++i)
        (*i) = (*i)* lonScale;



    int length = xData.length();
    double xMax = (maxLat-minLat)*latScale;
    double yMax = (maxLon-minLon)*lonScale;
    QVector <double>  x(length),y(length);
    for (int i=0 ;i<length; i++)
    {
        x[i]=xData.at(i);
        y[i]=yData.at(i);
    }

    int lengthW = xDataWay.length()-1;
    QVector <double>  xW(lengthW),yW(lengthW);
    for (int i=0 ;i<lengthW; i++)
    {
        xW[i]=xDataWay.at(i);
        yW[i]=yDataWay.at(i);
    }
    QVector <double> xS(1),yS(1);
    xS[0]=xDataWay.last();
    yS[0]=yDataWay.last();




ui->widget->setVisible(true);
    ui->widget->clearPlottables();
    QCPCurve *mainGraph = new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    ui->widget->addPlottable(mainGraph);
    QCPCurve *waypointGraph = new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    ui->widget->addPlottable(waypointGraph);
    QCPCurve *startpointGraph = new QCPCurve(ui->widget->xAxis,ui->widget->yAxis);
    ui->widget->addPlottable(startpointGraph);
    waypointGraph->setData(yW,xW);
    mainGraph->setData(y, x);
    startpointGraph->setData(yS,xS);
    waypointGraph->setLineStyle(QCPCurve::lsNone);
    waypointGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::yellow), 9));
    waypointGraph->setPen(QPen(QColor(120, 120, 120), 2));
    startpointGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssSquare, QPen(Qt::black, 1.5), QBrush(Qt::green), 9));
    startpointGraph->setPen(QPen(QColor(120, 120, 120), 2));
   QPen pen;
   pen.setColor(Qt::red);
   pen.setWidth(2);
    mainGraph->setPen(pen);





    ui->widget->axisRect()->setupFullAxesBox();
    ui->widget->axisRect()->setBackground(QBrush(Qt::black));
    ui->widget->setBackground(QBrush(Qt::black));
    ui->widget->axisRect()->axis(QCPAxis::atLeft)->setBasePen(QPen(Qt::gray));
    ui->widget->axisRect()->axis(QCPAxis::atRight)->setBasePen(QPen(Qt::gray));
    ui->widget->axisRect()->axis(QCPAxis::atTop)->setBasePen(QPen(Qt::gray));
    ui->widget->axisRect()->axis(QCPAxis::atBottom)->setBasePen(QPen(Qt::gray));

    ui->widget->axisRect()->axis(QCPAxis::atLeft)->setTicks(false);
    ui->widget->axisRect()->axis(QCPAxis::atRight)->setTicks(false);
    ui->widget->axisRect()->axis(QCPAxis::atTop)->setTicks(false);
    ui->widget->axisRect()->axis(QCPAxis::atBottom)->setTicks(false);
    ui->widget->axisRect()->axis(QCPAxis::atLeft)->grid()->setVisible(false);
    ui->widget->axisRect()->axis(QCPAxis::atBottom)->grid()->setVisible(false);


    double max = (xMax>yMax)?xMax:yMax;
    mainGraph->valueAxis()->setRange(0.5*xMax-ZOOM*0.5*max,0.5*xMax+ZOOM*0.5*max);
    mainGraph->keyAxis()->setRange(0.5*yMax-ZOOM*0.5*max,0.5*yMax+ZOOM*0.5*max);
    waypointGraph->valueAxis()->setRange(0.5*xMax-ZOOM*0.5*max,0.5*xMax+ZOOM*0.5*max);
    waypointGraph->keyAxis()->setRange(0.5*yMax-ZOOM*0.5*max,0.5*yMax+ZOOM*0.5*max);
    startpointGraph->valueAxis()->setRange(0.5*xMax-ZOOM*0.5*max,0.5*xMax+ZOOM*0.5*max);
    startpointGraph->keyAxis()->setRange(0.5*yMax-ZOOM*0.5*max,0.5*yMax+ZOOM*0.5*max);




    for (int s = 0; s<desString.length();s++)
    {
    QCPItemText *text = new QCPItemText(ui->widget);
    ui->widget->addItem(text);
    if (s == (desString.length()-1))
        text->position->setCoords(yS[0],xS[0]);
    else
        text->position->setCoords(yW[s],xW[s]);
    text->setPositionAlignment(Qt::AlignCenter|Qt::AlignBottom);
    text->setText(QString(desString.at(s)).replace("; ","\r\n"));
    text->setTextAlignment(Qt::AlignLeft);
    text->setFont(QFont(font().family(), 6));
    text->setColor(QColor(Qt::white));
    }


    ui->widget->replot();

}
