#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kmlmaploader.h"
#include "laptimerthread.h"
#include "plotaccel.h"
#include <QDebug>
#include <QString>
#include <wiringPi/wiringPi.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    widgetPlotter = new PlotWidget(ui->tabWidget->widget(1));
    kmlDataWidgetPlotter = new PlotKmlData(ui->tabWidget->widget(2));
    kmlTrackWidgetPlotter = new PlotKmlTrack(ui->tabWidget->widget(2));
    kmlTrackWidgetPlotter->setGeometry(400,10,240,240);
    kmlDataWidgetPlotter->setGeometry(360,260,400,160);
    system("gpio export 18 out");

    QDir dir("/home/pi/KmlFiles");
    QStringList filters;
    filters<<"*.kml";
    QFileInfoList fileList = dir.entryInfoList(filters, QDir::Files,QDir::Name);
    QStringList fileNameList;
    //QFileInfoList::iterator i;
    for(int i = 0;i<fileList.count();++i)
    {
        fileNameList.append(fileList.at(i).fileName());
    }

    ui->comboBox->addItems(fileNameList);

    QDir dirProj("data/");
    QFileInfoList fileListProj = dirProj.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot,QDir::Name);
    QStringList fileNameListProj;
    for(int i = 0;i<fileListProj.count();++i)
    {
        fileNameListProj.append(fileListProj.at(i).fileName());
    }

    ui->comboBox_2->addItems(fileNameListProj);

    QPalette palette;
    palette.setColor(QPalette::Background,QColor(0,0,0));
    palette.setColor(QPalette::Text,QColor(255,255,255));
    palette.setColor(QPalette::Foreground,QColor(192,192,192));
    palette.setColor(QPalette::Base,Qt::black);
    palette.setColor(QPalette::AlternateBase,Qt::gray);
    palette.setColor(QPalette::Button,Qt::gray);
    ui->centralWidget->setAutoFillBackground(true);
    ui->tabWidget->setAutoFillBackground(true);
    ui->tabWidget->tabBar()->setShape(QTabBar::TriangularNorth);
    ui->centralWidget->setPalette(palette);
    //ui->tabWidget->clear();
    ui->label->setPalette(palette);

    ui->label_2->setText("GPS Status: UNKNOWN  No. of Satellites: -- ");
    ui->label_17->setText("GPS OFF");
    ui->pushButton_3->setEnabled(false);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start(1000);
    time = QTime::currentTime();
    ui->label_3->setText(time.toString("hh:mm"));

    ui->tabWidget->setTabText(0," General Info ");
    ui->tabWidget->setTabText(1," Driving Mode ");
    ui->tabWidget->setTabText(2," Viewing Record");

    QFont font = ui->tabWidget->font();
    font.setPointSize(12);
    font.setBold(false);
    font.setFamily("Monospace");
    ui->tabWidget->setFont(font);

    ui->tabWidget->widget(0)->setAutoFillBackground(true);
    ui->tabWidget->widget(1)->setAutoFillBackground(true);
    ui->tabWidget->widget(2)->setAutoFillBackground(true);

    QIcon iconGreen("on.svg");
    QIcon iconRed("off.svg");
    //iconGreen.scaled(iconSize);
    //iconRed.scaled(iconSize);
    ui->pushButton_4->setIcon(iconRed);
    ui->pushButton_4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred );

    accelWidgetPlotter = new PlotAccel(ui->tabWidget->widget(1));
    accelWidgetPlotter->setGeometry(650,30,100,100);
    accelWidgetPlotter->plotAxis();

    ui->lcdNumber->setDecMode();
    ui->lcdNumber->setDigitCount(3);
    satInView = "--";


    wiringPiSetupSys();
    //pinMode(1,OUTPUT);
    GPSThread = new GPSLoggingThread(this);
    timerThread = new LapTimerThread(this);
    connect(ui->tab,SIGNAL(stateChangeRequest(int,int)),this,SLOT(onStateChangeRequestRcvd(int,int)));
    connect(ui->tabRecord,SIGNAL(stateChangeRequest(int,int)),this,SLOT(onStateChangeRequestRcvd(int,int)));
    connect(ui->tab_2,SIGNAL(stateChangeRequest(int,int)),this,SLOT(onStateChangeRequestRcvd(int,int)));

   connect(this, SIGNAL(terminateGPSThread()), GPSThread, SLOT(onTestTerminated()));
   connect(this,SIGNAL(currentCoordSender(QStringList)),timerThread, SLOT(onCurrentCoordReceived(QStringList)));
   connect(this,SIGNAL(currentSpeedSender(QString)),timerThread, SLOT(onCurrentSpeedReceived(QString)));
   connect(GPSThread, SIGNAL(gsvSender(QStringList)),this,SLOT(onGsvReceived(QStringList)));
   connect(GPSThread, SIGNAL(ggaSender(QStringList)),this,SLOT(onGgaReceived(QStringList)));
   connect(GPSThread, SIGNAL(vtgSender(QStringList)),this,SLOT(onVtgReceived(QStringList)));
   connect(this,SIGNAL(terminateTimingThread()),timerThread, SLOT(onThreadTerminated()));
   connect(timerThread,SIGNAL(sectionInfoSender(QStringList)),this,SLOT(onSectionInfoReceived(QStringList)));
   QList <QPushButton *> btnList= ui->centralWidget->findChildren<QPushButton *>();
   QList <QComboBox *> combxList = ui->centralWidget->findChildren<QComboBox *>();
   QList <QPushButton *>::iterator iBtn;
   QList <QComboBox *>::iterator iCombx;
   for (iBtn = btnList.begin();iBtn!=btnList.end();++iBtn)
   {
       (**iBtn).setStyleSheet("QPushButton{ background-color: white; } QPushButton:disabled{ background-color: grey;  } QPushButton:focus:pressed{ background-color: black; } QPushButton:focus{ background-color: orange; }");
       connect((*iBtn),SIGNAL(stateChangeRequest(int,int)),this,SLOT(onStateChangeRequestRcvd(int,int)));
   }
   for (iCombx = combxList.begin();iCombx!=combxList.end();++iCombx)
   {
       (**iCombx).setStyleSheet("QComboBox{ background-color: white; } QComboBox:focus{ background-color: orange; }QComboBox QAbstractListView{selection-background-color: orange;}");
       connect((*iCombx),SIGNAL(stateChangeRequest(int,int)),this,SLOT(onStateChangeRequestRcvd(int,int)));
   }


   ui->comboBox->state = 111;
   ui->comboBox_2->state = 311;
   ui->comboBox_3->state = 321;
   ui->comboBox_4->state = 132;
   ui->comboBox_5->state = 112;
   ui->pushButton->state = 211;
   ui->pushButton_2->state = 212;
   ui->pushButton_3->state = 213;
   ui->pushButton_4->state = 121;
   ui->pushButton_5->state = 122;
   ui->pushButton_6->state = 331;
   ui->pushButton_7->state = 142;
   (dynamic_cast<CWidget*>(ui->tabWidget->widget(0)))->state=100;
   (dynamic_cast<CWidget*>(ui->tabWidget->widget(1)))->state=200;
   (dynamic_cast<CWidget*>(ui->tabWidget->widget(2)))->state=300;

   ui->tabRecord->setFocus();

}

