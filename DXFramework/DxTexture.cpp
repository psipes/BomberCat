#include "stdafx.h"
#include <cassert>
#include "Utilities/Rect.h"
#include "Utilities/Point.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxFramework.h"
#include "DxFramework/DxTexture.h"
#include "DxFramework/DxSurface.h"

//=======================================================================
DxTexture::DxTexture ( void )
:myTexture(NULL)
{
   ZeroMemory( &myTextureInfo, sizeof(myTextureInfo) );
}

//=======================================================================
DxTexture::DxTexture ( const DxTexture& other )
:DxImage(other), myTexture(NULL)
{
   ZeroMemory( &myTextureInfo, sizeof(myTextureInfo) );
   *this = other;
}

//=======================================================================
DxTexture::~DxTexture ( void )
{
   destroy();
}

//=======================================================================
DxTexture& DxTexture::operator= ( const DxTexture& other )
{
   DxImage::operator=( other );
   if ( myTexture )
   {
      destroy();
   }
   if ( other.myTexture )
   {
      other.myTexture->AddRef();
      myTexture = other.myTexture;
      myTextureInfo = other.myTextureInfo;
   }
   return *this;
}


//=======================================================================
DxTexture::operator IDXTEXTURE ( void )
{
   return myTexture;
}

//=======================================================================
bool DxTexture::create ( IDXDEVICE device, const tstring& filename, Rect& srcRect, DWORD usage )
{
   HRESULT hr;
   bool  result;

   assert( getInfoFromFile( filename ) );

   // Load the file as a SURFACE, to get just the srcRect
   DxSurface srcSurface;
   result = srcSurface.createFromFile( device, filename, &srcRect );   
   assert(result);

   // Create appropriately sized TEXTURE, blank
   result = create( device, srcRect.width(), srcRect.height(), D3DUSAGE_RENDERTARGET );
   assert(result);

   // Get the TEXTURES surface[0]
   IDXSURFACE  texSurface = NULL;
   hr = myTexture->GetSurfaceLevel( 0, &texSurface );
   assert(SUCCEEDED(hr));
   
   // BitBlt from srcSurface to texture surface[0]
   hr = srcSurface.draw( device, NULL, texSurface, NULL );
   assert(SUCCEEDED(hr));

   IfRelease(&texSurface);

   srcSurface.destroy();
   return result;
}

//=======================================================================
bool DxTexture::create ( IDXDEVICE device, int width, int height, DWORD usage )
{
   HRESULT result;
   result = D3DXCreateTexture( device,             //Direct3D device object
                               width,              //image width
                               height,             //image height
                               1,                  //mip-map levels (1 for no chain)
                               usage,              //D3DUSAGE: )
                               D3DFMT_A8R8G8B8,     //texture format (default)
                               D3DPOOL_DEFAULT,    //memory class for the texture
                               &myTexture          //destination texture
                               );  

   if ( SUCCEEDED( result ) )
   {
      D3DSURFACE_DESC surfaceInfo;
      myTexture->GetLevelDesc( 0, &surfaceInfo );

      myTextureInfo.Width = surfaceInfo.Width;
      myTextureInfo.Height= surfaceInfo.Height;
      myTextureInfo.Format= surfaceInfo.Format;
      myTextureInfo.ResourceType = D3DRTYPE_TEXTURE;  // Guessing

      myTextureInfo.MipLevels = 1;                    // from create call, above
      myTextureInfo.Depth     = 1;                    // Guessing...
      myTextureInfo.ImageFileFormat = D3DXIFF_BMP;    // for lack of better idea here...
      
   }

   return SUCCEEDED( result );
}

//bool DxTexture::create( IDXDEVICE device, int width, int height, DWORD usage, 

//=======================================================================
bool DxTexture::create ( IDXDEVICE device, LPCVOID pData, UINT dataSize, D3DCOLOR transcolor )
{
   HRESULT result;
   assert( getInfoFromFileInMemory( pData, dataSize ) );

   result = D3DXCreateTextureFromFileInMemoryEx ( device,              //Direct3D device object
                                                  pData,               //Pointer to the file in memory
                                                  dataSize,            //the size of the file in memory height
                                                  fileInfo().Width,    //bitmap image width
                                                  fileInfo().Height,   //bitmap image height
                                                  1,                   //mip-map levels (1 for no chain)
                                                  D3DPOOL_DEFAULT,     //the type of surface (standard)
                                                  D3DFMT_A8R8G8B8,      //surface format (default)
                                                  D3DPOOL_DEFAULT,     //memory class for the texture
                                                  D3DX_DEFAULT,        //image filter
                                                  D3DX_DEFAULT,        //mip filter
                                                  transcolor,          //color key for transparency
                                                  &myTextureInfo,      //bitmap file myFileInfo (from loaded file)
                                                  NULL,                //color palette
                                                  &myTexture );        //destination texture
   return SUCCEEDED( result );
}

