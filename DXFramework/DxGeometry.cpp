#include "stdafx.h"

#include "DxGeometry.h"

//=======================================================================
bool copyFromVertexBuffer ( LPDIRECT3DVERTEXBUFFER9 vertexBuffer, unsigned int srcSize, void* dstBuffer, unsigned int srcOffset )
{
   HRESULT hr;

   D3DVERTEXBUFFER_DESC desc;
   hr = vertexBuffer->GetDesc( &desc );
   assert( SUCCEEDED(hr) );

   srcSize = ( srcSize == 0 ? desc.Size : srcSize );

   VOID* pVertices = NULL;
   if ( srcSize <= desc.Size && SUCCEEDED(vertexBuffer->Lock(srcOffset, srcSize, &pVertices, D3DLOCK_READONLY)) )
   {
      memcpy( dstBuffer, pVertices, srcSize );

      hr = vertexBuffer->Unlock();
      return SUCCEEDED(hr);
   }
   return false;
}

//=======================================================================
bool _DxGeometry::create ( IDXDEVICE device, LPDIRECT3DVERTEXBUFFER9* vertexBuffer, void* pVertices, unsigned int vertexCount, unsigned int vertexSize, DWORD fvf )
{
   if ( !pVertices )
   {
      return false;
   }

   HRESULT hr;
   D3DVERTEXBUFFER_DESC desc;
   ZeroMemory( &desc, sizeof(desc) );
   unsigned char* fullBuffer = NULL;
   unsigned int fullBufferSize = 0;

   myVertexCount = vertexCount;
   myVertexSize = vertexSize;
   myVertexBufferOffset = 0;
   myFVF = fvf;
   
   if ( *vertexBuffer )
   {
      (*vertexBuffer)->GetDesc( &desc );
      myVertexBufferOffset = desc.Size;
      fullBufferSize = desc.Size + (myVertexCount * myVertexSize);
      fullBuffer = new unsigned char[fullBufferSize];
      assert( copyFromVertexBuffer( *vertexBuffer, desc.Size, fullBuffer ) );
      IfRelease(vertexBuffer);
   }
   else
   {
      fullBufferSize = myVertexCount * myVertexSize;
      fullBuffer = new unsigned char[fullBufferSize];
   }

   memcpy( fullBuffer + myVertexBufferOffset, pVertices, (myVertexCount * myVertexSize) );
   
   hr = device->CreateVertexBuffer( (vertexCount * vertexSize) + myVertexBufferOffset, 0, myFVF, D3DPOOL_DEFAULT, vertexBuffer, NULL );
   assert(SUCCEEDED(hr));

   myVertexBuffer = *vertexBuffer;
   myVertexBuffer->AddRef();

   VOID* pVertexIndex = NULL;
   hr = myVertexBuffer->Lock( 0, fullBufferSize, ( void** )&pVertexIndex, 0 );
   if ( SUCCEEDED(hr) )

   {
      memcpy( pVertexIndex, fullBuffer, fullBufferSize );

      hr = myVertexBuffer->Unlock();
      return SUCCEEDED(hr);
   }

   delete[] fullBuffer;

   return SUCCEEDED(hr);
}

//=======================================================================
void _DxGeometry::destroy ()
{
   if ( !myVertexBuffer )
      return;

   myVertexBufferOffset = 0;
   myVertexCount = 0;
   myVertexSize = 0;
   IfRelease(&myVertexBuffer);
}

//=======================================================================
void _DxGeometry::translate ( FLOAT x, FLOAT y, FLOAT z )
{
   D3DXMATRIXA16 transformation;
   D3DXMatrixTranslation( &transformation, x, y, z );
   myTransformation = transformation * myTransformation;
}

//=======================================================================
void _DxGeometry::scale ( FLOAT x, FLOAT y, FLOAT z )
{
   D3DXMATRIXA16 transformation;
   D3DXMatrixScaling( &transformation, x, y, z );
   myTransformation = transformation * myTransformation;
}

//=======================================================================
void _DxGeometry::rotate ( FLOAT yaw, FLOAT pitch, FLOAT roll )
{
   D3DXMATRIXA16 transformation;
   D3DXMatrixRotationYawPitchRoll( &transformation, yaw, pitch, roll );
   myTransformation = transformation * myTransformation;
}

//=======================================================================
void _DxGeometry::draw ( IDXDEVICE device, D3DPRIMITIVETYPE primitiveType, unsigned int primitiveCount, IDXTEXTURE texture )
{
   HRESULT hr;
   D3DXMATRIXA16 prevWorldTransform, worldTransform;

   device->GetTransform( D3DTS_WORLD, &prevWorldTransform );

   worldTransform = myTransformation * prevWorldTransform;

   device->SetTransform( D3DTS_WORLD, &worldTransform );

   if ( primitiveCount == 0 )
   {
      switch (primitiveType)
      {
      case D3DPT_LINELIST:
         primitiveCount = myVertexCount / 2;
         break;

      case D3DPT_LINESTRIP:
         primitiveCount = myVertexCount - 1;
         break;

      case D3DPT_TRIANGLELIST:
         primitiveCount = myVertexCount / 3;
         break;

      case D3DPT_TRIANGLESTRIP:
         primitiveCount = myVertexCount - 2;
         break;

      case D3DPT_TRIANGLEFAN:
         primitiveCount = myVertexCount - 2;
         break;

      case D3DPT_POINTLIST:
         primitiveCount = myVertexCount;
         break;
      }
   }

   if ( texture )
   {
      hr = device->SetTexture( 0, texture );
      assert( SUCCEEDED(hr) );
   }

   hr = device->SetStreamSource( 0, myVertexBuffer, myVertexBufferOffset, myVertexSize );
   assert( SUCCEEDED(hr) );
   
   hr = device->SetFVF( myFVF );
   assert( SUCCEEDED(hr) );
   
   hr = device->DrawPrimitive( primitiveType, 0, primitiveCount );
   assert( SUCCEEDED(hr) );

   device->SetTransform( D3DTS_WORLD, &prevWorldTransform );

}
