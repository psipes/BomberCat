#include "stdafx.h"
#include "DxFramework/DxSound.h"
#include "DxFramework/DxSoundNone.h"
#include "DxFramework/DxSoundFMOD.h"
#include <cassert>

DxSound* DxSound::theSoundInterface = NULL;

//---------------------------------------------------------------------------------
// InterfaceTypes is multi-valued
//		foo = DxSound::getInterface( fmod | winNative | none );
DxSound* DxSound::getInterface (  unsigned int type, HWND hWnd  )
{
   // if there already is an interface, return it without creating a new one.
   // NOT THREAD SAFE
   if ( theSoundInterface )
   {
      return theSoundInterface;
   }

   if ( type & InterfaceTypes::fmod )
   {
      theSoundInterface = DxSoundFMOD::create( hWnd );
      if ( theSoundInterface )
         log( _T("Using FMOD sound system.\n") );
   }
   
   if ( !theSoundInterface && ( type & InterfaceTypes::directSound ) )
   {
     // theSoundInterface = DxSoundDirectSound::create( hWnd );
      if ( theSoundInterface )
         log( _T("Using DirectSound sound system.\n") );
   }

   if ( !theSoundInterface )
   {
      theSoundInterface = new DxSoundNone;
      log( _T("Using NULL sound system.\n") );
      assert( theSoundInterface );

   }



   return theSoundInterface;
}

//---------------------------------------------------------------------------------

void DxSound::releaseInterface ()
{
   theSoundInterface->shutdown();
   delete theSoundInterface;
   theSoundInterface = NULL;
}
