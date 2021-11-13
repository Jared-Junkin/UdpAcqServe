/*---------------------------------------------------------------------------*
*
*  File:    reslock.cpp
*  Date:    8/29/2002
*  Author:  Eric Raaen
*
*  Implementation of resource lock classes
*
*  $Log: reslock.cpp,v $
*  Revision 1.7  2009/08/20 17:27:35  er2
*  Ported to Qt4 - still needs testing
*
*  Revision 1.6  2009/03/05 16:07:23  er2
*  Changed usleep to ms_sleep (port to MinGW)
*
*  Revision 1.5  2007/06/07 21:18:34  lab
*  I checked QMutex, the recursive lock seems to be working now.  Since
*  the pthread workaround was causing build warnings, and some of Heather's
*  problems seemed to mutex related, I put PrivateResource (back) to the
*  QMutex implementation for GCC > 4, and tossed the pthread warnings
*  workaround (since it was GCC > 4 anyway).  Unfortunatly, a header
*  change in vacport means you should REBUILD THE WORLD.
*
*  Revision 1.4  2007/02/01 15:26:55  er2
*  Made itoa extern "C" and tried again to get rid of reslock warnings
*
*  Revision 1.3  2007/01/16 16:14:17  lab
*  Made mutex initialization "fix" dependent on GCC_VERSION
*
*  Revision 1.2  2006/12/15 02:55:21  er2
*  Got rid of warnings, but I think it is a compilier problem and should
*  "unfix" it later i.e. NEEDS_WORK
*
*  Revision 1.1.1.1  2005/03/24 23:16:52  er2
*  Initial commit of code to CVS repository. 03/24/2005 --- KDP
*
*
*---------------------------------------------------------------------------*/
#include "reslock.hpp"
#include "except.hpp"

//*---------------------------------------------------------------------------
//
// ResourceLock::ResourceLock(sem)
//
// Constructor - locks the mutex
//
//----------------------------------------------------------------------------
ResourceLock :: ResourceLock(PrivateResource& sem) : 
   Sem(sem)
{
   Sem.lock();
}
//*---------------------------------------------------------------------------
//
// ResourceLock::ResourceLock(sem, to)
//    PrivateResource& sem 
//    long to - time out for lock
//
// Constructor - locks the mutex, with time out
//
//----------------------------------------------------------------------------
ResourceLock :: ResourceLock(PrivateResource& sem, long to) :
   Sem(sem) 
{
   if (!sem.tryLock(to))
      throw StdExceptionBase("Mutex lock timeout");
}

//*---------------------------------------------------------------------------
//
// ResourceLock::~ResourceLock()
//
// Destructor - unlocks the mutex
//
//----------------------------------------------------------------------------
ResourceLock :: ~ResourceLock()
{
   Sem.unlock();
}

