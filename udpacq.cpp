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
   ai_(0),
   fd_(-1),
   sock(0),
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
   ai_(0),
   fd_(-1),
   sock(0),
   access_()
{
}
//*---------------------------------------------------------------------------
UdpAcqDevice::~UdpAcqDevice() 
{ 
   if (ai_)
      freeaddrinfo(ai_);
   if (fd_ != -1)
      close(fd_);
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
      if (fd_ != -1)
      {
         close(fd_);
         fd_ = -1;
      }
      if (ai_)
      {
         freeaddrinfo(ai_);
         ai_ = 0;
      }
   }
}

void UdpAcqDevice::initSocket(){
    if(!sock){
        sock = new QUdpSocket();
        const QString &tmp = QString::fromUtf8(ip_);
        QHostAddress addr = QHostAddress(tmp);

        QHostAddress &addrRef = addr;
        sock->bind(addrRef, port_);
    }
}
// this function establishes a udpsocket connection between the UdpAcqDevice and (ip_, port_) if one doesn't already exist.
// then it writes cmd to (ip_, port_) and returns the response in a QByteArray
QByteArray UdpAcqDevice::sendCommand(QByteArray cmd)
{
   initSocket();
   const QString &tmp = QString::fromUtf8(ip_);
   QHostAddress addr = QHostAddress(tmp);

   QHostAddress &addrRef = addr;
   sock->writeDatagram(cmd, addrRef, port_);

   int i = 0;
   while(!sock->hasPendingDatagrams() && i<100){
       QThread::msleep(1);
       i++;
   }

   if(sock->hasPendingDatagrams()){
       QNetworkDatagram rec = sock->receiveDatagram();
       return rec.data();
  }

  QString err("Error, no data received");
  QByteArray ret = err.toUtf8();
  return ret;
}

QByteArray UdpAcqDevice::sendCommand(QString cmd) { return sendCommand(cmd.toLatin1()); }


} // namespace