void MainWindow::onGsvReceived(QStringList dataGsv)
{
 qWarning()<<"Hi I am GSV";
 ui->tableWidget->item(5,0)->setText(dataGsv.at(0));//set SAT IN VIEW
 satInView = dataGsv.at(0);

}

void MainWindow::onGgaReceived(QStringList dataGga)
{
    //qWarning()<<"Hi I am GGA";
    ui->tableWidget->item(0,0)->setText(dataGga.at(1));//set LAT
    ui->tableWidget->item(1,0)->setText(dataGga.at(2));//set LONG
    ui->tableWidget->item(7,0)->setText(dataGga.at(3));//set FIX STATUS
    ui->label_2->setText("GPS Status: " + dataGga.at(3) + "  No. of Satellites: "+satInView);
    ui->tableWidget->item(6,0)->setText(dataGga.at(4));//set SAT IN USE
    ui->tableWidget->item(2,0)->setText(dataGga.at(5));//set ALT
    qWarning()<<dataGga.at(0);
    time.setHMS(dataGga.at(0).split(":").at(0).toDouble(),
                dataGga.at(0).split(":").at(1).toDouble(),
                dataGga.at(0).split(":").at(2).toDouble());//set TIME to system

    widgetPlotter->plotPosition(dataGga.at(6),dataGga.at(7));
    if (timerThread->isRunning())
    {
        QStringList tmp= dataGga.mid(6,-1);
        tmp.append(dataGga.at(5));
        emit currentCoordSender(tmp);
    }
}

