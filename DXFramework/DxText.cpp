#include "stdafx.h"
#include <cassert>
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxText.h"

//=======================================================================
DxText::DxText()
{
   myFont = NULL;
}

//=======================================================================
DxText::~DxText()
{
   destroy();
}

//=======================================================================
bool DxText::create( IDXFONT fontObj, tstring inputString, int xPixPos, int yPixPos, int width, int height, D3DCOLOR color )
{
   myFont = fontObj;
   myFont->AddRef();

   myString = inputString;

   myColor = color;

   myPosition.x = float(xPixPos);
   myPosition.y = float(yPixPos);

   myWidth  = width;
   myHeight = height;

   myRect.left   =  LONG(myPosition.x);
   myRect.right  =  LONG(myPosition.x + myWidth);
   myRect.top    =  LONG(myPosition.y);
   myRect.bottom =  LONG(myPosition.y + myHeight);

   return true;
}

//=======================================================================
bool DxText::create( IDXFONT fontObj, int inputInt, int xPixPos, int yPixPos, int width, int height, D3DCOLOR color )
{
   myFont = fontObj;
   myFont->AddRef();

   setText( inputInt );

   myColor = color;

   myPosition.x = float(xPixPos);
   myPosition.y = float(yPixPos);

   myWidth  = width;
   myHeight = height;

   myRect.left   =  LONG(myPosition.x);
   myRect.right  =  LONG(myPosition.x + myWidth);
   myRect.top    =  LONG(myPosition.y);
   myRect.bottom =  LONG(myPosition.y + myHeight);

   return true;
}
//=======================================================================
bool DxText::update()
{
   return true;
}

//=======================================================================
bool DxText::destroy()
{
   myString = _T("");
   myPosition = D3DXVECTOR3( 0, 0, 0 );
   myScale = D3DXVECTOR3( 0, 0, 0 );
   myWidth = NULL;
   myHeight = NULL;
   myRect.set( 0, 0, 0, 0 );
   IfRelease( &myFont );
   return true;
}

//=======================================================================
bool DxText::draw( IDXSPRITE spriteObj )
{
   myFont->DrawText(spriteObj, myString.c_str(), -1, &myRect, DT_TOP|DT_RIGHT, myColor );

   return true;
}

//=======================================================================
void DxText::setText( tstring newString )
{
   myString = newString.c_str();
}

//=======================================================================
void DxText::setText( int number )
{
   myConverter.str( std::string() );
   myConverter << number;
   myString = _T( myConverter.str() );
}

//=======================================================================
void DxText::setText( float number )
{
   myConverter.str( std::string() );
   myConverter << number;
   myString = _T( myConverter.str() );
}

//=======================================================================
void DxText::setPosition ( float x, float y )
{
	setXPosition( x );
	setYPosition( y );

   myRect.left   =  LONG(myPosition.x);
   myRect.right  =  LONG(myPosition.x + myWidth);
   myRect.top    =  LONG(myPosition.y);
   myRect.bottom =  LONG(myPosition.y + myHeight);
}

//=======================================================================
void DxText::setXPosition ( float value )
{
	myPosition.x = value;

   myRect.left   =  LONG(myPosition.x);
   myRect.right  =  LONG(myPosition.x + myWidth);
   myRect.top    =  LONG(myPosition.y);
   myRect.bottom =  LONG(myPosition.y + myHeight);
}

//=======================================================================
void DxText::setYPosition ( float value )
{
	myPosition.y = value;

   myRect.left   =  LONG(myPosition.x);
   myRect.right  =  LONG(myPosition.x + myWidth);
   myRect.top    =  LONG(myPosition.y);
   myRect.bottom =  LONG(myPosition.y + myHeight);
}

//=======================================================================
void DxText::setScale ( float scaleX, float scaleY )
{
	myScale.x = scaleX;
	myScale.y = scaleY;

   myWidth *= 2;
   myHeight *= 2;

   myRect.right = myRect.left + myWidth;
   myRect.bottom = myRect.top+ myHeight;
}

//=======================================================================
void DxText::clear ( )
{
   myString = "";
}