#include "stdafx.h"
#include <cassert>
#include <map>
#include <fstream>
#include "Utilities/TTypes.h"
#include "Utilities/Rect.h"
#include "Utilities/Point.h"
#include "Utilities/TStringRoutines.h"
#include "DxFramework/DxTexture.h"
#include "DxFramework/DxAnimation.h"
//=======================================================================
DxAnimation::DxAnimation ()
:myName(), myCurrentFrame(0), myFrameDirection(0), myFrameCount(0)
{  
   memset( this->myFrames, 0xA5, sizeof( this->myFrames ) );
}

//=======================================================================
DxAnimation::~DxAnimation ( )
{
}

//=======================================================================
//bool DxAnimation::init ( IDXDEVICE device, const tstring& textureFilename, const tstring& animDescrStr, float speed ) 
//{
//   bool result = false;
//   mySpeed = speed;
//   //if ( !myFrames.empty() )
//   //{
//   //   myFrames.clear();
//   //}
//
//   //tstring toParse = trimFront( _T("(") + textureFilename + _T(")[") + trimPath(textureFilename) + _T("]") + animDescrStr );
//   //result = parse( device, toParse );
//   //if ( result && animDescrStr.empty() )
//   //{
//   //   DxTexture& texture = myTextures.begin()->second;
//   //   myFrames.push_back( std::pair<Rect,DxTexture*>( Rect( Point(), texture.width(), texture.height() ), &texture ) );
//   //}
//   return result;
//}

//=======================================================================
bool DxAnimation::init ( DxTexture* texture, const tstring& animationDesc, float speed, D3DCOLOR excludeColor )
{
   Point pos;
   int width = -1, height = -1;
   tstringstream ss( animationDesc );
   int type = -1;  

   ss >> myName >> type;
   if ( ss.fail() )
   {
      return false;
   }

   animation( (ANIMATION)type );

   Rect  texRect( 0,0, texture->width(), texture->height() );

   while ( true )
   {
      DxAnimationFrame frame;
      ss >> pos.x >> pos.y >> width >> height;
      if ( ss.fail() )
      {
         break;
      }
      frame.rect.set( pos, width, height );
      frame.texture = texture;
      
      //TODO SAS - Frame rectangle should be verified - like below - to be within texture bounds?
      //           Then... if it ISN'T, do what?
      Rect intersection;
      ::IntersectRect( &intersection, &frame.rect, &texRect );
      if ( intersection != frame.rect )
      {
         logln( _T(__FUNCTION__) _T(": frame dimensions are out of texture bounds: %s"), myName.c_str() );
      }

      addFrame( frame );
   }
   mySpeed = speed;
   myExcludeColor = excludeColor;
   return myTimer.start();
}

//=======================================================================
bool DxAnimation::init ( DxTexture* texture, D3DCOLOR excludeColor )
{
   Point pos;
   DxAnimationFrame frame;
   frame.rect.set( 0, 0, texture->width(), texture->height() );
   frame.texture = texture;
   addFrame( frame ); 
   mySpeed = 0;
   myExcludeColor = excludeColor;
   return myTimer.start();
}

//=======================================================================
void DxAnimation::update ()
{
   if ( !myTimer.isRunning() )
      return;

   if ( mySpeed == 0 || myTimer.elapsedTime() < 1000 / mySpeed )
      return;

   int nextFrame = myCurrentFrame + myFrameDirection;

   switch ( myAnimation )
   {
   case ANIMATION::SINGLE:
      {
         myFrameDirection = 1;
         if ( nextFrame >= ((int)myFrameCount - 1) )
         {
            nextFrame = (myFrameCount - 1);
            myTimer.stop();
         }
      }
      break;

   case ANIMATION::LOOP:
      {
         myFrameDirection = 1;
         nextFrame = nextFrame % myFrameCount;
      }
      break;

   case ANIMATION::ROCKER:
      {
         if ( nextFrame >= ((int)myFrameCount - 1) )
         {
            nextFrame = (myFrameCount - 1);
            myFrameDirection = -1;
         }
         else if ( nextFrame <= 0 )
         {
            nextFrame = 0;
            myFrameDirection = 1;
         }
      }
      break;
   default:
      myFrameDirection = 0;
      break;
   } 
   myCurrentFrame = nextFrame;
   myTimer.restart();
}

