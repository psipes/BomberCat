#pragma once

#if !defined(_DXGEOMETRY_H_)
#define _DXGEOMETRY_H_
#include <cassert>
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"

bool copyFromVertexBuffer ( LPDIRECT3DVERTEXBUFFER9 vertexBuffer, unsigned int srcSize, void* dstBuffer, unsigned int srcOffset = 0 );
//bool copyToVertexBuffer ( IDXDEVICE device, void* srcData, unsigned int srcSize, LPDIRECT3DVERTEXBUFFER9* vertexBuffer, bool clearVertexBuffer = false );

//=======================================================================
//=======================================================================
class _DxGeometry
{
public:

   _DxGeometry () {myVertexBufferOffset = 0; myVertexCount = 0; myVertexSize = 0; myVertexBuffer = NULL; D3DXMatrixIdentity(&myTransformation); }
   ~_DxGeometry () {destroy();}
   //void* operator[] ( unsigned int index );

   bool create ( IDXDEVICE device, LPDIRECT3DVERTEXBUFFER9* vertexBuffer, void* pVertices, unsigned int vertexCount, unsigned int vertexSize, DWORD fvf );
   void destroy ();

   

   void translate ( FLOAT x, FLOAT y, FLOAT z );
   void scale ( FLOAT x, FLOAT y, FLOAT z );
   void rotate ( FLOAT yaw, FLOAT pitch, FLOAT roll );

   void draw ( IDXDEVICE device, D3DPRIMITIVETYPE primitiveType, unsigned int primitiveCount, IDXTEXTURE texture = NULL );

private:

private:   
   unsigned int      myVertexBufferOffset;
   unsigned int      myVertexCount;
   unsigned int      myVertexSize;
   DWORD             myFVF;
   IDXVERTEXBUFFER   myVertexBuffer;
   D3DXMATRIXA16     myTransformation;
};

//=======================================================================
//=======================================================================
template <typename TYPE>
class DxGeometry
{
public:

   DxGeometry () {}
   ~DxGeometry () {}
   
   //=======================================================================
   //TYPE& operator[] ( unsigned int index )
   //{
   //   int rawIndex = index * sizeof(TYPE);
   //   void* pVertex = myGeometry[rawIndex];

   //   if ( pVertex != NULL && ( rawIndex + (sizeof(TYPE) - 1) ) < myGeometry.size() )
   //   {
   //      return *(static_cast<TYPE*>(pVertex));
   //   }
   //   assert(false);

   //   throw NULL;
   //}

   //=======================================================================
   bool create ( IDXDEVICE device, LPDIRECT3DVERTEXBUFFER9* vertexBuffer, void* pVertices, unsigned int vertexCount, DWORD fvf )
   {
      return myGeometry.create( device, vertexBuffer, pVertices, vertexCount, sizeof(TYPE), fvf );
   }


   //=======================================================================
   void destroy ()
   {
      myGeometry.destroy();
   }

   //=======================================================================
   void translate ( FLOAT x, FLOAT y, FLOAT z )
   {
      myGeometry.translate(x, y, z);
   }

   //=======================================================================
   void scale ( FLOAT x, FLOAT y, FLOAT z )
   {
      myGeometry.scale(x, y, z);
   }

   //=======================================================================
   void rotate ( FLOAT yaw, FLOAT pitch, FLOAT roll )
   {
      myGeometry.rotate(yaw, pitch, roll);
   }

   //=======================================================================
   void draw ( IDXDEVICE device, D3DPRIMITIVETYPE primitiveType, unsigned int primitiveCount = 0, IDXTEXTURE texture = NULL )
   {
      myGeometry.draw( device, primitiveType, primitiveCount, texture );
   }
private:
   _DxGeometry myGeometry;
};

#endif //_DXGEOMETRY_H_