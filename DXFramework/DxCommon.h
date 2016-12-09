
#pragma once

#if !defined(_DXCOMMON_H_)
#define _DXCOMMON_H_

#if defined(DIRECTINPUT_VERSION) 
   #if (DIRECTINPUT_VERSION != 0x0800)
      #error Unexpected Condition: DIRECTINPUT_VERSION != 0x0800
   #endif

   #undef DIRECTINPUT_VERSION // warning on undef undefined allowed.
#endif

#if !defined(DIRECTINPUT_VERSION)
   #define DIRECTINPUT_VERSION (0x0800)
#endif

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <tchar.h>
#include <dinput.h>
#include <xinput.h>

#include "Utilities/TStringRoutines.h"
#include "Utilities/Logger.h"
#include "Utilities/NonCopyable.h"
#include "Utilities/TTypes.h"


// libraries
#pragma comment( lib, "winmm.lib" )
#pragma comment( lib, "user32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "dxguid.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )
#pragma comment( lib, "Dinput8.lib" )

//Company wide allow warning disablers...
//  warning C4482: nonstandard extension used: enum 'Kitty::Direction' used in qualified name : DISABLE ALLOW
//    DISABLE THIS WARNING GLOBALLY FOR ALL DxFramework work.  Why? STUPID C++ rule.  That's Why.
#pragma warning( disable : 4482 )



template<typename ITYPE> bool IfRelease ( ITYPE** interfaceInstance )
{
   if ( *interfaceInstance )
   {
      (*interfaceInstance)->Release();
      (*interfaceInstance) = NULL;
      return true;
   }
   return false;
}

#endif //_DXCOMMON_H_