void MainWindow::onVtgReceived(QStringList dataVtg)
{
    //qWarning()<<"Hi I am VTG";
    ui->tableWidget->item(4,0)->setText(dataVtg.at(0));//set COURSE
    ui->tableWidget->item(3,0)->setText(dataVtg.at(1));//set SPEED
    if (timerThread->isRunning())
    {
        emit currentSpeedSender(dataVtg.last());
    }
    ui->lcdNumber->display((int) dataVtg.at(1).toDouble() );
}


void MainWindow::showTime()
{

    ui->label_3->setText(time.toString("hh:mm"));//initialise time
    //qWarning()<<"t";
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete accelWidgetPlotter;
    delete GPSThread;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    delete widgetPlotter;
    widgetPlotter = new PlotWidget(ui->tabWidget->widget(1));

    KMLMapLoader testLoader;
    testLoader.loadFile(ui->comboBox->currentText());
    testLoader.parseFile();
    QStringList testCoord;
    QStringList testWaypoints;//0-start/end 1-waypoint1 ...
    QList <KMLData>::iterator i;
    QList <QString>::iterator j;
        qWarning()<<"done";
        qWarning()<<testLoader.contentHandler->KMLDataList.count();
    for (i=testLoader.contentHandler->KMLDataList.begin();
         i!=testLoader.contentHandler->KMLDataList.end();++i)
    {
        if (i->parentName == "gx:Track")
        {
            for (j=i->dataInfo.begin();j!=i->dataInfo.end();++j)
                //qWarning()<<(*j);
                testCoord.append(*j);
        }
        if (i->parentName == "Point")
        {
            for (j=i->dataInfo.begin();j!=i->dataInfo.end();++j)
                //qWarning()<<(*j);
                testWaypoints.append(*j);
        }
    }
    testCoord.append(testCoord.first());
    widgetPlotter->plotTrack(testCoord,testWaypoints);
    currentCoords = testCoord;
    currentWaypoints = testWaypoints;
}

void MainWindow::on_pushButton_4_clicked()
{
    //power on
    //
    QIcon iconGreen("on.svg");
    QIcon iconRed("off.svg");

    qWarning()<<(digitalRead(18));
    if (digitalRead(18)==LOW)
    {
        digitalWrite (18, HIGH);
        GPSThread->start(QThread::HighPriority);
        ui->label_17->setText("GPS ON");
        //iconGreen.scaled(iconSize);
        //iconRed.scaled(iconSize);
        ui->pushButton_4->setIcon(iconGreen);
        ui->pushButton_4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred );

    }
    else if (digitalRead(18)==HIGH)
    {
        //qWarning()<<GPSThread->isFinished();
        emit terminateGPSThread();
        //qWarning()<<GPSThread->isFinished();
        digitalWrite (18, LOW);
        ui->label_17->setText("GPS OFF");
        clearGPS();
        //iconGreen.scaled(iconSize);
        //iconRed.scaled(iconSize);
        ui->pushButton_4->setIcon(iconRed);
        ui->pushButton_4->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred );
  }
}

