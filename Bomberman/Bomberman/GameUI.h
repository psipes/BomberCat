#pragma once

#if !defined(_GAMEUI_H_)
#define _GAMEUI_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "Utilities/NonCopyable.h"
#include "Utilities/TTypes.h"
#include "Utilities/Rect.h"
#include "Utilities/Point.h"
#include "DxFramework/DxTexture.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxGameSprite.h"
#include "DxFramework/DxText.h"
#include "Utilities/Timer.h"
#include "DxFramework/DxAssetManager.h"
#include "Bomberman/Kitty.h"


class GameUI
{
public:

   //========================================================================================
   //
	GameUI ( void );

   //========================================================================================
   //
	~GameUI ( void );

public:

   //========================================================================================
   //Initializes the user interface
   bool init( IDXFONT font, int xPos, int yPos, int time, D3DCOLOR color );

   //========================================================================================
   //Updates teh user interface
   bool update( Kitty& myKittyKat );

   //========================================================================================
   //Destroyes the user interface
   bool destroy();

   //========================================================================================
   //Draws the user interface
   bool draw( IDXSPRITE spriteObj );

   //========================================================================================
   //timer functions
   //Updates the timer
   bool updateClockTimer();

   //========================================================================================
   //Resets the timer
   bool resetTimer();

   //========================================================================================
   //Add more time to the timer
   void addTime( int addTime );

   //========================================================================================
   //Subtract more time from the timer
   void subTime( int subTime );

   //========================================================================================
   //Returns the amount of time left on the timer
   inline int getTime () { return myTotalTime - ( (int)( myTimer.elapsedTime() ) / 1000 ); }

   //========================================================================================
   //points functions
   //Add points to the score
   void addPoints( int addPoints );

   //========================================================================================
   //Subtract points from the score
   void subPoints( int subPoints );

   //========================================================================================
   //Reset the amount of points
   void resetPoints();

   //========================================================================================
   //Set the amount of points
   void setPoints( int newTotal ); 

   //========================================================================================
   //Return the score
   inline int getPoints () { return myScoreAmount; }

   //========================================================================================
   //Update the number of lives
   void updateLivesAmountText( Kitty& myKittyKat );

private:
   int              myTotalTime;
   int              myStartTotalTime;
   int              myScoreAmount;
   int              myLivesAmount;

   DxGameSprite     myKittyFace;
   DxGameSprite     myPowerup;
   DxGameSprite     myUIHeader;
   DxGameSprite     myClockFace;

   DxText           myTimerText;
   DxText           myScoreText;
   DxText           myLivesText;

   D3DCOLOR         myTextColor;
   
   Timer            myTimer;

};

#endif 