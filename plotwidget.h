#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QWidget
{
    Q_OBJECT

public:

    explicit PlotWidget(QWidget *parent = 0);
    void plotTrack(QStringList coordinateStr,QStringList waypointStr);
    void plotPosition(QString lat, QString lon);
    ~PlotWidget();
    double currentLatMax,currentLatMin,currentLonMax,currentLonMin,currentLonScale,currentLatScale;
    double currentXMax,currentYMax;
    QCPCurve *positionGraph;




private:
    Ui::PlotWidget *ui;
};

#endif // PLOTWIDGET_H
