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
    Acqtester::Acqtester(UdpAcqDevice &dev, int durationSeconds, QString command): myDev(dev){
                duration_ = durationSeconds;
                text_ = "";
                cmd_ = command;
    }

    void Acqtester::run(){
        std::time_t endTime = time(NULL) + duration_;
        while(time(NULL) < endTime && text_.length() == 0){
            try{
                QMutex mutex;
                QMutexLocker locker(&mutex);
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

    QString Acqtester::text(){
        return text_;
    }

}


// just realized I'm going to need to have all of this in mainwindow...
