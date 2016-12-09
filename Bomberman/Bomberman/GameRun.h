#pragma once

#if !defined(_GAMERUN_H_)
#define _GAMERUN_H_

#include "DxFramework/DxFramework.h"
#include "DxFramework/DxKeyboard.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxText.h"
#include "Bomberman/GameUI.h"
#include "Bomberman/Level.h"
#include "Bomberman/Controller.h"
#include "Bomberman/GameInterface.h"

class GameRun : public GameInterface
{
public:

   //========================================================================================
   //
   GameRun ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller );

   //========================================================================================
   //
   ~GameRun ();

   //========================================================================================
   //Initializes the game run window
   bool init ( Game* window );

   //========================================================================================
   //Runs the game run window
   void run ( Game* window );

   //========================================================================================
   //Resets the game run window
   void reset ( Game* window );

   //========================================================================================
   //Shutdowns the game run window
   void shutdown ( Game* window );

private:

   //========================================================================================
   //Checks to see if the game ended by dying
   void gameOverByDeath ( Game* window );

   //========================================================================================
   //Checks to see if you lost lives through timer
   void loseLifeByTimer ();

   //========================================================================================
   //Checks to see if you won the game by getting through the levels
   void winGameByLevels ( Game* window );

private:
   int               myBombCount;
   int               myBombNum;

   D3DCOLOR          bgColor;

   DxTexture         myImage;	

   tstring           myGameTitle;

   Kitty             myKitty;

   RECT              myBgRect;

   Controller        ourController;

   DxGameSprite      myTitleSprite;

   DxSound*          mySoundInterface;
   DxSoundIdentifier* mySound;

   Level             myLevel;
};

#endif //_GAMERUN_H_