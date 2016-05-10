#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "plotwidget.h"
#include "plotaccel.h"
#include "kmlmaploader.h"
#include "gpsloggingthread.h"
#include "laptimerthread.h"
#include "plotkmldata.h"
#include "plotkmltrack.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    PlotWidget *widgetPlotter;
    PlotAccel *accelWidgetPlotter;
    PlotKmlData *kmlDataWidgetPlotter;
    PlotKmlTrack *kmlTrackWidgetPlotter;
    QTimer *timer;
    QTime time;
    QStringList currentCoords;
    QStringList currentWaypoints;
    QString satInView;

signals:
    void terminateGPSThread();
    void currentCoordSender(QStringList list);
    void currentSpeedSender(QString speed);
    void terminateTimingThread();

private:
    Ui::MainWindow *ui;
    GPSLoggingThread *GPSThread;
    LapTimerThread *timerThread;
    void clearTime();
    void clearGPS();
private slots:
    void showTime();

    void on_comboBox_currentIndexChanged(const QString &arg1);
    void on_pushButton_4_clicked();
    void onGsvReceived(QStringList dataGsv);
    void onGgaReceived(QStringList dataGga);
    void onVtgReceived(QStringList dataVtg);
    void onSectionInfoReceived(QStringList sectionInfo);
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
    void on_comboBox_2_currentIndexChanged(const QString &arg1);
    void on_pushButton_6_clicked();
    void onStateChangeRequestRcvd(int currentState,int keyIndicator);

    void on_pushButton_7_clicked();
};

#endif // MAINWINDOW_H
