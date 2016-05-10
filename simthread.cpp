#include "simthread.h"

SIMThread::SIMThread(QObject *parent):QThread(parent)
{

}

void SIMThread::run()
{
    exec;
}


