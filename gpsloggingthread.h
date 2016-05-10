#ifndef GPSLOGGINGTHREAD_H
#define GPSLOGGINGTHREAD_H
#include <QThread>
#include "qextserialport.h"
#include <QTimer>
#include <QStringList>
class GPSLoggingThread : public QThread
{
    Q_OBJECT
public:
    explicit GPSLoggingThread(QObject *parent=0);
    ~GPSLoggingThread();
    QByteArray data;
    uint count,countGSV,countVTG,countGGA;
    static const uint FRESHPERIOD = 3;
    QextSerialPort *port;
    QTimer *timer;
    QByteArray getCheckSum(QByteArray msg);

    bool checkNMEASum(QByteArray nmeaLine);

    void run();

private:
    QStringList gsvData(QByteArray nmeaLine);
    QStringList ggaData(QByteArray nmeaLine);
    QStringList vtgData(QByteArray nmeaLine);

public slots:
    void onDataAvailable();
    void onTestTerminated();


signals:
void gsvSender(QStringList dataGsv);
void ggaSender(QStringList dataGga);
void vtgSender(QStringList dataVtg);



};

#endif // GPSLOGGINGTHREAD_H
