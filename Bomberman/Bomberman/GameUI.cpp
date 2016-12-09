#include "stdafx.h"
#include <cassert>
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"
#include "Bomberman/GameUI.h"

//=======================================================================
//GameUI constructor
GameUI::GameUI()
{

}

//=======================================================================
//GameUI deconstructor
GameUI::~GameUI()
{
}

//=======================================================================
//Initializes GameUI by creating sprites and DxTexts for display of time, lives, and score
bool GameUI::init( IDXFONT font, int xPos, int yPos, int time, D3DCOLOR color )
{
   myTextColor = color;
   //create clockface and myKitty face sprites
   //create score, lives, and timer text objects
   myUIHeader.create("HEADER6", 10);
   myUIHeader.setPosition(0.0f, 0.0f);
   myClockFace.setScale( 0.275f, 0.275f );
   myClockFace.create( "CLOCK", 5 );
   myClockFace.setPosition( float( xPos ), float( yPos ) );

   myKittyFace.setScale( 0.275f, 0.275f );
   myKittyFace.create( "LIVES-CAT", 10 );
   myKittyFace.setPosition( float( xPos + 500 ), float( yPos ) );

   myPowerup.setScale( 0.25f, 0.25f );
   myPowerup.create( "INVISIBLOCK" );
   myPowerup.setPosition( 0.0f, 0.0f );

   myLivesText.create( font, "9999", ( xPos + 500 + myKittyFace.getWidth() ), ( yPos + myKittyFace.getHeight() / 4 ), 50, 50, myTextColor );

   //TODO: make setScale method
   //myTimerText.setScale( 2.0f, 2.0f );

   myTimerText.create( font, "99:99", ( xPos + myClockFace.getWidth() ), ( yPos + myClockFace.getHeight() / 4 ), 50, 50, myTextColor );

   //Adjust positioning of the score text
   xPos += 300;

   myScoreAmount = 0;
   myScoreText.create( font, "0", xPos, ( yPos + myClockFace.getHeight() / 4 ), 100, 50, myTextColor );
   myStartTotalTime = time;
   myTotalTime = myStartTotalTime;  
 
   myTotalTime = 180;

   return true;
}
//=======================================================================
//Updates GameUI sprite components and calls functions for clock and life amounts to update
bool GameUI::update( Kitty& myKittyKat )
{
   if( myTimer.isRunning() == false )
   {
      myTimer.start();   
   }
   myUIHeader.update();
   myClockFace.update();
   myKittyFace.update();
   if ( myKittyKat.power() != Powerup::POWER_NONE )
   {
      myPowerup.changeAnimation( Powerup::toString( myKittyKat.power() ), 0 );
      myPowerup.update();
   }

   updateClockTimer();
   updateLivesAmountText( myKittyKat );
   
   return true;
}

//=======================================================================
//Destroys GameUI by destroying sprites and texts
bool GameUI::destroy()
{
   myUIHeader.destroy();
   myClockFace.destroy();   
   myKittyFace.destroy();
   myPowerup.destroy();

   myTimerText.destroy();
   myScoreText.destroy();
   myLivesText.destroy();

   myTextColor = NULL;

   myTimer.restart();   
   myTimer.stop();

   myTotalTime = 0;
   myStartTotalTime = 0;
   myScoreAmount = 0;
   myLivesAmount = 0;

   return true;
}

//=======================================================================
//Draws GameUI by drawing sprites for clock, lives, and score
bool GameUI::draw( IDXSPRITE spriteObj )
{
   myUIHeader.draw( spriteObj );
   myClockFace.draw( spriteObj );
   myTimerText.draw( spriteObj );

   myScoreText.draw( spriteObj );

   myKittyFace.draw( spriteObj );
   myPowerup.draw( spriteObj );
   myLivesText.draw( spriteObj );

   return true;
}

//=======================================================================
//Updates timer for use in the GameUI clock
bool GameUI::updateClockTimer()
{
   int time = ( int( myTimer.elapsedTime() ) / 1000 );

   myTimerText.setText( myTotalTime - time );

   return true;
}

//=======================================================================
//Resets timer for use in the GameUI clock
bool GameUI::resetTimer()
{
   myTotalTime = myStartTotalTime;
   myTimer.restart();
   myTimer.stop();

   return true;
}

//=======================================================================
//Adds time to the GameUI clock via powerup
void GameUI::addTime( int addTime )
{
   myTotalTime += addTime;
}

//=======================================================================
//Subtracts time from GameUI timer
void GameUI::subTime( int subTime )
{
   myTotalTime -= subTime;
}

//=======================================================================
//Adds points to the player score
void GameUI::addPoints( int addPoints )
{
   myScoreAmount += addPoints;
   myScoreText.setText( myScoreAmount );
}

//=======================================================================
//Resets player score to 0
void GameUI::resetPoints( )
{
   myScoreAmount = 0;
   myScoreText.setText( myScoreAmount );
}

//=======================================================================
//Subtracts points from player score
void GameUI::subPoints( int subPoints )
{
   myScoreAmount -= subPoints;
   myScoreText.setText( myScoreAmount );
}

//=======================================================================
//Sets score to amount when function called
void GameUI::setPoints( int newTotal )
{
   myScoreAmount = newTotal;
   myScoreText.setText( myScoreAmount );
}

//=======================================================================
//Sets text based on the the number of lives player has
void GameUI::updateLivesAmountText( Kitty& myKittyKat )
{
   myLivesAmount = myKittyKat.getLivesAmount();
   myLivesText.setText( myLivesAmount );
}





