//-------------------------------------------------------------------------------------------
// SEE DxSound.h
//-------------------------------------------------------------------------------------------

#pragma once


#if !defined ( DxSOUNDNONE_H__ )
#define DxSOUNDNONE_H__

#include <windows.h>
#include <mmsystem.h>
#include "Utilities/NonCopyable.h"
#include "Utilities/TTypes.h"
#include "Utilities/Logger.h"
#include "DXFramework/DxSoundIdentifier.h"
#include "DXFramework/DxSound.h"

#pragma comment( lib, "Winmm.lib" )


class DxSoundNone : public DxSound
{

friend class DxSound;

private:
   DxSoundNone ();
   ~DxSoundNone ();

public:

   bool init ( HWND hwnd );
   void update ();
   void shutdown ();

   // release a sound (required if interface is set to fmod)
   void releaseSound ( DxSoundIdentifier*& identifier );

   bool play ( DxSoundIdentifier* identifier );

   // continuously loop through a sound
   void loop (  DxSoundIdentifier* identifier, bool enable = true );

   // load a sound from a file
   bool load ( const tstring& soundFileName, DxSoundIdentifier*& identifier );

   // stop a sound that is playing
   bool stop ( DxSoundIdentifier* identifier );

   // has a sound stopped playing
   bool isPlaying ( DxSoundIdentifier* identifier );

   // pause a sound that is playing (calling play should play from paused point)
   bool pause ( DxSoundIdentifier* identifier );

   // if volume is 0 or lower, then sound is muted.  if volume is 100 or greater, then sound is set to max.
   void setVolume ( DxSoundIdentifier* identifier, int volume = 100 );

   // insert a number between 0.01 and 100.0.  default speed is 1.0.  double is 2.0
   void setSpeed ( DxSoundIdentifier* identifier, float speed = 1.0f );


};

#endif