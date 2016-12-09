//-------------------------------------------------------------------------
// Author: Issac Irlas
//-------------------------------------------------------------------------
#pragma once

#if !defined(_INSTANCEMONITOR_H_)
#define _INSTANCEMONITOR_H_
#include <windows.h>
#include "Utilities/NonCopyable.h"

// Used to check for other instances of an application.
class InstanceMonitor : public NonCopyable
{
public:
    //======================================================================
    // Default Constuctor
    InstanceMonitor ( );

    //======================================================================
    // Constuctor that is created with a mutex.
    InstanceMonitor ( LPCTSTR mutexName );

    //======================================================================
    // Default Destuctor
    ~InstanceMonitor ( );

    //======================================================================
    // Creates a mutex if one has not been created.
    bool create ( LPCTSTR mutexName );

    //======================================================================
    // True if the mutex created did not return ERROR_ALREADY_EXISTS.
    bool isThisTheOnlyInstance ( );
private:
    HANDLE myMutex;
    DWORD myLastError;
};

#endif //_INSTANCEMONITOR_H_