#include "gpsloggingthread.h"

#include <QTextStream>
#include <QDebug>
#include <QList>
#include <QStringList>


GPSLoggingThread::GPSLoggingThread(QObject *parent):QThread(parent)
{
    qWarning()<<"GPSLoggingThread";

          count=0;
          countGGA=0;
          countVTG=0;
          countGSV=0;

}
GPSLoggingThread::~GPSLoggingThread()
{
}

void GPSLoggingThread::run()
{
    port = new QextSerialPort("/dev/ttyAMA0");
    port->setBaudRate(BAUD9600);
    port->setDataBits(DATA_8);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setStopBits(STOP_1);

    //tmpFile = new QFile("/home/pi/gps_sample_output.txt");
    //tmpFile->open(QIODevice::ReadWrite | QIODevice::Text);
    //outStream = new QTextStream(tmpFile);

    //timer = new QTimer(this);

    connect(port, SIGNAL(readyRead()), this, SLOT(onDataAvailable()));
    //need a button-triggered signal here, slot - onTestTimeReached
    port->open(QIODevice::ReadWrite);
    port->flush();
    //timer->setSingleShot(true);
    //timer->start(10000);
    qWarning()<<"Start receiving.";
//    while(true)
//    {

//    }
    exec();
}


void GPSLoggingThread::onDataAvailable()
      {
          if(port->canReadLine())
          {
          data = port->readLine();
          //qWarning()<<"Good line.";

          if (data.at(0)=='$')
          {

          if (checkNMEASum(data))
          {

         //QString dataString(data);
          QString header(data.left(6));
          int headerNum = -1;
          if (header=="$GPGGA") headerNum=0;
          else if (header=="$GPVTG") headerNum=1;
          else if (header=="$GPGSV") headerNum=2;

          //qWarning()<<"header is"<<headerNum;

          switch (headerNum) {

            case 0 :
              countGGA++;
              if (countGGA%(5*FRESHPERIOD)==0)
                    {
                        emit ggaSender(ggaData(data));
                          count++;
                    }
              break;
            case 1 :
              countVTG++;
              if (countVTG%(5*FRESHPERIOD)==0)
                    {
                        emit vtgSender(vtgData(data));
                          count++;
                    }
                      break;

             case 2 :
              countGSV++;
              if(countGSV%(5*FRESHPERIOD)==0)
               {
                  emit gsvSender(gsvData(data));
                  count++;
               }
                          break;

            default :
              break;

          }


          /*only for testing - poll NMEA data
          if(count==15)
          {
              QByteArray ubx = "$PUBX,04*37\r\n";
              qint64 i;
              i = port->write(ubx);
              qWarning()<<i<<" bytes are written.";
          }
          */


          }
            else
              qWarning()<<"Checksum failed.";
          }

      }
}
void GPSLoggingThread::onTestTerminated()
{
    quit();
    //port->close();
    //delete port;
    qWarning()<<"Thread Terminated";
}


