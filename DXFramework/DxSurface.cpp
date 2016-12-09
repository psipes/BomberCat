#include "stdafx.h"
#include <assert.h>
#include "Utilities/Rect.h"
#include "Utilities/Point.h"
#include "Utilities/Logger.h"
#include "Utilities/Utilities.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxSurface.h"

//=======================================================================
DxSurface::DxSurface ( void )
:mySurface( NULL )
{
   ZeroMemory( &mySurfaceDescription, sizeof(mySurfaceDescription) );
}

//=======================================================================
DxSurface::DxSurface ( const DxSurface& other )
:DxImage(other), mySurface(NULL)
{
   ZeroMemory( &mySurfaceDescription, sizeof(mySurfaceDescription) );
   *this = other;
}

//=======================================================================
DxSurface::~DxSurface ( void )
{
   destroy();
}

//=======================================================================
DxSurface& DxSurface::operator= ( const DxSurface& other )
{
   DxImage::operator=( other );
   if ( mySurface )
   {
      destroy();
   }
   if ( other.mySurface )
   {
      other.mySurface->AddRef();
      mySurface = other.mySurface;
      mySurfaceDescription = other.mySurfaceDescription;
   }
   return *this;
}

//=======================================================================
bool DxSurface::createEmpty ( IDXDEVICE device, unsigned width, unsigned height )
{
   HRESULT result = device->CreateOffscreenPlainSurface( width, height, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &mySurface, NULL );
   if ( FAILED( result ) )
   {
      log( _T("Error: Could not create Surface!\n") );
      return false;
   }
   result = mySurface->GetDesc( &mySurfaceDescription );
   return SUCCEEDED(result);
}

//=======================================================================
bool DxSurface::createFromFile ( IDXDEVICE device, const tstring& filepath, const RECT* srcRect )
{
   DWORD colorKey = 0;
   D3DXIMAGE_INFO info;

   if ( !getInfoFromFile( filepath ) )
   {
      log( _T("Error: Could not load file!\n") );
      return false;
   }
   if ( !createEmpty( device, 
                      (srcRect ? ( srcRect->right - srcRect->left ): fileInfo().Width), 
                      (srcRect ? ( srcRect->bottom - srcRect->top ): fileInfo().Height) ) )
   {
      return false;
   }

   HRESULT result = D3DXLoadSurfaceFromFile( mySurface, NULL, NULL, filepath.c_str(), srcRect, D3DX_DEFAULT, colorKey, &info );
   return SUCCEEDED( result );

}

//=======================================================================
void DxSurface::destroy ( void )
{
   removeFileInfo();
   IfRelease( &mySurface );
   ZeroMemory( &mySurfaceDescription, sizeof(mySurfaceDescription) );
}

//=======================================================================
HRESULT DxSurface::setToBackBuffer ( IDXDEVICE device, UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE type )
{
   //todo WTF???
   return device->GetBackBuffer( iSwapChain, iBackBuffer, type, &mySurface );
}

//=======================================================================
HRESULT DxSurface::draw ( IDXDEVICE device, LPRECT sRect, LPDIRECT3DSURFACE9 target, LPRECT tRect, D3DTEXTUREFILTERTYPE filter )
{
   //todo sas: Ok... passing in a valid srcRect, and dstRect, and this... this... I have no words for it..
   //          ...this 'stuff' ends up computing wrong values. Sorry. Crap.  Toss it.

   //D3DVIEWPORT9 viewport = {0};
   //device->GetViewport( &viewport );      //todo... ok, I hope this isn't to avoid using winScreenWidth(), etc....

   //Rect rCSource( (sRect ? sRect->left : 0), 
   //               (sRect ? sRect->top : 0), 
   //               (sRect ? (sRect->right - sRect->left) : width( )), 
   //               (sRect ? (sRect->bottom - sRect->top) : height( )) );
   //Rect rCTarget( (tRect ? tRect->left : viewport.X), 
   //               (tRect ? tRect->top : viewport.Y), 
   //               (tRect ? (tRect->right - tRect->left) : viewport.Width), 
   //               (tRect ? (tRect->bottom - tRect->top) : viewport.Height) );

   //rCTarget.right( min( (LONG)(viewport.X + viewport.Width), rCTarget.right( ) ) );
   //rCTarget.bottom( min( (LONG)(viewport.Y + viewport.Height), rCTarget.bottom( ) ) );

   //rCSource.right( min( rCTarget.width( ), rCSource.width( ) ) );
   //rCSource.bottom( min( rCTarget.height( ), rCSource.height( ) ) );

   //rCSource.left( rCTarget.left( ) < 0 ?  abs( rCSource.left( ) - rCTarget.left( ) ) : rCSource.left( ) );
   //rCSource.top( rCTarget.top( ) < 0 ?  abs( rCSource.top( ) - rCTarget.top( ) ) : rCSource.top( ) );

   //rCTarget.left( max( (LONG)viewport.X, rCTarget.left( ) ) );
   //rCTarget.top( max( (LONG)viewport.Y, rCTarget.top( ) ) );

   HRESULT hr = device->StretchRect( mySurface, sRect, target, tRect, filter );
   return hr;
}

//=======================================================================
bool DxSurface::setColor( IDXDEVICE device, D3DCOLOR color )
{
	HRESULT result = device->ColorFill( mySurface, NULL, color );

   return ( SUCCEEDED( result ) );
}