//=======================================================================
void DxAnimation::shutdown ()
{
   for ( unsigned int index = 0; index < ourMaxAnimationFrames; index++ )
   {
      myFrames[index].texture = NULL;
   }
}

//=======================================================================
void DxAnimation::reset() //added for resetting animation so it doesn't start mid frame
{
   myCurrentFrame = 0;
   myTimer.restart();
}

//=======================================================================
void DxAnimation::play ()
{
   myTimer.start();
}


//=======================================================================
void DxAnimation::stop ()
{
   myTimer.stop();
}

//=======================================================================
bool DxAnimation::isPlaying ()
{
   return myTimer.isRunning();
}

//=======================================================================
bool DxAnimation::hasEnded ()
{
   switch ( myAnimation )
   {
   case ANIMATION::SINGLE:
   case ANIMATION::LOOP:
      return (myCurrentFrame == (myFrameCount - 1) );

   case ANIMATION::ROCKER:
      return ( myCurrentFrame == (myFrameCount - 1) && myFrameDirection > 0) || 
             ( myCurrentFrame == 0 && myFrameDirection < 0 );
      break;
   }
   return false;
}


//=======================================================================
DxAnimation::ANIMATION DxAnimation::animation ( ANIMATION type )
{
   myAnimation = type;
   myFrameDirection = 1;
   return myAnimation;
}


//=======================================================================
float DxAnimation::speed ( float value )
{
   mySpeed = value;
   return mySpeed;
}

//=======================================================================
void DxAnimation::drawFrame ( IDXSPRITE spriteobj, float x, float y, float rotation, D3DCOLOR color )
{
   if ( myFrames[myCurrentFrame].texture )
   {
      Rect dstRect;
      dstRect.x((LONG)x);
      dstRect.y((LONG)y);
      dstRect.width(myFrames[myCurrentFrame].rect.width());
      dstRect.height(myFrames[myCurrentFrame].rect.height());
      myFrames[myCurrentFrame].texture->drawStretch ( spriteobj, &myFrames[myCurrentFrame].rect, &dstRect, rotation, color );
   }
}


//=======================================================================
void DxAnimation::drawFrame ( IDXSPRITE spriteobj, D3DXVECTOR3* position, 
                              D3DXVECTOR2* scale, float rotation, 
                              D3DXVECTOR2* center, D3DCOLOR color )
{
   if ( myFrames[myCurrentFrame].texture )
   {
      myFrames[myCurrentFrame].texture->drawEx ( spriteobj, &myFrames[myCurrentFrame].rect,
                                                 center, position, rotation, color, scale );
   }
}

//=======================================================================
//private
//
//bool DxAnimation::parse ( DxAnimationInfo animInfo )
//{
//   tstring parseStr( animInfo.information );
//   TCHAR state = _T('\0');
//   tstringstream ss( parseStr );
//   int animationType = -1;
//
//   ss >> myName >> animationType;
//   animation( (ANIMATION)animationType );
//
//   while ( true )
//   {
//      int x = -1, y = -1, width = -1, height = -1;
//      Rect rect;
//
//      ss >> x >> y >> width >> height;
//      if ( ss.fail() )
//      {
//         break;
//      }
//
//      assert( width != -1 && height != -1 );
//      rect.set( Point( x, y ), width, height );
//
//      if ( myFrameCount >= ANIMATION_MAX_LENGTH )
//      {
//         return false;
//      }
//
//      myFrames[myFrameCount] = rect;
//      myFrameCount++;
//   }
//   return true;
//}

//=======================================================================
//private
//tstring getNextToken ( const tstring& str, const tstring& pattern, char& state )
//{
//   size_t index = str.find_first_of( pattern );
//   size_t endIndex;
//   if ( index == tstring::npos || index == (str.length() - 1) )
//   {
//      assert( false );
//      return false;
//   }
//
//   endIndex = str.find_first_of( pattern, index + 1 );
//   if ( endIndex == tstring::npos )
//      endIndex = str.length();
//
//   state = str[index];
//   return str.substr( index + 1, (endIndex - index) - 1 );
//}