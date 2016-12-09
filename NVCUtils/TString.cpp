
#include "tstring.h"


#ifdef UNICODE
   extern tistream&     tcin  = std::wcin;
   extern tostream&     tcout = std::wcout;
#else
   extern tistream&     tcin  = std::cin;
   extern tostream&     tcout = std::cout;
#endif