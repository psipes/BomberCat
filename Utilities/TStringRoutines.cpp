#include "Utilities/TTypes.h"
#include "Utilities/TStringRoutines.h"

//=======================================================================
tstring Util::reverse ( const tstring& str )
{
   tstring rStr(str);
   for ( unsigned index = 0, endIndex = (str.length() - 1); index < endIndex; index++, endIndex = (str.length() - index) - 1 )
   {
      rStr[index] ^= rStr[endIndex];
      rStr[endIndex] ^= rStr[index];
      rStr[index] ^= rStr[endIndex];
   }
   return rStr;
}

//=======================================================================
tstring Util::trimFront ( const tstring& str, const tstring& toTrim )
{
   int index = str.find_first_not_of( toTrim );
   if ( index == tstring::npos )
   {
      return tstring(); //return empty
   }
   tstring trimmed = str.substr( index );
   return trimmed;
}

//=======================================================================
tstring Util::trimBack ( const tstring& str, const tstring& toTrim )
{
   int index = str.find_last_not_of( toTrim );
   if ( index == tstring::npos )
   {
      return tstring(); //return empty
   }
   tstring trimmed = str.substr( 0, index + 1 );
   return trimmed;
}

//=======================================================================
tstring Util::trimBoth ( const tstring& str, const tstring& toTrim )
{
   return trimBack( trimFront( str, toTrim ), toTrim );
}

//=======================================================================
tstring Util::trimPath ( const tstring& str, const tstring& toTrim )
{
   int index = str.find_last_of( toTrim );
   if ( index == tstring::npos || index == 0 )
   {
      return str;
   }
   return str.substr( index + 1 );
}


//=======================================================================
bool Util::tokenizeStringToPair ( const tstring& line, 
                                  tstring& first, tstring& second, 
                                  const tstring& sepChrs )
{
   tstring parsedLine( line );

   int firstIndex = parsedLine.find_first_of( sepChrs );
   int secondIndex = parsedLine.find_last_of( sepChrs );
   if ( parsedLine.length() == 0 || firstIndex == tstring::npos || secondIndex == tstring::npos )
   {
      return false;
   }

   first  = parsedLine.substr( 0, firstIndex );
   second = parsedLine.substr( secondIndex + 1 );

   return true;
} 