//=======================================================================
bool DxTexture::create ( IDXDEVICE device, const tstring& filename, D3DCOLOR transcolor, POINT* srcSize )
{
   HRESULT result;
   assert( getInfoFromFile( filename ) );
   DWORD usage = 0;  // 0 (default), D3DUSAGE_RENDERTARGET, D3DUSAGE_DYNAMIC

   result = D3DXCreateTextureFromFileEx( device,                                         //Direct3D device object
						                 filename.c_str(),                               //bitmap filename
                                         (srcSize ? (srcSize->x) : fileInfo().Width),    //bitmap image width
                                         (srcSize ? (srcSize->y) : fileInfo().Height),   //bitmap image height
						                       1,                                              //mip-map levels (1 for no chain)
						                       usage,                                          //Usage (see above)
						                       D3DFMT_A8R8G8B8,                                 //surface format (default)
						                       D3DPOOL_DEFAULT,                                //memory class for the texture
						                       D3DX_DEFAULT,                                   //image filter
						                       D3DX_DEFAULT,                                   //mip filter
						                       transcolor,                                     //color key for transparency
                                         &myTextureInfo,                                 //bitmap file myFileInfo (from loaded file)
						                       NULL,                                           //color palette
						                       &myTexture );                                   //destination texture
   return SUCCEEDED( result );
}

//=======================================================================
void DxTexture::destroy ( void )
{
   removeFileInfo();
   IfRelease( &myTexture );
   ZeroMemory( &myTextureInfo, sizeof(myTextureInfo) );
}


//=======================================================================
void DxTexture::stretchRect ( IDXDEVICE device, RECT* srcRect, IDXTEXTURE dstTexture, RECT* dstRect )
{
   IDXSURFACE srcSurface = NULL, dstSurface = NULL, prevTarget = NULL;
   HRESULT hr;   

   hr = myTexture->GetSurfaceLevel( 0, &srcSurface );
   assert( SUCCEEDED(hr) );

   hr = dstTexture->GetSurfaceLevel( 0, &dstSurface );
   assert( SUCCEEDED(hr) );

   D3DSURFACE_DESC srcDescr, dstDescr;
   srcSurface->GetDesc( &srcDescr );
   dstSurface->GetDesc( &dstDescr );

   if ( dstDescr.Usage & D3DUSAGE_RENDERTARGET )
   {
      hr = device->StretchRect( srcSurface, srcRect, dstSurface, dstRect, D3DTEXF_NONE );
      assert(SUCCEEDED(hr));
   }

   IfRelease(&srcSurface);
   IfRelease(&dstSurface);
   IfRelease(&prevTarget);
}


//=======================================================================
HRESULT DxTexture::draw ( IDXSPRITE spriteobj, float x, float y, float rotation, D3DCOLOR color )
{	
   D3DXVECTOR3 position( x, y, 0.0f );
   return drawEx( spriteobj, NULL, NULL, &position, rotation, color, NULL );
}

//======================================================================
HRESULT DxTexture::drawScale ( IDXSPRITE spriteobj, float x, float y, float width, float height, float rotation, D3DCOLOR color )
{
   Rect dstRect( (LONG)x, (LONG)y, (LONG)(x + width), (LONG)(y + height) );
   return drawStretch( spriteobj, NULL, &dstRect, rotation, color );
}

//======================================================================
HRESULT DxTexture::drawStretch ( IDXSPRITE spriteobj, Rect* srcRect, Rect* dstRect, float rotation, D3DCOLOR color )
{
   D3DXVECTOR3 position( (dstRect ? dstRect->left : 0.0f), (dstRect ? dstRect->top : 0.0f), 0.0f );
   D3DXVECTOR2 scale( (dstRect ? dstRect->width() : 1.0f) / (srcRect ? srcRect->width() : width()),
                      (dstRect ? dstRect->height() : 1.0f) / (srcRect ? srcRect->height() : height()) );
   return drawEx( spriteobj, srcRect, NULL, &position, rotation, color, &scale );

}

//=======================================================================
HRESULT DxTexture::drawEx ( IDXSPRITE spriteobj, RECT* srcRect, D3DXVECTOR2* center, D3DXVECTOR3* position, 
                          float rotation, D3DCOLOR color, D3DXVECTOR2* scale )
{
	D3DXMATRIX matrix, lastMatrix;
	HRESULT result;
   D3DXVECTOR2 center2d( (center ? center->x : 0.0f) + (position ? position->x : 0.0f), 
                         (center ? center->y : 0.0f) + (position ? position->y : 0.0f) );
	D3DXVECTOR3 center3d( (center ? center->x : 0.0f), (center ? center->y : 0.0f), 0.0f ); 

   D3DXMatrixTransformation2D( &matrix, &center2d, 0, scale, &center2d, rotation, NULL );
   
   result = spriteobj->GetTransform( &lastMatrix );
   assert( SUCCEEDED( result ) );

	result = spriteobj->SetTransform( &matrix );
   assert( SUCCEEDED( result ) );

   result = spriteobj->Draw( myTexture, srcRect, &center3d, position, color );
	assert( SUCCEEDED( result ) );

   result = spriteobj->SetTransform( &lastMatrix );
   assert( SUCCEEDED( result ) );

   return result;
}