QStringList GPSLoggingThread::ggaData(QByteArray nmeaLine)
{
    // GGA List: 0-time; 1-lat; 2-lon; 3-fix status; 4-sat used; 5-MSL alt; 6-lat(num); 7-lon(num); 8-time(num)
    QString strLine(nmeaLine);
    QStringList tmpList = strLine.split(",");
    QStringList transmittedList;

    if (tmpList.at(1).length() == 0)
        transmittedList.append("UNKNOWN");
    else
    {
        transmittedList.append(tmpList.at(1).left(6));
        transmittedList.last().insert(4,":");
        transmittedList.last().insert(2,":");
    }

    if ((tmpList.at(2).length() == 0 )| (tmpList.at(4).length() == 0))
    {
        transmittedList.append("UNKNOWN");
        transmittedList.append("UNKNOWN");
    }
    else

    {
        QString latDegMin,latSec,lonDegMin,lonSec;
        latDegMin = tmpList.at(2).left(4);
        QChar deg(176);
        latDegMin.insert(2,deg);
        latDegMin.append("\'");
        latSec = tmpList.at(2);
        latSec.remove(0,4);
        latSec.prepend("0");
        latSec = QString::number((latSec.toDouble()*60));
        latSec.append("\"");
        transmittedList.append(latDegMin+latSec);

        if(tmpList.at(3)=="N")
        {
            transmittedList.last() = transmittedList.last().append(" N");
        }
        else if (tmpList.at(3)=="S")
        {
            transmittedList.last() = transmittedList.last().append(" S");
        }



        lonDegMin = tmpList.at(4).left(5);
        lonDegMin.insert(3,deg);
        lonDegMin.append("\'");
        lonSec = tmpList.at(4);
        lonSec.remove(0,5);
        lonSec.prepend("0");
        lonSec = QString::number((lonSec.toDouble()*60));
        lonSec.append("\"");
        transmittedList.append(lonDegMin+lonSec);

        if(tmpList.at(5)=="E")
        {
            transmittedList.last() = transmittedList.last().append(" E");
        }
        else if (tmpList.at(5)=="W")
        {
            transmittedList.last() = transmittedList.last().append(" W");
        }
     }








        if (tmpList.at(6)=="0")
        {
            transmittedList.append("No Fix/Invalid");
        }
        else if (tmpList.at(6)=="1")
        {
            transmittedList.append("Standard Fix");
        }
        else if (tmpList.at(6)=="2")
        {
            transmittedList.append("DGPS Fix");
        }
        else if (tmpList.at(6)=="6")
        {
            transmittedList.append("Estimated Fix");
        }

        transmittedList.append(tmpList.at(7));
        if (tmpList.at(9).length()==0)
        transmittedList.append("UNKNOWN");
        else
        transmittedList.append(tmpList.at(9)+" m");

        if (tmpList.at(3)=="N")
            transmittedList.append(tmpList.at(2));
        else
            transmittedList.append("-"+tmpList.at(2));

        if (tmpList.at(5)=="E")
            transmittedList.append(tmpList.at(4));
        else
           transmittedList.append("-"+tmpList.at(4));

        transmittedList.append(tmpList.at(1));
        return transmittedList;
}


QStringList GPSLoggingThread::vtgData(QByteArray nmeaLine)
    {
        // VTG List: 0-course; 1-speed; 2-validity; 3-speed(num)
        QString strLine(nmeaLine);
        QStringList tmpList = strLine.split(",");
        QStringList transmittedList;

        QChar deg(176);
        if (tmpList.at(1).length()==0)
        transmittedList.append("UNKNOWN");
        else
        transmittedList.append(tmpList.at(1)+deg);

        if (tmpList.at(7).length()==0)
        transmittedList.append("UNKNOWN");
        else
        transmittedList.append(tmpList.at(7)+" km/h");

        if ((tmpList.at(9)=="N")|(tmpList.at(9)=="E"))
        {
            transmittedList.append("Invalid");
        }
        else
        {
            transmittedList.append("Valid");
        }

        transmittedList.append(tmpList.at(7));
        return transmittedList;

    }

QStringList GPSLoggingThread::gsvData(QByteArray nmeaLine)
    {
        // GSV List: 0-sat in view; 1*N-sat number; 2*N-elevation; 2*N-azimuth; 4*N-CN0

        QString strLine(nmeaLine);
        QStringList tmpList = strLine.split(",");
        QStringList transmittedList;

//        int num =(tmpList.length()-5)/4;
        transmittedList.append(tmpList.at(3));
//        QChar deg(24);
//        for (int i=0;i<num;i++)
//        {
//            transmittedList.append(tmpList.at(4+4*i));
//            transmittedList.append(tmpList.at(5+4*i)+deg);
//            transmittedList.append(tmpList.at(6+4*i)+deg);

//            if (tmpList.at(7+4*i).length()==0)
//            transmittedList.append("UNAVAILABLE");
//            else
//            transmittedList.append(tmpList.at(7+4*i)+" dBHz");
//        }
    return transmittedList;
    }

bool GPSLoggingThread::checkNMEASum(QByteArray nmeaLine)
{
    QString strLine(nmeaLine);
    QStringList tmpList = strLine.split("*");
    QByteArray payload(tmpList.at(0).toUtf8());
    QByteArray checkSumChar(tmpList.at(1).left(2).toUtf8());
    char tmp=0;
    for (int i=1;i<payload.length();i++)
    {
        tmp  = tmp ^ payload.at(i);
    }

    return (tmp == *(QByteArray::fromHex(checkSumChar).data()));
}
