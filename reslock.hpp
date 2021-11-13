/*---------------------------------------------------------------------------*
*
*  File:    reslock.hpp
*  Date:    8/29/2002
*  Author:  Eric Raaen
*
*  Classes for dealing with mutexes using the "resource acquisition is 
*  initialization" idiom.  Patterned after IBM IOC classes to make porting
*  VAC code easier.
*
*  $Log: reslock.hpp,v $
*  Revision 1.2  2007/06/07 21:18:34  lab
*  I checked QMutex, the recursive lock seems to be working now.  Since
*  the pthread workaround was causing build warnings, and some of Heather's
*  problems seemed to mutex related, I put PrivateResource (back) to the
*  QMutex implementation for GCC > 4, and tossed the pthread warnings
*  workaround (since it was GCC > 4 anyway).  Unfortunatly, a header
*  change in vacport means you should REBUILD THE WORLD.
*
*  Revision 1.1.1.1  2005/03/24 23:16:52  er2
*  Initial commit of code to CVS repository. 03/24/2005 --- KDP
*
*
*---------------------------------------------------------------------------*/
#ifndef RESLOCK_HPP
#define RESLOCK_HPP

#include <QMutex>

typedef QMutex PrivateResource;

//*---------------------------------------------------------------------------
//
// Class: ResourceLock 
//
// Uses the "resource acquisition is initialization" idiom, i.e. locks mutex
// in constructor, unlocks it in destructor
// 
//----------------------------------------------------------------------------
class ResourceLock 
{
public:
   ResourceLock (PrivateResource& sem);
   ResourceLock (PrivateResource& sem, long to);
   ~ResourceLock ();

private:
   PrivateResource &Sem;

private:
   ResourceLock (const ResourceLock& cp);             // not implemented
   ResourceLock& operator= (const ResourceLock& rhs); // not implemented
};


#endif



