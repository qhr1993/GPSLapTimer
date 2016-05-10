#ifndef PLOTKMLDATA_H
#define PLOTKMLDATA_H

#include "qcustomplot.h"

#include <QWidget>

namespace Ui {
class PlotKmlData;
}

class PlotKmlData : public QWidget
{
    Q_OBJECT

public:
    explicit PlotKmlData(QWidget *parent = 0);
    void plotData(QStringList speed,QStringList alt);
    ~PlotKmlData();

private:
    Ui::PlotKmlData *ui;
};

#endif // PLOTKMLDATA_H
