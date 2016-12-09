#include "Utilities/PathUtilities.h"

HINSTANCE           PathUtilities::ourDllHandle = NULL;
PtrPathAddBackslash PathUtilities::_pathAddBackslash = NULL;
PtrPathAppend       PathUtilities::_pathAppend = NULL;
PtrPathCanonicalize PathUtilities::_pathCanonicalize = NULL;
PtrPathIsDirectory  PathUtilities::_pathIsDirectory = NULL;
PtrPathIsRelative   PathUtilities::_pathIsRelative = NULL;
PtrPathFileExists   PathUtilities::_pathFileExists = NULL;
PtrPathStripPath    PathUtilities::_pathStripPath = NULL;


//=======================================================================
bool PathUtilities::init ( )
{
   ourDllHandle = LoadLibrary( "C:\\Windows\\System32\\Shlwapi.dll" );

   _pathAddBackslash =  (PtrPathAddBackslash)GetProcAddress( ourDllHandle, "PathAddBackslashA" ); 
   _pathAppend =        (PtrPathAppend)GetProcAddress( ourDllHandle, "PathAppendA" );
   _pathCanonicalize =  (PtrPathCanonicalize)GetProcAddress( ourDllHandle, "PathCanonicalizeA" ); 
   _pathIsDirectory =   (PtrPathIsDirectory)GetProcAddress( ourDllHandle, "PathIsDirectoryA" );
   _pathIsRelative =    (PtrPathIsRelative)GetProcAddress( ourDllHandle, "PathIsRelativeA" );
   _pathFileExists =    (PtrPathFileExists)GetProcAddress( ourDllHandle, "PathFileExistsA" );
   _pathStripPath =     (PtrPathStripPath)GetProcAddress( ourDllHandle, "PathStripPathA" );
   
   return _pathAddBackslash != NULL && 
          _pathAppend != NULL &&
          _pathCanonicalize != NULL &&
          _pathIsDirectory != NULL &&
          _pathIsRelative != NULL &&
          _pathFileExists != NULL &&
          _pathStripPath != NULL;
}

//=======================================================================
void PathUtilities::shutdown ( )
{
   if ( ourDllHandle != NULL )
   {
      FreeLibrary( ourDllHandle );
      ourDllHandle = NULL;
      _pathAddBackslash = NULL;
      _pathAppend = NULL;
      _pathCanonicalize = NULL;
      _pathIsDirectory = NULL;
      _pathFileExists = NULL;
      _pathStripPath = NULL;

   }
}

//=======================================================================
bool PathUtilities::pathAddBackslash( tstring& path )
{
   if ( _pathAddBackslash == NULL || ( path.length() ) >= MAX_PATH )
   {
      return false;
   }
   TCHAR newPath[MAX_PATH] = {0};

   memcpy( newPath, path.c_str(), path.length() ); // probably unsafe

   bool result = ((*_pathAddBackslash)( newPath ) != NULL);

   if ( result )
   {
      path = newPath;
   }

   return result;
}

//=======================================================================
bool PathUtilities::pathAppend ( tstring& path, const tstring& more )
{
   if ( _pathAppend == NULL || ( path.length() + more.length() ) >= MAX_PATH )
   {
      return false;
   }
   TCHAR newPath[MAX_PATH] = {0};

   memcpy( newPath, path.c_str(), path.length() ); // probably unsafe

   bool result = ((*_pathAppend)( newPath, more.c_str() ) == TRUE);

   if ( result )
   {
      path = newPath;
   }

   return result;
}

//=======================================================================
bool PathUtilities::pathCanonicalize ( tstring& dstPath, const tstring& srcPath )
{
   if ( _pathCanonicalize == NULL || (dstPath.length() + srcPath.length()) >= MAX_PATH )
   {
      return false;
   }
   TCHAR newPath[MAX_PATH] = {0};

   memcpy( newPath, dstPath.c_str(), dstPath.length() ); // probably unsafe

   bool result = ((*_pathCanonicalize)( newPath, srcPath.c_str() ) == TRUE);

   if ( result )
   {
      dstPath = newPath;
   }

   return result;
}

//=======================================================================
bool PathUtilities::pathIsDirectory ( const tstring& path )
{
   if ( _pathIsDirectory == NULL )
   {
      return false;
   }
   BOOL result = (*_pathIsDirectory)( path.c_str() );
   return !!result;
}

//=======================================================================
bool PathUtilities::pathIsRelative ( const tstring& path )
{
   if ( _pathIsRelative == NULL )
   {
      return false;
   }
   BOOL result = (*_pathIsRelative)( path.c_str() );
   return !!result;
}


//=======================================================================
bool PathUtilities::pathFileExists ( const tstring& path )
{
   if ( _pathFileExists == NULL )
   {
      return false;
   }

   BOOL result = (*_pathFileExists)( path.c_str() );
   return !!result;
}

//=======================================================================
bool PathUtilities::pathStripPath ( tstring& path )
{
   if ( _pathCanonicalize == NULL )
   {
      return false;
   }
   TCHAR newPath[MAX_PATH] = {0};

   memcpy( newPath, path.c_str(), path.length() ); // probably unsafe

   (*_pathStripPath)( newPath );

   path = newPath;
   return true;
}