#pragma once

#if !defined(_TSTRINGROUTINES_H_)
#define _TSTRINGROUTINES_H_
#define SIZEOFARRAY(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))
#include <tchar.h>
#include "Utilities\TTypes.h"

namespace Util
{

static const tstring whitespace = " \t\n\v\f\r\b";

//=======================================================================
tstring reverse ( const tstring& str );

//=======================================================================
tstring trimFront ( const tstring& str, const tstring& toTrim = whitespace );

//=======================================================================
tstring trimBack ( const tstring& str, const tstring& toTrim = whitespace);

//=======================================================================
tstring trimBoth ( const tstring& str, const tstring& toTrim = whitespace );

//=======================================================================
tstring trimPath ( const tstring& str, const tstring& toTrim = _T("\\/") );

//=======================================================================
bool tokenizeStringToPair ( const tstring& line, 
                            tstring& first, tstring& second, 
                            const tstring& sepChrs = whitespace );
}

#endif //_TSTRINGROUTINES_H_