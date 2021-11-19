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
//#include "devexcep.hpp"
#include "udpacq.hpp"
#include <chrono>
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

//quint16 UdpAcqDevice::getPort(){
//    return port_;
//}

//QByteArray UdpAcqDevice::getIP(){
//    return ip_;
//}

//QString UdpAcqDevice::test(){
//    return port_;
//}

//*---------------------------------------------------------------------------
//  UdpAcqDevice::initSocket()
//
// Closes and frees fd_ and ai_ so caller should check if they want to keep them
//---------------------------------------------------------------------------

// I can't do this (as far as I know) unless i either 1) modify udpacq.h to all me to return something from this function, or 2) modify udpacq.h to make udpsocket_ a global variable or 3) make a wrapper class that explicitly
// overrides this function in initsocket!
void UdpAcqDevice::initSocket()
{
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
   const char* hostname = ip_.constData();
   QByteArray port = QByteArray::number(port_);
   qDebug() << "init" << hostname << port;
   struct addrinfo hints = {};
   hints.ai_family=AF_UNSPEC;
   hints.ai_socktype=SOCK_DGRAM;
   hints.ai_protocol=IPPROTO_UDP;
   hints.ai_flags=AI_ADDRCONFIG;
   int rc = getaddrinfo(hostname, port.constData(), &hints, &ai_);
   if (rc)
   {
      qDebug() << "getaddr error" << gai_strerror(rc);
      throw StdExceptionBase("UdpAcqDevice::initSocket 1");
   }
   qDebug() << "get socket";
   fd_ = socket(ai_->ai_family, ai_->ai_socktype, ai_->ai_protocol);
   if (fd_ == -1)
   {
      qDebug() << "socket open error";
      throw StdExceptionBase("UdpAcqDevice::initSocket 2");
   }
   struct timeval tv = {};
   tv.tv_sec = 0;
   tv.tv_usec = tmo_ * 1000;
   rc = setsockopt(fd_, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
   if (rc)
      qDebug() << "setsockopt error" << rc;

}
//*---------------------------------------------------------------------------
// UdpAcqDevice::sendTo(bfr, sz)
// UdpAcqDevice::recvFrom(bfr, sz)
//
// Checks fd_ and calls initSocket() if necessary
//---------------------------------------------------------------------------
ssize_t UdpAcqDevice::sendTo(const void *bfr, size_t sz)
{
   if (fd_ == -1) initSocket();
   return sendto(fd_, bfr, sz, 0, ai_->ai_addr, ai_->ai_addrlen);
}
ssize_t UdpAcqDevice::recvFrom(void *bfr, size_t sz)
{
   if (fd_ == -1) initSocket();
   return recvfrom(fd_, bfr, sz, 0, ai_->ai_addr, &ai_->ai_addrlen);
}
//*---------------------------------------------------------------------------
QByteArray UdpAcqDevice::sendCommand(QByteArray cmd)
{
   char bfr[1024];
   QUdpSocket * udpsocket_ = new QUdpSocket(this);
   udpsocket_->bind(QHostAddress::LocalHost, port_);

   udpsocket_->writeDatagram(cmd, QHostAddress::LocalHost, port_);

   // this works fine in qDebug mode but it fails when I run it live and I truly don't fucking understand why
   const QString &tmp = QString::fromUtf8(ip_);
   QHostAddress *addr = new QHostAddress(tmp);

   int i = 0;
   while(!udpsocket_->hasPendingDatagrams() && i<100){
       QThread::msleep(1);
       i++;
   }

   int x = 1;

   if(udpsocket_->hasPendingDatagrams()){
       x = udpsocket_->readDatagram(bfr,sizeof(bfr), addr, &port_);
  }

  QByteArray ret;
  char bfr2[x-1];
//  memcpy(bfr2, bfr, sizeof(bfr2));

  qDebug() << x-1;
  for(int s = 0; s<x-1; s++){
      qDebug() << "bfr[s] = " << bfr[s];
      bfr2[s] = bfr[s];
  }
  qDebug() << "where the fuck is this extra shit coming from?";

  ret = QByteArray(bfr2);
  delete addr;
  return ret;

//  ret = QByteArray(bfr);
//  delete addr;
//  return ret;
}

#if 0 // old way
   QElapsedTimer tmo;
   tmo.start();
   if (sock()->writeDatagram(cmd, addr_, port_) > 0)
   {
      while (!tmo.hasExpired(tmo_))
      {
         if (sock_->hasPendingDatagrams())
         {
            qint64 sz = sock_->readDatagram(bfr, 1024, &addr_, &port_);
            if (sz > 0)
            {
               bfr[sz] = 0;
               return QByteArray(bfr);
            }
         }
      }
      throw TimeoutException("UdpAcqDevice::sendCmd");
   }
   throw DeviceCommunicationError("UdpAcqDevice::sendCmd");
#endif

QByteArray UdpAcqDevice::sendCommand(QString cmd) { return sendCommand(cmd.toLatin1()); }


//*---------------------------------------------------------------------------
QByteArray UdpAcqDevice::recv(unsigned int maxSize)
{  
   char bfr[maxSize];
   ssize_t sz = recvFrom(bfr, maxSize);
   if (sz > 0)
   {
      bfr[sz] = 0;
      return QByteArray(bfr);
   }
   throw StdExceptionBase("UdpAcqDevice::recv");
}

} // ns

