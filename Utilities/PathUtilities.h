#pragma once

#if !defined(_PATHUTILITIES_H_)
#define _PATHUTILITIES_H_

#include <windows.h>
#include <vector>
#include <Shlwapi.h>
#include "Utilities/TTypes.h"
#include "Utilities/NonCopyable.h"

typedef LPTSTR (STDAPICALLTYPE *PtrPathAddBackslash)( LPTSTR path );
typedef BOOL (STDAPICALLTYPE *PtrPathAppend)( LPTSTR path, LPCTSTR more );
typedef BOOL (STDAPICALLTYPE *PtrPathCanonicalize)( LPTSTR dstPath, LPCTSTR srcPath );
typedef BOOL (STDAPICALLTYPE *PtrPathIsDirectory)( LPCTSTR path );
typedef BOOL (STDAPICALLTYPE *PtrPathIsRelative)( LPCTSTR path );
typedef BOOL (STDAPICALLTYPE *PtrPathFileExists)( LPCTSTR path );
typedef void (STDAPICALLTYPE *PtrPathStripPath) ( LPTSTR path );
class PathUtilities : NonCopyable
{
public:
   //=======================================================================
   static bool init ( );

   //=======================================================================
   static void shutdown ( );

   //=======================================================================
   static bool pathAddBackslash( tstring& path );

   //=======================================================================
   static bool pathAppend ( tstring& path, const tstring& more );

   //=======================================================================
   static bool pathCanonicalize ( tstring& dstPath, const tstring& srcPath );

   //=======================================================================
   static bool pathIsDirectory ( const tstring& path );

   //=======================================================================
   static bool pathIsRelative ( const tstring& path );

   //=======================================================================
   static bool pathFileExists ( const tstring& path );

   //=======================================================================
   static bool pathStripPath ( tstring& path );

private:
   PathUtilities ( void );
   ~PathUtilities ( void );
   static HINSTANCE           ourDllHandle;
   static PtrPathAddBackslash _pathAddBackslash;
   static PtrPathAppend       _pathAppend;
   static PtrPathCanonicalize _pathCanonicalize;
   static PtrPathIsDirectory  _pathIsDirectory;
   static PtrPathIsRelative   _pathIsRelative;
   static PtrPathFileExists   _pathFileExists;
   static PtrPathStripPath    _pathStripPath;
   //static FARPROC _pathCanonicalize;
};

#endif //_PATHUTILITIES_H_
