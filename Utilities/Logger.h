#pragma once
#if !defined(_LOGGER_H_)
#include <windows.h>
#include "Utilities\TTypes.h"


//=======================================================================
// Logs a formated message to the debug console.
// Usage: log ( _T( formatedMessage ), arg1, arg2, etc. );
void log ( LPCTSTR format, ... );


//=======================================================================
// Logs a formated message to the debug console with an added new line after the log.
// Usage: logln ( _T( formatedMessage ), arg1, arg2, etc. );
void logln ( LPCTSTR format, ... );

#endif //_LOGGER_H_