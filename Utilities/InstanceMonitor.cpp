#include <assert.h>
#include "Utilities/InstanceMonitor.h"

//=======================================================================
InstanceMonitor::InstanceMonitor ( )
   :myMutex(NULL), myLastError(0)
{
}


//=======================================================================
InstanceMonitor::InstanceMonitor ( LPCTSTR mutexName )
   :myMutex(NULL), myLastError(0)
{
    create( mutexName );
}

//=======================================================================
InstanceMonitor::~InstanceMonitor ( )
{
   if ( myMutex )
   {
      CloseHandle( myMutex );
      myMutex = NULL;
      myLastError = 0;
   }
}

//=======================================================================
bool InstanceMonitor::create ( LPCTSTR mutexName )
{
    assert( myMutex == NULL );
    myMutex = CreateMutex( NULL, FALSE, mutexName );
    myLastError = GetLastError();
    return !!myMutex;
}

//=======================================================================
bool InstanceMonitor::isThisTheOnlyInstance ( )
{
   return (myLastError != ERROR_ALREADY_EXISTS);
}