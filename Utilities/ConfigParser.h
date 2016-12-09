
#pragma once

#include <iostream>

#include "Utilities/TTypes.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/TStringRoutines.h"

//----------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 


//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 


class ConfigParser : private NonCopyable
{
public:
   ConfigParser ( ) { }
   ~ConfigParser ( ) { }
 

   bool getNextLine ( tifstream& strm, tstring& nextLine )
   {      
      tstring  line;
      nextLine = "";

      while ( std::getline( strm, line ) )
      {
         line = Util::trimBoth( line );

         if ( !line.empty() && line[0] != _T('#') )
         {
            nextLine = line;
            return true;
         }
      }
   
      return false;
   }

   // This should go into something OTHER than here so it can be used for other purposes
   static bool parsePairString ( const tstring& line, 
                                 tstring& first, tstring& second, 
                                 const TCHAR* sepChrs = _T(" \t\n\r\b\v" ) )
   {
      tstring trimed( line );

      size_t index = trimed.find_first_of( sepChrs );
      if ( trimed.length() == 0 || index == tstring::npos )
         return false;

      //todo improve error handling.
      first  = trimed.substr( 0, index );
      second = trimed.substr( trimed.find_last_of(sepChrs) + 1 );
      return true;
   } 


private:
   ConfigParser ( const ConfigParser& other );
   ConfigParser& operator= ( const ConfigParser& rhs );
};
