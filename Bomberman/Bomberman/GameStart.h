#pragma once

#if !defined(_GAMESTART_H_)
#define _GAMESTART_H_

#include "DxFramework/DxFramework.h"
#include "DxFramework/DxKeyboard.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxText.h"
#include "DxFramework/DxController.h"
#include "Bomberman/Level.h"
#include "Bomberman/Controller.h"
#include "Bomberman/GameInterface.h"

class GameStart : public GameInterface
{
public:

   //========================================================================================
   //
   GameStart ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller );

   //========================================================================================
   //
   ~GameStart ();

   //========================================================================================
   //Initializes the game start window
   bool init ( Game* window );

   //========================================================================================
   //Runs the game start window
   void run ( Game* window );

   //========================================================================================
   //Resets the game start window
   void reset ( Game* window );

   //========================================================================================
   //Shutdowns the game start window
   void shutdown ( Game* window );

private:
   DxGameSprite      myLogoSprite;
   DxGameSprite      myTitleSprite;
   DxGameSprite      myStartSprite;
  
};

#endif //_GAMELOSE_H_