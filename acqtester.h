#ifndef ACQTESTER_H
#define ACQTESTER_H

#include "udpacq.hpp"
#include <QThread>
#include <QString>


namespace DeviceLib{
    class Acqtester: public QThread{
    Q_OBJECT
    public:
       Acqtester(UdpAcqDevice &dev, int durationSeconds, QString command);
       QString text();
       void run() override;
    private:
       UdpAcqDevice &myDev;
       time_t duration_;
       QString text_;
       QString cmd_;
    };
}


#endif // ACQTESTER_H
