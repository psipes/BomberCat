#include "stdafx.h"
#include "DxSoundNone.h"


DxSoundNone::DxSoundNone ()
{
}

DxSoundNone::~DxSoundNone ()
{
}

bool DxSoundNone::init ( HWND hwnd )
{
   hwnd;
   return true;
}

void DxSoundNone::update ()
{
}

void DxSoundNone::shutdown ()
{
}

void DxSoundNone::releaseSound ( DxSoundIdentifier*& identifier )
{
}

// continuously loop through a sound
void DxSoundNone::loop ( DxSoundIdentifier* identifier, bool enable )
{
}

// load a sound from a file
bool DxSoundNone::load ( const tstring& soundFileName, DxSoundIdentifier*& identifier )
{
   identifier = new DxNoneSoundIdentifier;

   return true;
}

// start sound from saved pause time.  this time will be zero if pause was not called.
bool DxSoundNone::play ( DxSoundIdentifier* identifier )
{
   return true;
}

// stop a sound that is playing
bool DxSoundNone::stop ( DxSoundIdentifier* identifier )
{
   return true;
}

// has a sound stopped playing
bool DxSoundNone::isPlaying ( DxSoundIdentifier* identifier )
{
   return true;
}

// pause a sound that is playing (calling play should play from paused point)
bool DxSoundNone::pause ( DxSoundIdentifier* identifier )
{
   return true;
}

void DxSoundNone::setVolume ( DxSoundIdentifier* identifier, int volume )
{
}

void DxSoundNone::setSpeed ( DxSoundIdentifier* identifier, float speed )
{
}