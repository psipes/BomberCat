//-------------------------------------------------------------------------
// Author: Issac Irlas
//-------------------------------------------------------------------------
#pragma once

#if !defined(_DXSURFACE_H_)
#define _DXSURFACE_H_

#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxImage.h"


//===================================================================================================
class DxSurface : public DxImage
{
public:
   DxSurface ( void );
   DxSurface ( const DxSurface& other );
   ~DxSurface ( void );
   DxSurface& operator= ( const DxSurface& other );
   operator IDXSURFACE ( void ) { return mySurface; }
   operator bool ( void ) { return mySurface != NULL; }

   UINT width ( void ) { return mySurfaceDescription.Width; }
   UINT height ( void ) { return mySurfaceDescription.Height; }

   bool createEmpty ( IDXDEVICE device, unsigned width, unsigned height );

   bool createFromFile ( IDXDEVICE device, const tstring& filepath, const RECT* srcRect = NULL );

   void destroy ( void );

   HRESULT setToBackBuffer ( IDXDEVICE device, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE type );
   HRESULT draw ( IDXDEVICE device, LPRECT sRect, IDXSURFACE target, LPRECT tRect, D3DTEXTUREFILTERTYPE filter = D3DTEXF_NONE );

   // Set surface's color
   bool setColor( IDXDEVICE device, D3DCOLOR color );

private:
   IDXSURFACE     mySurface;
   D3DSURFACE_DESC mySurfaceDescription;
};

#endif //_DXSURFACE_H_