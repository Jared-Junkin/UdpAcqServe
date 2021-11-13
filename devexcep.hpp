/*---------------------------------------------------------------------------*
*
*  File:    devexcep.hpp
*  Date:    9/9/1998
*  Author:  Eric Raaen
*
*  Ported from VAC 09/2002
*  Ported to Qt5 09/2015
*
*  Exception classes for Device library
*
*  $Log: devexcep.hpp,v $
*  Revision 1.1.1.1  2005/03/24 23:16:52  er2
*  Initial commit of code to CVS repository. 03/24/2005 --- KDP
*
*  Revision 1.1  1999/05/06 19:10:15  Raaen
*  Initial revision
*
*
*---------------------------------------------------------------------------*/
#ifndef DEVEXECP_HPP
#define DEVEXECP_HPP
#include <except.hpp>

namespace DeviceLib
{


namespace DeviceExceptionPrivate
{
   extern const char dre[];
   extern const char dce[];
   extern const char gpce[];
   extern const char dnr[];
   extern const char dna[];
   extern const char spre[];
   extern const char spwe[];
}

typedef DerivedException<DeviceExceptionPrivate::dre> DeviceRequestError;
typedef DerivedException<DeviceExceptionPrivate::dre> DeviceCommunicationError;
typedef DerivedException<DeviceExceptionPrivate::dnr> DetectorNotReady;
typedef DerivedException<DeviceExceptionPrivate::dna> DeviceNotAvailable;
typedef DerivedException<DeviceExceptionPrivate::gpce> GPIBCommunicationError;
typedef DeviceRequestError GPIBRequestError;
typedef DerivedException<DeviceExceptionPrivate::spre> SetPointReadError;
typedef DerivedException<DeviceExceptionPrivate::spwe> SetPointWriteError;

}
#endif

