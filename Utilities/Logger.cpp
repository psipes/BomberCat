#include <tchar.h>
#include "Utilities\TTypes.h"
#include "Utilities/Logger.h"



//=======================================================================
void log ( LPCTSTR format, ... )
{
    TCHAR cstrMessage[UCHAR_MAX] = {0};

    va_list listArgs;
    va_start( listArgs, format );
    VTSPRINTF_S( cstrMessage, UCHAR_MAX, format, listArgs );
    va_end( listArgs );
 
    OutputDebugString( cstrMessage );
}

//=======================================================================
void logln ( LPCTSTR format, ... )
{
    TCHAR cstrMessage[UCHAR_MAX] = {0};

    va_list listArgs;
    va_start( listArgs, format );
    VTSPRINTF_S( cstrMessage, UCHAR_MAX, format, listArgs );
    va_end( listArgs );
 
    OutputDebugString( (cstrMessage + tstring("\n")).c_str() );
}