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
#include <unistd.h>
//#include "devexcep.hpp"
#include "udpacq.hpp"

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
   ssize_t sz = sendTo(cmd.constData(), cmd.length());
   if (sz > 0)
   {
      sz = recvFrom(bfr, sizeof(bfr));
      if (sz > 0)
      {
         if (sz > 1023) qDebug() << "blew the buffer";
         bfr[sz] = 0;
         return QByteArray(bfr);
      }
      else
         throw StdExceptionBase("UdpAcqDevice::sendCommand recv");
   }
   else
      throw StdExceptionBase("UdpAcqDevice::sendCommand send1");
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

