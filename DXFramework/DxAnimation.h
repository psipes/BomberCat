#pragma once

#if !defined(_DXANIMATION_H_)
#define _DXANIMATION_H_
#define ANIMATION_MAX_LENGTH 256
#include <vector>
#include <map>
#include "Utilities/Rect.h"
#include "Utilities/TTypes.h"
#include "Utilities/Timer.h"
#include "DxFramework/DxAnimationInfo.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTexture.h"

//
// The animation can be described in two ways: In line with a string for a animation script file.
// In the first version of init use animation descrition string to format for each frame use the following
//    "RECT(xoffset yoffset width height) RECT(xoffset yoffset width height)"
//
// In the second version of init use the following from a file:
//# Base read file. This file will call any sprite sheets needed, 
//# load them, and allow us to create animations from specified
//# frames
//
//
//
//
//# all items following this are in this file.
//@spritesheet.png
//
//# Type		- The type of animatio, this can be the following:
//#			0 - Looping
//#			1 - Rocking
//#			2 - Single loop
//
//
//# ALL LINES MUST BE IN THE FOLLOWING FORMAT:
//# symbol			type	x y width height	{more x y width height's}
//#
//CANDYCORN-LOOP		0		916 646 32 36		948 646 32 36		980 646 32 36		1012 646 32 36
//SMILEYFACE-ROCK		1		1076 580 32 30		1110 576 28 34		1144 574 24 36
//ROCKCROC-ROCK		1		884  538 32 36   	918 538 32 36       948 538 32 36
//
//
//# allows multiple files.
//# @otherfile.ext
//

class DxAnimation
{
public:
   static const int ourMaxAnimationFrames = 256;
   typedef enum 
   {
      LOOP,
      ROCKER,
      SINGLE
   } ANIMATION;

   DxAnimation ();
   ~DxAnimation ();
   //DxAnimation ( const DxAnimation& other )
   //{
   //   myName           = other.myName;
   //   mySpeed          = other.mySpeed;
   //   myCurrentFrame   = other.myCurrentFrame;
   //   myFrameDirection = other.myFrameDirection;
   //   myExcludeColor   = other.myExcludeColor;
   //   myAnimation      = other.myAnimation;
   //   myFrameCount     = other.myFrameCount;
   //   memcpy( myFrames, other.myFrames, sizeof(myFrames) );
   //}

   //DxAnimation& operator= ( const DxAnimation& other )
   //{
   //   myName           = other.myName;
   //   mySpeed          = other.mySpeed;
   //   myCurrentFrame   = other.myCurrentFrame;
   //   myFrameDirection = other.myFrameDirection;
   //   myExcludeColor   = other.myExcludeColor;
   //   myAnimation      = other.myAnimation;
   //   myFrameCount     = other.myFrameCount;
   //   memcpy( myFrames, other.myFrames, sizeof(myFrames) );
   //   return *this;
   //}

   bool init ( DxTexture* texture, const tstring& animationDesc, float speed, D3DCOLOR excludeColor = D3DCOLOR_ARGB( 0, 0, 0, 0 ) );
   bool init ( DxTexture* texture, D3DCOLOR excludeColor = D3DCOLOR_ARGB( 0, 0, 0, 0 ) );
   void update ();
   void shutdown ();
   void reset ();
   void play ();
   void stop ();
   bool isPlaying ();
   bool hasEnded ();

   inline void addFrame ( DxAnimationFrame& frame )
   {
      myFrames[myFrameCount] = frame;
      myFrameCount++;
   }

   void setFrame ( int frame ) { frame = frame % myFrameCount; myCurrentFrame = frame; }

   int getCurrentFrame() const { return myCurrentFrame; }

   inline const tstring& name () { return myName; }

   inline const tstring& name ( const tstring& name ) { return (myName = name); }

   inline const D3DCOLOR excludeColor () { return myExcludeColor; }

   inline const D3DCOLOR excludeColor ( D3DCOLOR color ) { return (myExcludeColor = color); }

   ANIMATION animation ( ANIMATION type );
   ANIMATION animation ( ) { return myAnimation; }


   //TODO: Add conditional to make sure they don't blow up the game when called before a frame is made
   int width() const { return (myFrameCount > 0) ? myFrames[myCurrentFrame].width() : -1; }
   int height() const { return (myFrameCount > 0) ? myFrames[myCurrentFrame].height() : -1; }


   //TODO: Add pause() function ????


   float speed ( float value );
   float speed () { return mySpeed; }
   void drawFrame ( IDXSPRITE spriteobj, float x, float y, float rotation = 0, D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255 ) );
   void drawFrame ( IDXSPRITE spriteobj, D3DXVECTOR3* position, D3DXVECTOR2* scale, float rotation, D3DXVECTOR2* center, D3DCOLOR color );
   unsigned int getFrameCount() { return myFrameCount; }
private:
   bool parse ( const tstring& animationDesc );

private:
   tstring          myName;
   float            mySpeed;
   int              myCurrentFrame;
   int              myFrameDirection;
   D3DCOLOR         myExcludeColor;
   ANIMATION        myAnimation;
   Timer            myTimer;
   unsigned int     myFrameCount;
   DxAnimationFrame myFrames[ourMaxAnimationFrames];

};

#endif //_DXANIMATION_H_