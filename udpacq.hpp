/*---------------------------------------------------------------------------*
*
*  File:    udpacq.hpp
*  Date:    4/22/2016
*  Author:  Eric Raaen
*
*  Class for handling UDP device sessions
*
*---------------------------------------------------------------------------*/
#ifndef UDPACQ_HPP
#define UDPACQ_HPP
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#include <QObject>
#include <QString>
#include <QByteArray>
#include <reslock.hpp>

namespace DeviceLib
{
//*---------------------------------------------------------------------------
//
// class: UdpAcqDevice 
//
// A UDP socket with a mutex that implements a trival command response
// QUdpSocket didn't work from multiple sockets, so here is a POSIX one
//
//----------------------------------------------------------------------------
class UdpAcqDevice : QObject
{
   Q_OBJECT
   typedef QObject inherited;
public:
   UdpAcqDevice(QString addr);  // ip:port
   // why tf was it all pissy about this?
   UdpAcqDevice(const UdpAcqDevice &cp);             // not implemented
   explicit UdpAcqDevice(QString ip, unsigned int port);  // roll your own
   virtual ~UdpAcqDevice(); 

   int timeout() const;    
   void setTimeout(int ms);

   QString address() const;   // host:port
   void setAddress(QString addr);
   void setAddress(QString host, unsigned int port);
   unsigned int port() const; 
   void setPort(unsigned int p);

   int send(QByteArray cmd);
   QByteArray recv(unsigned int maxSize);

   quint16 getPort();
   QByteArray getIP();
   // jared put these in here for determining if it's got the right port
   // I've commented them out in the cpp file bc it's bad security hygiene to write these sort of functions

   PrivateResource& access();

   // Command / response
   QByteArray sendCommand(QString cmd);
   QByteArray sendCommand(QByteArray cmd);
   QByteArray sendCommand(const char *cmd);  // C string


private:
   void initSocket();
   ssize_t sendTo(const void *bfr, size_t len);
   ssize_t recvFrom(void *bfr, size_t len);

private:
   QByteArray ip_;
   quint16 port_;
   int tmo_; // ms.
   struct addrinfo *ai_;
   int fd_;
   PrivateResource access_;

private:
   UdpAcqDevice& operator=(const UdpAcqDevice &rhs); // not implemented
}; 
//*---------------------------------------------------------------------------
// UdpAcqDevice inline functions
//----------------------------------------------------------------------------
inline int UdpAcqDevice::timeout() const { return tmo_; } 
inline void UdpAcqDevice::setTimeout(int ms) { tmo_ = ms; }
inline unsigned int UdpAcqDevice::port() const { return port_; }
inline void UdpAcqDevice::setPort(unsigned int p) { port_ = p; }
inline QString UdpAcqDevice::address() const { return ip_ + ":" + QString::number(port_); }
inline void UdpAcqDevice::setAddress(QString host, unsigned int port) 
{ ip_ = host.toLatin1(); port_ = port; }
inline int UdpAcqDevice::send(QByteArray cmd) {  return sendTo(cmd, cmd.length()); }
inline PrivateResource& UdpAcqDevice::access() { return access_; } 
inline QByteArray UdpAcqDevice::sendCommand(const char *cmd) {  return sendCommand(QByteArray(cmd)); }

} // namespace
#endif

