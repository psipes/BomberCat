//-----------------------------------------------------------------------
//Provides more usable version of DirectX interface and device
//variables
//
//Benito Moncivais
//Issac Irlas
//-----------------------------------------------------------------------
#pragma once

#if !defined( _DXTYPES_H_ )
#define _DXTYPES_H_

#include "DxFramework/DxCommon.h"

typedef LPDIRECT3D9				   IDXINTERFACE;
typedef LPDIRECT3DDEVICE9		   IDXDEVICE;
typedef LPDIRECT3DSURFACE9		   IDXSURFACE;
typedef LPDIRECTINPUT8			   IDXINPUT;
typedef LPDIRECTINPUTDEVICE8	   IDXINPUTDEVICE;
typedef LPDIRECT3DTEXTURE9		   IDXTEXTURE;
typedef LPD3DXSPRITE			      IDXSPRITE;
typedef LPD3DXFONT               IDXFONT;
typedef LPD3DXBUFFER             IDXBUFFER;
typedef LPDIRECT3DVERTEXBUFFER9  IDXVERTEXBUFFER;

#endif //_DXTYPES_H_