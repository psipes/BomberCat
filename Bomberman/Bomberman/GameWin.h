#pragma once

#if !defined(_GAMEWIN_H_)
#define _GAMEWIN_H_

#include "DxFramework/DxFramework.h"
#include "DxFramework/DxKeyboard.h"
#include "DxFramework/DxController.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxText.h"
#include "Bomberman/GameUI.h"
#include "Bomberman/ScoreParser.h"
#include "Bomberman/Level.h"
#include "Bomberman/Controller.h"
#include "Bomberman/GameInterface.h"

class GameWin : public GameInterface
{
public:

   //========================================================================================
   //
   GameWin ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller );

   //========================================================================================
   //
   ~GameWin ();

   //========================================================================================
   //Initializes the game win window
   bool init ( Game* window );

   //========================================================================================
   //Runs the game win window
   void run ( Game* window );

   //========================================================================================
   //Resets the game win window
   void reset ( Game* window );

   //========================================================================================
   //Shutdowns the game win window
   void shutdown ( Game* window );

   //========================================================================================
   //Return true if playerScore is the new high score
   bool checkIfHighScore ( int playerScore );


private:
   char              myInitials[4];

   int               myPlayerScore;
   int               myIndex;
   int               myHighScoreIndex;

   bool              myControllerNotPressed;
   bool              myTextEntered;
   bool              myInHighScoreList;

   tstring           myString;
   tstring           myEnterHiScoreString1;
   tstring           myEnterHiScoreString2;
   tstring           myEnterHiScoreString3;



   DxGameSprite      myBGSprite;
   DxGameSprite      myHSSprite;
   DxGameSprite      myStartSprite;

   DxText            myNameText[5];
   DxText            myScoreText[5];
   DxText            myEnterHiScoreText1;
   DxText            myEnterHiScoreText2;
   DxText            myEnterHiScoreText3;
   DxText            myEnterCursor;

   ScoreParser       myScore;

};

#endif //_GAMEWIN_H_