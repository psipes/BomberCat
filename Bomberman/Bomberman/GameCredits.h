#pragma once

#if !defined(_GAMECREDITS_H_)
#define _GAMECREDITS_H_

#include "Utilities/Timer.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxFramework.h"
#include "DxFramework/DxGeometry.h"
#include "DxFramework/DxKeyboard.h"
#include "DxFramework/DxGameSprite.h"
#include "DxFramework/DxTexture.h"
#include "Bomberman/GameInterface.h"

#define D3DFVF_XYZ_TEXTURE (D3DFVF_XYZ|D3DFVF_TEX1)

//========================================================================================
//Todo
struct VERTEX_TEXTURE //: public DxVertex
{
   FLOAT x, y, z; // The transformed position for the vertex
   FLOAT tu, tv;        // The vertex color
};

class GameCredits : public GameInterface
{
public:

   //========================================================================================
   //
   GameCredits ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller );

   //========================================================================================
   //
   ~GameCredits ();

   //========================================================================================
   //Initializes the window
   bool init ( Game* window );

   //========================================================================================
   //Runs the window
   void run ( Game* window );

   //========================================================================================
   //Resets the window
   void reset ( Game* window );

   //========================================================================================
   //Shutdowns the window
   void shutdown ( Game* window );

private:

   //========================================================================================
   //Creates matrices
   void setupMatrices ( Game* window );

private:
   bool                       canRotate;

   float                      myHAngle;
   float                      myVAngle;

   Timer                      myTimer;

   DxGameSprite               myStartSprite;
   DxTexture*                 myTexture;
   DxGeometry<VERTEX_TEXTURE> myQuads[4];

   IDXVERTEXBUFFER            myBuffer;
};


#endif //_GAMECREDITS_H_