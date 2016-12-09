#include "stdafx.h"
#include "DxSoundIdentifier.h"

DxSoundIdentifier::DxSoundIdentifier ()
{
}

DxSoundIdentifier::~DxSoundIdentifier ()
{
}

void DxSoundIdentifier::loop ( bool enabled )
{
}

bool DxSoundIdentifier::isPlaying ()
{
   return true;
}

void DxSoundIdentifier::setVolume ( int volume )
{
}

//=================================================================================================
//=================================================================================================


DxFMODSoundIdentifier::DxFMODSoundIdentifier ()
{
   myFsound = NULL;
   myFchannel = 0;
   myIsPaused = false;
   myIsPlaying = false;
   myLoopEnabled = false;
}

DxFMODSoundIdentifier::~DxFMODSoundIdentifier ()
{
}

void DxFMODSoundIdentifier::setVolume ( int volume )
{
   myVolume = (float)volume / 100;
}

void DxFMODSoundIdentifier::loop ( bool enabled )
{
   myLoopEnabled = enabled;
}