void MainWindow::on_pushButton_clicked()//initiate lap timer
{
    timerThread->initiate(currentCoords,currentWaypoints,false,ui->comboBox->currentText());
    timerThread->start(QThread::HighPriority);
    ui->pushButton->setEnabled(false);
    ui->pushButton_3->setEnabled(true);    
}

void MainWindow::on_pushButton_3_clicked()//stop lap timer
{
    emit terminateTimingThread();
    ui->pushButton->setEnabled(true);
    ui->pushButton_3->setEnabled(false);
    clearTime();
}

void MainWindow::onSectionInfoReceived(QStringList sectionInfo)
{
    switch(sectionInfo.at(0).toUInt())
    {
        case 0:
        ui->label_6->setText(sectionInfo.at(1));
        ui->label_25->setText(sectionInfo.at(3));
        ui->label_23->setText(sectionInfo.at(2));
        ui->label_10->setText(sectionInfo.at(4));
        break;
        case 1:
        ui->label_7->setText(sectionInfo.at(1));
        ui->label_26->setText(sectionInfo.at(3));
        ui->label_5->setText(sectionInfo.at(2));
        ui->label_10->setText(sectionInfo.at(4));
        break;
        case 2:
        ui->label_8->setText(sectionInfo.at(1));
        ui->label_24->setText(sectionInfo.at(3));
        ui->label_19->setText(sectionInfo.at(2));
        ui->label_10->setText(sectionInfo.at(4));
        break;
        case 3:
        ui->label_9->setText(sectionInfo.at(1));
        ui->label_27->setText(sectionInfo.at(3));
        ui->label_21->setText(sectionInfo.at(2));
        ui->label_10->setText(sectionInfo.at(4));
        break;

    }
}

void MainWindow::clearTime()
{
    ui->label_6->setText("**:**.***");
    ui->label_25->setText("-**.***");
    ui->label_23->setText("**:**.***");
    ui->label_7->setText("**:**.***");
    ui->label_26->setText("-**.***");
    ui->label_5->setText("**:**.***");
    ui->label_8->setText("**:**.***");
    ui->label_24->setText("-**.***");
    ui->label_19->setText("**:**.***");
    ui->label_9->setText("**:**.***");
    ui->label_27->setText("-**.***");
    ui->label_21->setText("**:**.***");
    ui->label_10->setText("");
}

void MainWindow::clearGPS()
{
    ui->tableWidget->clearContents();
    ui->label_2->setText("GPS Status: UNKNOWN  No. of Satellites: -- ");
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    QDir dirLap("data/"+arg1+"/");
    QStringList filtersLap;
    filtersLap<<"*.kml";
    QFileInfoList fileListLap = dirLap.entryInfoList(filtersLap, QDir::Files,QDir::Name);
    QStringList fileNameListLap;
    //QFileInfoList::iterator i;
    for(int i = 0;i<fileListLap.count();++i)
    {
        fileNameListLap.append(fileListLap.at(i).fileName());
    }

    ui->comboBox_3->addItems(fileNameListLap);
}

