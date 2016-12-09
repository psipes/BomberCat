
#pragma once

#define WIN32_EXTRA_LEAN

#if !defined( DIRECTINPUT_VERSION )
#define DIRECTINPUT_VERSION 0x0800
#endif 

#include <assert.h>
#include <string>

#include <windows.h>
#include <tchar.h>

#include "Utilities/TTypes.h"



//----------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 


//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 

/// <summary> 
/// class Utilities is a container for *static* utility functions which are generally 
/// useful in any project.  If you want to make *MANY* static Utilities, separate, fine.
///
/// By design, these routines are intended to be static, and if possible, inline.
/// 
/// Non-static methods must be approved before being added to Utils!
/// </summary>

//TODO: SAS:  Ok.  Now get rid of MISCELLANEOUS... or make them separate, but NAMED RIGHT.
//            Me I'd favor the later, but AFTER THE PROJECT IS DONE!!!!!!!!!!

class Utilities
{
public:
   
   static const TCHAR pathQuoteChar = _T('"');     // Approach works w/#if#endif for various OS's... there's a better way though....
   static const TCHAR spaceChar     = _T(' ');
   static const TCHAR pathSepChar   = _T('\\');

   //----------------------------------------------------------------------------------------
   // Get Current working directory in tstring format.
   static inline tstring getCurrentDirectory ( void ) {
      TCHAR buffer[ MAX_PATH + 1 ]; // Not really.... you should note that in Unicode, 32767 is possible!!!!!   
      if ( !GetCurrentDirectory( MAX_PATH+1, buffer ) )
         assert( false );  // Deal with this...

      // Canonize this path? (ensure proper '\' or '/' for OS, quoting, char-quoting (unix), etc.
      
      tstring  dir( buffer );
      assert( dir.length() > 0 );

      return dir;
   }

   
   //----------------------------------------------------------------------------------------
   // Properly (when implemented fully) appends a path to an existing path, returning a proper canonical path
   //
   static inline tstring appendPath ( const tstring& path, const tstring& newPath ) 
   {
      if (path.length() == 0)
         return newPath;         // probably should get current folder, make it a complete path

      tstring fullPath( path );
      int   lastIx   = fullPath.length();
      TCHAR lastChar   = fullPath[lastIx-1];

      if ( lastChar != pathSepChar )
         fullPath.insert( lastIx++, tstring(1,pathSepChar) );

      //Should check newPath is a path (or should it work for a FILE? Should these be DIFFERENT? )
      //Should check newPath spaces/whitespace & canonize.
      //Should check newPath redundancy ( "c:/foo/boo/doo", "../doo" ) should NOT be "c:/foo/boo/doo/../doo"
      //FIND THE APPROPRIATE API's!!!!
      
      fullPath.insert( lastIx, newPath );

      // And More To Validate ???

      return fullPath;
   }

   
   static inline tstring quotedPath ( const tstring& path )
   {
      if ( path.length() > 0 && path[0] != pathQuoteChar )
         return pathQuoteChar + path + pathQuoteChar;

      return path;
   }


   ///////////<summary>
   ///////////Accessory function to safely release an IUnknown derived object
   ///////////and set the reference variable to NULL
   ///////////</summary>
   ////////template <typename T>
   ////////static inline void ifRelease( T*& p ) { if (p) p->Release(); p = NULL; }


private:
   Utilities ( const Utilities& other );
   Utilities& operator= ( const Utilities& rhs );
};
