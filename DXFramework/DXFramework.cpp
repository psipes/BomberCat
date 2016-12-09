// DxFramework.cpp : The Application Class.
//

#include "stdafx.h"
#include <assert.h>
#include "Utilities/Logger.h"
#include "Utilities/PathUtilities.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxAssetManager.h"
#include "DxFramework/DxFramework.h"

IDXDEVICE DxFramework::ourDevice = NULL;
IDXINTERFACE DxFramework::ourInterface = NULL;
IDXSPRITE DxFramework::ourSpriteInterface = NULL;
IDXFONT DxFramework::ourFontInterface = NULL;
D3DPRESENT_PARAMETERS* DxFramework::ourPresentParameters = NULL;
//=======================================================================
DxFramework::DxFramework ( )
:myBackBuffer(NULL)
{
}

//=======================================================================
DxFramework::~DxFramework ( )
{
}

//=======================================================================
//protected
bool DxFramework::winPostCreateWindow ( )
{
   HRESULT result;

   LONG_PTR style = GetWindowLongPtr( hWnd(), GWL_STYLE );
   SetWindowLongPtr( hWnd(), GWL_STYLE, style & ~(removeWindowStyle()) );

   if ( !PathUtilities::init() )
   {
      return false;
   }
   
   ourInterface = Direct3DCreate9(D3D_SDK_VERSION);

   if ( dxInterface() == NULL )
   {
      log( _T("Error when initailizing DirectX.") );
      return false;
   }

   ZeroMemory(&myD3Dpp, sizeof(myD3Dpp));
   myD3Dpp.Windowed = !fullscreen();
   myD3Dpp.SwapEffect = swapEffect();
   myD3Dpp.BackBufferFormat = format();
   myD3Dpp.BackBufferCount = backBufferCount();
   myD3Dpp.BackBufferWidth = winScreenWidth();
   myD3Dpp.BackBufferHeight = winScreenHeight();
   myD3Dpp.hDeviceWindow = hWnd();
   myD3Dpp.PresentationInterval = presentationInterval();
   myD3Dpp.EnableAutoDepthStencil = enableAutoDepthStencil();
   myD3Dpp.AutoDepthStencilFormat = autoDepthStencilFormat();

   myD3Dpp = ( presentParameters() ? *presentParameters() : myD3Dpp );

   if ( ourPresentParameters != NULL )
   {
      log( _T("Warning: Class Present Parameters Overrided!") );   
   }
   ourPresentParameters = &myD3Dpp;

   result = dxInterface()->CreateDevice( D3DADAPTER_DEFAULT, 
                                         D3DDEVTYPE_HAL, 
                                         hWnd(),
                                         D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                         &myD3Dpp, 
                                         &ourDevice );
   if ( FAILED(result) || ourDevice == NULL )
   {
      log( _T("Could not create DirectX device.") );
      return false;
   }

   result = device()->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &myBackBuffer );
   if ( FAILED(result) || myBackBuffer == NULL )
   {
      log( _T("Could not retrieve back buffer.") );
      return false;
   }

   result = D3DXCreateSprite( device(), &ourSpriteInterface );
   if ( FAILED(result) || ourSpriteInterface == NULL )
   {
      log( _T("Unable to create sprite!") );
      return false;
   }

   result = D3DXCreateFont( device(), 18, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &ourFontInterface );
   if( FAILED( result ) )
   {
      log( _T("Unable to create font!") );
      return false;
   };

   return gameInit();
}

//=======================================================================
//protected
bool DxFramework::winAppIdle ( )
{
   assert( !!ourDevice && "Trying to use an invalid DirectX device!" );
   gameRun();
   return true;
}

//=======================================================================
//protected
bool DxFramework::winAppExit ( )
{
   gameExit();

   DxAssetManager::getInstance().shutdown();

   if( fontInterface() )
   {
      IfRelease( &ourFontInterface );
   }
   if ( spriteInterface() )
   {
      IfRelease( &ourSpriteInterface );
   }
   if ( device() )
   {
      IfRelease( &ourDevice );
   }
   if ( dxInterface() )
   {
      ourInterface->Release();
      ourInterface = NULL;
   }

   PathUtilities::shutdown();

   return true;
}