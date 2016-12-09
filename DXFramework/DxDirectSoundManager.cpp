#include "stdafx.h"
#include "DxDirectSoundManager.h"

DxDirectSoundManager::DxDirectSoundManager ()
{
}

DxDirectSoundManager::~DxDirectSoundManager ()
{
}

bool DxDirectSoundManager::init ( HWND hwnd, DWORD coopLevel )
{
   HRESULT hr;

   hr = DirectSoundCreate8( NULL, &mySoundInterface, NULL );

   if ( FAILED( hr ) )
      return false;

   hr = mySoundInterface->SetCooperativeLevel( hwnd, coopLevel );

   if ( FAILED( hr ) )
      return false;

   return true;
}

bool DxDirectSoundManager::create ()
{
   HRESULT hr;
   DWORD i;
   LPDIRECTSOUNDBUFFER  dsBuffer;
   DWORD                bufferSize;
   DxWaveFile*         waveFile;

   UNREFERENCED_PARAMETER( hr );
   UNREFERENCED_PARAMETER( i );
   UNREFERENCED_PARAMETER( dsBuffer );
   UNREFERENCED_PARAMETER( bufferSize );
   UNREFERENCED_PARAMETER( waveFile );

   if ( !mySoundInterface )
      return false;

   return true;
}