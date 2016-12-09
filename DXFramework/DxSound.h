//-------------------------------------------------------------------------------------------
// Sound interface class.
//
// Declare a DxSoundIdentifier variable for every sound you will have in your program.
//
// Get the interface by calling getInterface() first to retrieve the desired sound interface.
// Then call init().
//    example: getInterface()->init( hWnd() );
//
// To use sounds, create a DxSoundIdentifier variable
//    and feed it into these functions.
//
// You must first load a sound file into the DxSoundIdentifier
//    using DxSound::load().
//    example: getInterface()->load( filepath, mySoundIdentifier );
//
// When closing a program, call getInterface()->releaseSound() for each sound in your program.
// Then call getInterface()->shutdown.
// Last, call releaseSound();
//-------------------------------------------------------------------------------------------

#pragma once


#if !defined ( DxSOUND_H__ )
#define DxSOUND_H__

#include <windows.h>
#include <mmsystem.h>
#include "Utilities/NonCopyable.h"
#include "Utilities/TTypes.h"
#include "Utilities/Logger.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxSoundIdentifier.h"

#pragma comment( lib, "Winmm.lib" )


class DxSound : public NonCopyable
{
public:
   // InterfaceTypes is multi-valued
   //		foo = DxSound::getInterface( fmod | winNative | none );
   static DxSound* getInterface (  unsigned int type = fmod | directSound | winNative | none, HWND hWnd = NULL  );
   static void releaseInterface ();

protected:
   DxSound () {}
   virtual ~DxSound () {}

   static DxSound* theSoundInterface;

public:

   enum InterfaceTypes { fmod = 1, directSound = 2, winNative = 4, none = 8 };


   virtual bool init ( HWND hwnd ) = 0;
   virtual void update () = 0;
   virtual void shutdown () = 0;

   // release a sound (required if interface is set to fmod)
   virtual void releaseSound ( DxSoundIdentifier*& identifier ) = 0;

   virtual bool play ( DxSoundIdentifier* identifier ) = 0;

   // continuously loop through a sound
   virtual void loop (  DxSoundIdentifier* identifier, bool enable = true ) = 0;

   // load a sound from a file
   virtual bool load ( const tstring& soundFileName, DxSoundIdentifier*& identifier ) = 0;

   // stop a sound that is playing
   virtual bool stop ( DxSoundIdentifier* identifier ) = 0;

   // has a sound stopped playing
   virtual bool isPlaying ( DxSoundIdentifier* identifier ) = 0;

   // pause a sound that is playing (calling play should play from paused point)
   virtual bool pause ( DxSoundIdentifier* identifier ) = 0;

   // if volume is 0 or lower, then sound is muted.  if volume is 100 or greater, then sound is set to max.
   virtual void setVolume ( DxSoundIdentifier* identifier, int volume = 100 ) = 0;

   // insert a number between 0.01 and 100.0.  default speed is 1.0.  double is 2.0
   virtual void setSpeed ( DxSoundIdentifier* identifier, float speed = 1.0f ) = 0;

};

#endif