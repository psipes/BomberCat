#pragma once

#if !defined(_DXTEXT_H_)
#define _DXTEXT_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "Utilities/NonCopyable.h"
#include "Utilities/TTypes.h"
#include "Utilities/Rect.h"
#include "Utilities/Point.h"
#include "DxFramework/DxTexture.h"
#include "DxFramework/DxTypes.h"

class DxText
{
public:
	DxText ( void );
	~DxText ( void );

public:

   bool create( IDXFONT fontObj, int inputInt, int xPixPos, int yPixPos, int width, int height, D3DCOLOR color = D3DCOLOR_XRGB( 0, 0, 0 ) ); 
   bool create( IDXFONT fontObj, tstring inputString, int xPixPos, int yPixPos, int width, int height, D3DCOLOR color = D3DCOLOR_XRGB( 0, 0, 0 ) );//whatever's needed for the DrawText method
   bool update();
   bool destroy();

   bool draw( IDXSPRITE spriteObj );

   void setText( tstring newString );
   void setText ( int number );
   void setText ( float number );

   void setPosition ( float x, float y );
   void setXPosition ( float value );
   void setYPosition ( float value );

   float getXPosition () { return myPosition.x; }
   float getYPosition () { return myPosition.y; }

   void setScale( float scaleX, float scaleY );

   void clear();


private:


private:
   IDXFONT        myFont;
   tstring        myString;
   D3DCOLOR       myColor;
   D3DXVECTOR3    myPosition;
   D3DXVECTOR3    myScale;
   int            myWidth;
   int            myHeight;
   Rect           myRect;   
   tstringstream  myConverter;
};

#endif //_DXGAMESPRITE_H_