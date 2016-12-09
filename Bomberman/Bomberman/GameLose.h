#pragma once

#if !defined(_GAMELOSE_H_)
#define _GAMELOSE_H_

#include "DxFramework/DxFramework.h"
#include "DxFramework/DxKeyboard.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxText.h"
#include "DxFramework/DxController.h"
#include "Bomberman/GameUI.h"
#include "Bomberman/ScoreParser.h"
#include "Bomberman/Level.h"
#include "Bomberman/Controller.h"
#include "Bomberman/GameInterface.h"

class GameLose : public GameInterface
{
public:

   //========================================================================================
   //
   GameLose ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller );

   //========================================================================================
   //
   ~GameLose ();

   //========================================================================================
   //Initializes the game over window
   bool init ( Game* window );

   //========================================================================================
   //Run the game over window
   void run ( Game* window );

   //========================================================================================
   //Reset the game over window
   void reset ( Game* window );

   //========================================================================================
   //Shutdown the game over window
   void shutdown ( Game* window );

   //========================================================================================
   //Return true if playerScore equals highschore
   bool checkIfHighScore ( int playerScore );


private:

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

};

#endif //_GAMELOSE_H_