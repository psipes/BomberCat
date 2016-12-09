#pragma once

#if !defined ( __DXDirectSoundManager_H__ )
#define __DXDirectSoundManager_H__

#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <dsound.h>
#include "DxWaveFile.h"

class DxDirectSoundManager
{
public:
   DxDirectSoundManager ();
   ~DxDirectSoundManager ();

   bool init ( HWND hwnd, DWORD coopLevel );

   bool create ();

private:
   LPDIRECTSOUND8 mySoundInterface;
};

#endif