

#pragma once

#if !defined(_DXTEXTURE_H_)
#define _DXTEXTURE_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "Utilities/NonCopyable.h"
#include "Utilities/Rect.h"
#include "Utilities/TTypes.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxImage.h"
#include "DxFramework/DxTypes.h"

//===================================================================================================
class DxTexture : public DxImage
{
public:
   DxTexture ( void );
   DxTexture ( const DxTexture& other );
   ~DxTexture ( void );
   DxTexture& operator= ( const DxTexture& other );
   operator IDXTEXTURE ( void );

   UINT width ( void ) { return myTextureInfo.Width; }
   UINT height ( void ) { return myTextureInfo.Height; }
   
   bool create ( IDXDEVICE device, int width, int height, DWORD usage = 0 );
   bool create ( IDXDEVICE device, LPCVOID pData, UINT dataSize, D3DCOLOR transcolor );
   bool create ( IDXDEVICE device, const tstring& filename, D3DCOLOR transcolor, POINT* srcSize = NULL );
   bool create ( IDXDEVICE device, const tstring& filename, Rect& srcRect, DWORD usage = 0 );
   void destroy ( void );

   void stretchRect ( IDXDEVICE device, RECT* srcRect, IDXTEXTURE dstTexture, RECT* dstRect );

   HRESULT draw ( IDXSPRITE spriteobj, float x, float y, float rotation = 0, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ) );
   HRESULT drawScale ( IDXSPRITE spriteobj, float x, float y, float width, float height, float rotation = 0, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ) );
   HRESULT drawStretch ( IDXSPRITE spriteobj, Rect* srcRect, Rect* dstRect, float rotation = 0, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ) );

   HRESULT drawEx ( IDXSPRITE spriteobj, RECT* srcRect, D3DXVECTOR2* center, D3DXVECTOR3* position, float rotation, 
                  D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ) , D3DXVECTOR2* scale = NULL );

   inline const tstring& name ( ) { return myName; }
   inline const tstring& name ( const tstring& name ) { return (myName = name); }

private:  
   tstring        myName;
   IDXTEXTURE     myTexture;
   D3DXIMAGE_INFO myTextureInfo;
};

#endif //_DXTEXTURE_H_