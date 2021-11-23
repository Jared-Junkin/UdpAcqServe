/*---------------------------------------------------------------------------*
*
*  File:    udpacq.cpp
*  Date:    4/22/2016
*  Author:  Eric Raaen
*
*  Class for handling UDP device sessions
*
*---------------------------------------------------------------------------*/
#include <unistd.h>
#include <QDebug>
#include <except.hpp>
#include <QHostAddress>
#include <QThread>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include "udpacq.hpp"
#include <thread>

namespace DeviceLib
{

//*---------------------------------------------------------------------------
//
// UdpAcqDevice::UdpAcqDevice(addr) 
//
// Construct from host:port, e.g. addr="localhost:9999"
//
//----------------------------------------------------------------------------
UdpAcqDevice::UdpAcqDevice(QString addr) :
   inherited(),
   ip_(),
   port_(0),
   tmo_(400),
   addr_(""),
   sock_(0),
   access_()
{
   setAddress(addr);
}
//*---------------------------------------------------------------------------
UdpAcqDevice::UdpAcqDevice(QString ip, unsigned int port) : 
   inherited(),
   ip_(ip.toLatin1()),
   port_(port),
   tmo_(200),
   addr_(""),
   sock_(0),
   access_()
{
}
//*---------------------------------------------------------------------------
UdpAcqDevice::~UdpAcqDevice() 
{ 
    delete sock_;
}

//*---------------------------------------------------------------------------
void UdpAcqDevice::setAddress(QString addr)
{
   QStringList pip = addr.split(":");
   if (pip.size() == 2)
   {
      ip_ = (pip[0] == "localhost") ?
         QByteArray("127.0.0.1") :
         pip[0].toLatin1();
      port_ = pip[1].toInt();
   }
}

// this function establishes a udpsocket connection between the UdpAcqDevice and (ip_, port_) if one doesn't already exist.
void UdpAcqDevice::initSocket(){
    if(!sock_){
        sock_ = new QUdpSocket();
        QString tmp = QString::fromUtf8(ip_);
        addr_ = QHostAddress(tmp);

        sock_->bind(addr_, port_);
    }
} 
// this function writes cmd to (ip_, port_) after calling initsocket() to establish the udpsocket connection w/ the server and returns the response in a QByteArray
QByteArray UdpAcqDevice::sendCommand(QByteArray cmd)
{
   initSocket();
   sock_->writeDatagram(cmd, addr_, port_);

   int i = 0;
   while(!sock_->hasPendingDatagrams() && i<100){
       QThread::msleep(1);
       i++;
   }

  if(sock_->hasPendingDatagrams()){
       QNetworkDatagram rec = sock_->receiveDatagram();
       return rec.data();
  }
  else{
      throw "TimeoutEx";
  }
}

QByteArray UdpAcqDevice::sendCommand(QString cmd) { return sendCommand(cmd.toLatin1()); }

} // namespace

