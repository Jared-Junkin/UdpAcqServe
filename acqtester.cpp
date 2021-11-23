#include "acqtester.h"
#include <time.h>
#include <iostream>
#include "udpacq.hpp"
#include <QMutex>
#include <QMutexLocker>
#include <QByteArray>
#include <QDebug>


using namespace std;

namespace DeviceLib{
Acqtester::Acqtester(UdpAcqDevice &dev, int durationSeconds, QString command):
    myDev(dev),
    duration_(durationSeconds),
    text_(""),
    cmd_(command){}

// this function sends cmd_ repeatedly for duration_ seconds. Each time the response is assigned to variable text_.
void Acqtester::run(){
    std::time_t endTime = time(NULL) + duration_;
    while(time(NULL) < endTime){
        try{
            QMutexLocker locker(&myDev.access());
            QByteArray recv = myDev.sendCommand(cmd_);
            QString resp = QString(recv);
            text_ = resp;
        }
        catch(std::exception&ex){
            cerr << "Exception: " << ex.what() << endl;
            cout << "Thread died" << endl;
            return;
        }
        msleep(100);
    }
}

// this function returns text_, because it is a private variable.
QString Acqtester::text(){
    return text_;
}
} //namespace