void MainWindow::on_pushButton_6_clicked()
{
    QFile txtFile("data/"+ui->comboBox_2->currentText()+"/"+"log.txt");
    if (!txtFile.exists())
    {
        ui->textBrowser->setText("Log file not found.");
    }
    else
    {
        txtFile.open(QIODevice::ReadOnly|QIODevice::Text);
        QString info;
        QTextStream txtStream(&txtFile);
        QRegExp reg("[-\.]");
        info = txtStream.readAll().section("_",ui->comboBox_3->currentText().section(reg,1,1).toInt()
                                           ,ui->comboBox_3->currentText().section(reg,1,1).toInt());
        qWarning()<<ui->comboBox_3->currentText().section(reg,1,1).toInt();
        ui->textBrowser->setText("Record Info: \r\n"+info);
        txtFile.close();
    }



    QFile kmlFile("data/"+ui->comboBox_2->currentText()+"/"+ui->comboBox_3->currentText());
    if (!kmlFile.exists())
    {
       //
    }
    else
    {
       kmlFile.open(QIODevice::ReadOnly|QIODevice::Text);
       QXmlStreamReader kmlStreamReader(&kmlFile);
       QStringList tmpCoord,tmpSpd,tmpAlt,tmpWaypt,tmpDes;
       bool altFlag = false, speedFlag = false,wayptFlag = false;
       kmlStreamReader.readNext();
       //Reading from the file
       while (!kmlStreamReader.isEndDocument())
       {
           if (kmlStreamReader.isStartElement())
           {
               QString name = kmlStreamReader.name().toString();
               if (name == "coord")//read time of each element
               {
                  tmpCoord.append(kmlStreamReader.readElementText());
                  tmpAlt.append(tmpCoord.last().section(" ",2,2));
               }
               else if ((name == "value")&& speedFlag )//read best of each element
               {
                   tmpSpd.append(kmlStreamReader.readElementText());
               }
               else if ((name == "value")&& altFlag)//read when of each element
               {
                  //tmpAlt.append(kmlStreamReader.readElementText());
               }
               else if ((name=="coordinates")&& wayptFlag)
               {
                   tmpWaypt.append(kmlStreamReader.readElementText());
               }
               else if ((name == "description")&& wayptFlag)
               {
                   tmpDes.append(kmlStreamReader.readElementText());
               }
               else if (name =="SimpleArrayData")
               {
                   if (kmlStreamReader.attributes().value("name").toString()=="speed")
                       speedFlag  = true;
                   else if (kmlStreamReader.attributes().value("name").toString()=="accuracy")
                       altFlag = true;
                   else
                   {
                       speedFlag = false;
                       altFlag = false;
                   }
               }
               else if ((name =="Placemark")&&(kmlStreamReader.attributes().value("id").toString()!="tour"))
               {
                   wayptFlag = true;
               }
           }
           else if (kmlStreamReader.isEndElement())
           {
               QString name  = kmlStreamReader.name().toString();
               if (name == "SimpleArrayData")
               {
                   speedFlag = false;
                   altFlag = false;
               }
               else if (name =="Placemark")
               {
                   wayptFlag = false;
               }

           }
           kmlStreamReader.readNext();
       }
       kmlTrackWidgetPlotter->plotTrack(tmpCoord,tmpWaypt,tmpDes);
       kmlDataWidgetPlotter->plotData(tmpSpd,tmpAlt);

    }

}

void MainWindow::onStateChangeRequestRcvd(int currentState, int keyIndicator)
{
    int nextState;
    switch (keyIndicator)
    {
    case 3:
    nextState = currentState-1;
    break;
    case 4:
    nextState = currentState + 1;
    break;
    case 2:
        nextState = currentState + 10;
    break;
    case 1:
        nextState = currentState - 10;
    break;
    case -1:
        nextState = currentState/100*100;
    break;
    case 0:
        nextState = currentState + 11;
        break;
    default:
        nextState = 0;
        break;
    }


        switch (nextState)
        {
        case 100:
            ui->tabRecord->setFocus();
            break;
        case 200:
            ui->tab->setFocus();
            break;
        case 300:
            ui->tab_2->setFocus();
            break;
        case 111:
            ui->comboBox->setFocus();
            break;
        case 121:
            ui->pushButton_4->setFocus();
            break;
        case 112:
            ui->comboBox_5->setFocus();
            break;
        case 122:
            ui->pushButton_5->setFocus();
            break;
        case 132:
            ui->comboBox_4->setFocus();
            break;
        case 142:
            ui->pushButton_7->setFocus();
            break;
        case 211:
            ui->pushButton->setFocus();
            break;
        case 212:
            ui->pushButton_2->setFocus();
            break;
        case 213:
            ui->pushButton_3->setFocus();
            break;
        case 311:
            ui->comboBox_2->setFocus();
            break;
        case 321:
            ui->comboBox_3->setFocus();
            break;
        case 331:
            ui->pushButton_6->setFocus();
            break;
        default:
            nextState = 0;
            break;
        }


}
