#ifndef PLOTKMLTRACK_H
#define PLOTKMLTRACK_H

#include "qcustomplot.h"


#include <QWidget>

namespace Ui {
class PlotKmlTrack;
}

class PlotKmlTrack : public QWidget
{
    Q_OBJECT

public:
    explicit PlotKmlTrack(QWidget *parent = 0);
    void plotTrack(QStringList coordinateStr,QStringList waypointStr,QStringList desString);
    ~PlotKmlTrack();
    double currentLatMax,currentLatMin,currentLonMax,currentLonMin,currentLonScale,currentLatScale;
    double currentXMax,currentYMax;

private:
    Ui::PlotKmlTrack *ui;
};

#endif // PLOTKMLTRACK_H
