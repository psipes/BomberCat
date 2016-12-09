#pragma once

#if !defined(_GAMELOAD_H_)
#define _GAMELOAD_H_

#include "DxFramework/DxFramework.h"
#include "DxFramework/DxKeyboard.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxText.h"
#include "Bomberman/GameUI.h"
#include "Bomberman/Level.h"
#include "Bomberman/Controller.h"
#include "Bomberman/GameInterface.h"

class GameLoad : public GameInterface
{
public:

   //========================================================================================
   //
   GameLoad ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller );

   //========================================================================================
   //
   ~GameLoad ();

   //========================================================================================
   //Initializes the load window
   bool init ( Game* window );

   //========================================================================================
   //Runs the load window
   void run ( Game* window );

   //========================================================================================
   //Resets the load window
   void reset ( Game* window );

   //========================================================================================
   //Shutdowns the load window
   void shutdown ( Game* window );

private:
   int               myLoadingIndex;

   DxGameSprite      myTitleSprite;

};

#endif //_GAMELOAD_H_