//-------------------------------------------------------------------------
// Class that contains the actual sound.
//
// To use this, declare a DxSoundIdentifier variable 
//    and use it as a parameter in DxSound functions
//-------------------------------------------------------------------------

#pragma once

#if !defined ( __DxSOUNDIDENTIFIER_H__ )
#define __DxSOUNDIDENTIFIER_H__

#include <tchar.h>
#include "Utilities/TTypes.h"
#include "DxFramework/fmod.hpp"


class DxSoundIdentifier
{
protected:
   DxSoundIdentifier ();
   virtual ~DxSoundIdentifier ();

public:   
   virtual tstring name () = NULL;

   virtual void loop ( bool enabled = true ) = NULL;

   virtual bool isPlaying ( ) = NULL;

   virtual bool isLooping () = NULL;

   virtual void setVolume ( int volume ) = NULL;


private:
   DxSoundIdentifier ( const DxSoundIdentifier& other );
   DxSoundIdentifier& operator= ( const DxSoundIdentifier& other );
};



//----------------------------------------------------------------------------------------


class DxNoneSoundIdentifier : public DxSoundIdentifier
{
public:
   DxNoneSoundIdentifier () {}
   ~DxNoneSoundIdentifier () {}

public:   
   tstring name () { return _T("NULL Sound Identifier"); }

   void loop ( bool enabled = true ) { return; }

   bool isPlaying ( ) { return false; }

   bool isLooping () { return false; }

   void setVolume ( int volume ) { return; }


};



//----------------------------------------------------------------------------------------
class DxFMODSoundIdentifier : public DxSoundIdentifier
{
   friend class DxSoundFMOD;

protected:
   DxFMODSoundIdentifier ();
   ~DxFMODSoundIdentifier ();

public:
   tstring name () {  return myFileName; }

   void loop ( bool enabled = true );

   bool isPlaying ( ) {  return true; }

   bool isLooping () {  return true; }

   // set volume to a number between 0 and 100
   void setVolume ( int volume );

private:

   // the actual sound for FMOD
   FMOD::Sound* myFsound;
   FMOD::Channel* myFchannel;

   // volume setting
   float myVolume;

   bool myIsPlaying;
   bool myIsPaused;
   bool myLoopEnabled;

   // sound file name
   tstring myFileName;
};


#endif