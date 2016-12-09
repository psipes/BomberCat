//-----------------------------------------------------------
//
// DirectX engine
// version 1.0
// William Harline
// Issac Irlas
//
//-----------------------------------------------------------

#pragma once

#if !defined ( _DXFRAMEWORK_H_ )
#define _DXFRAMEWORK_H_

#include "DxCommon.h"
#include "WinApplFramework/WinApplFramework.h"
#include "Utilities/TTypes.h"
#include "DxTypes.h"

class DxFramework : public WinApplFramework
{
public:
   //=======================================================================
   // Default Constructor
   DxFramework ( );

   //=======================================================================
   // Default Overridable Deconstructor
   virtual ~DxFramework ( );

   //=======================================================================
   // The Interface used to create and control the dxDevice.
   static IDXINTERFACE dxInterface ( )
   {
      return ourInterface;
   }

   //=======================================================================
   // The interface used to create other directX objects.
   static IDXDEVICE device ()
   {
      return ourDevice;
   }

   //=======================================================================
   //
   static IDXSPRITE spriteInterface ()
   {
      return ourSpriteInterface;
   }

   //=======================================================================
   //
   static IDXFONT fontInterface ()
   {
      return ourFontInterface;
   }

   //=======================================================================
   //
   static D3DPRESENT_PARAMETERS presentParametersInterface ()
   {
      if ( ourPresentParameters == NULL)
      {
         D3DPRESENT_PARAMETERS pp;
         ZeroMemory(&pp, sizeof(pp));
         return pp;
      }
      return *ourPresentParameters;
   }

protected:
   //=======================================================================
   // The initializer for any pre-game functionality.
   // Note: Must override to add functionality.
   virtual bool gameInit ( ) = 0;

   //=======================================================================
   // The updater for running and drawing frames for the game.
   // Note: Must override to add functionality.
   virtual void gameRun( ) = 0;

   //=======================================================================
   // The cleanup function for the game.
   // Note: Must override to add functionality.
   virtual void gameExit ( ) = 0;

   //=======================================================================
   // The current backBuffer of window.
   IDXSURFACE backBuffer ( )
   {
      return myBackBuffer;
   }

   //=======================================================================
   // Overrided to serive as the directX initializer and calls gameInit
   // after initialization.
   bool winPostCreateWindow ( void );
   
   //=======================================================================
   // Overrided to wrap directX frame by frame updates and calls gameUpdate.
   bool winAppIdle ( void );

   //=======================================================================
   // Overrided to call gameExit cleaning up after directX.
   bool winAppExit ( void );

   //=======================================================================
   virtual LONG removeWindowStyle ( )
   {
      return NULL;
   }

   //=======================================================================
   // Overrided to 
   virtual D3DPRESENT_PARAMETERS* presentParameters ()
   {
      return NULL;
   }

   //=======================================================================
   // The presentation parameters of directX that determines the 
   // fullscreen/windowed state of the window.
   virtual bool fullscreen ( )
   {
      return FALSE;
   }

   //=======================================================================
   // The presentation parameters of directX that determines the type of
   // swap effect that directX will use.
   virtual D3DSWAPEFFECT swapEffect ( )
   {
      return D3DSWAPEFFECT_DISCARD;
   }

   //=======================================================================
   // The presentation parameters of directX that determines 
   // format of the back buffer.
   virtual D3DFORMAT format ( )
   {
      return D3DFMT_A8R8G8B8;
   }

   //=======================================================================
   // The presentation parameters of directX that determines the number of
   // back buffers.
   virtual UINT backBufferCount ( )      
   {
      return 1;
   }

   //=======================================================================
   // The presentation parameters of directX that determines the update 
   // interval of directX when rendering.
   virtual UINT presentationInterval ( ) 
   { 
      return D3DPRESENT_INTERVAL_DEFAULT; 
   }

   //=======================================================================
   virtual BOOL enableAutoDepthStencil ( ) 
   { 
      return false; 
   }
   
   //=======================================================================
   virtual D3DFORMAT autoDepthStencilFormat ( ) 
   { 
      return D3DFMT_UNKNOWN; 
   }

private:
   static IDXINTERFACE           ourInterface;
   static IDXDEVICE              ourDevice;
   static IDXSPRITE              ourSpriteInterface;
   static IDXFONT                ourFontInterface;
   static D3DPRESENT_PARAMETERS* ourPresentParameters;
   D3DPRESENT_PARAMETERS myD3Dpp;
   IDXSURFACE            myBackBuffer;
};
#endif //_DXFRAMEWORK_H_