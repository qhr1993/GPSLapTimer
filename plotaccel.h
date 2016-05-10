#ifndef PLOTACCEL_H
#define PLOTACCEL_H

#include <QWidget>
#include <qcustomplot.h>

namespace Ui {
class PlotAccel;
}

class PlotAccel : public QWidget
{
    Q_OBJECT

public:
    explicit PlotAccel(QWidget *parent = 0);
    ~PlotAccel();
    void plotAcceleration(QStringList accelData);
    void plotAxis();
    QCPGraph *graph;

private:
    Ui::PlotAccel *ui;
};

#endif // PLOTACCEL_H
