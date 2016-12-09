#include "stdafx.h"


#include "Bomberman/Bomberman.h"
#include "Bomberman/GameLose.h"

//=======================================================================
//GameLose constructor, requires GameUI to obtain player final score
GameLose::GameLose ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller )
:GameInterface( gameUI, keyboard, controller )
{
   myState = State::INIT;
   myController = controller;
}

//=======================================================================
//GameLose deconstructor
GameLose::~GameLose ()
{
}

//=======================================================================
//Initializes GameLose screen by creating sprites for animation and text to display high scores,
//also checks if player's score makes it in the top 5 scores and creates text for input of player initials
bool GameLose::init ( Game* window )
{
   myBGSprite.create( "GAMELOSE", 3 );
   myBGSprite.setPosition( 0.0f, 0.0f);
   myBGSprite.setScale( 0.75F, 0.75F );
   
   myHSSprite.create( "HIGHSCORE" );
   myHSSprite.setPosition( 225.0f, 40.0f);
   myHSSprite.setScale( 0.50F, 0.50F);

   myStartSprite.create( "START", 1.8f );
   myStartSprite.setPosition( 275.0f, 500.0f);
   myStartSprite.setScale( 0.25F, 0.25F);

   DxSound::getInterface( DxSound::fmod )->play( DxAssetManager::getInstance().getSound( "GAME_OVER" ) );
   int yPos = 128;
   int XPos = 340;

   myPlayerScore = myGameUI.getPoints();
   myHighScoreIndex = 5;
   myInHighScoreList = false;
   myString = "AAA";

   myScore.readScore();
   myInHighScoreList = checkIfHighScore( myPlayerScore );

   strcpy_s( myInitials, "AAA" );
   myIndex = 0;
   myTextEntered = false;
   myEnterHiScoreString1 = "Enter your initials";
   myEnterHiScoreString2 = "Use arrow keys to change letters";
   myEnterHiScoreString3 = "and SPACE to set";

   myControllerNotPressed = false;

   for( int i = 0; i < SIZEOFARRAY(myNameText); i++)
   {
      if ( myInHighScoreList == true )
      {
         if ( i < myHighScoreIndex )
         {
            myNameText[i].create( window->fontInterface(), myScore.getScores( i ).name, XPos, yPos, 45, 50, D3DCOLOR_XRGB(255, 255, 255)  );
            myScoreText[i].create( window->fontInterface(), myScore.getScores( i ).score , XPos + 51, yPos, 45, 50, D3DCOLOR_XRGB(255, 255, 255)  );
            yPos = yPos + 32;
         }
         else if ( i == myHighScoreIndex ) 
         {
            myNameText[i].create( window->fontInterface(), myString, XPos, yPos, 45, 50, D3DCOLOR_XRGB(255, 255, 255)  );
            myScoreText[i].create( window->fontInterface(), myPlayerScore , XPos + 51, yPos, 45, 50, D3DCOLOR_XRGB(255, 255, 255)  );
            yPos = yPos + 32;
         }
         else
         {
            myNameText[i].create( window->fontInterface(), myScore.getScores( i -1 ).name, XPos, yPos, 45, 50, D3DCOLOR_XRGB(255, 255, 255)  );
            myScoreText[i].create( window->fontInterface(), myScore.getScores( i - 1 ).score , XPos + 51, yPos, 45, 50, D3DCOLOR_XRGB(255, 255, 255)  );
            yPos = yPos + 32;                       
         }
      }
      else
      {
         myNameText[i].create( window->fontInterface(), myScore.getScores( i ).name, XPos, yPos, 45, 50, D3DCOLOR_XRGB(255, 255, 255)  );
         myScoreText[i].create( window->fontInterface(), myScore.getScores( i ).score , XPos + 51, yPos, 45, 50, D3DCOLOR_XRGB(255, 255, 255)  );
         yPos = yPos + 32;
      }
   }
   myEnterHiScoreText1.create( window->fontInterface(), myEnterHiScoreString1, XPos, yPos, 225, 50, D3DCOLOR_XRGB(255, 255, 255)  );
   myEnterHiScoreText2.create( window->fontInterface(), myEnterHiScoreString2, XPos, yPos+32, 250, 50, D3DCOLOR_XRGB(255, 255, 255)  );
   myEnterHiScoreText3.create( window->fontInterface(), myEnterHiScoreString3, XPos, yPos+64, 225, 50, D3DCOLOR_XRGB(255, 255, 255)  );

   myEnterCursor.create( window->fontInterface(), _T("_"), (int)myNameText[myHighScoreIndex].getXPosition() - 20, (int)myNameText[myHighScoreIndex].getYPosition(), 45, 50, D3DCOLOR_XRGB( 255, 255, 255 ) );


   state( State::RUN );
   return true;
}

//=======================================================================
//Run loop for GameLose screen, uses keyboard for input of initials if player makes it into top 5 scores
void GameLose::run ( Game* window )
{
   // enter initials stuff
   if ( !myTextEntered && myInHighScoreList )
   {
      if ( DxKeyboard::keyDown( VK_UP ) || ( myController.dPadUpDown() && myControllerNotPressed ) )
      {
         if ( myInitials[myIndex] == 'A' )
         {
            myInitials[myIndex] = 'Z';
         }
         else
         {
            myInitials[myIndex]--;
         }
         myString = myInitials;
         myNameText[myHighScoreIndex].setText( myString );

         myControllerNotPressed = false;
        
      }
      if ( DxKeyboard::keyDown( VK_DOWN ) || ( myController.dPadDownDown() && myControllerNotPressed ) )
      {
         if ( myInitials[myIndex] == 'Z' )
         {
            myInitials[myIndex] = 'A';
         }
         else
         {
            myInitials[myIndex]++;
         }
         myString = myInitials;
         myNameText[myHighScoreIndex].setText( myString );

         myControllerNotPressed = false;

      }
      if ( DxKeyboard::keyDown( VK_LEFT ) || ( myController.dPadLeftDown() && myControllerNotPressed ) )
      {
         if ( myIndex > 0 )
         {
            myIndex--;
            myEnterCursor.setXPosition( myEnterCursor.getXPosition() - 10 );
         }

         myControllerNotPressed = false;
      }
      if ( DxKeyboard::keyDown( VK_RIGHT ) || ( myController.dPadRightDown() && myControllerNotPressed ) )
      {
         if ( myIndex < 2 )
         {
            myIndex++;
            myEnterCursor.setXPosition( myEnterCursor.getXPosition() + 10 );
         }

         myControllerNotPressed = false;
      }
      if ( DxKeyboard::keyDown( VK_SPACE ) || myController.buttonADown() )
      {
         myTextEntered = true;
         myScore.writeScore( myPlayerScore, myString);
      }
   }
   else
   {
      myTextEntered = true;
   }

   if ( !myController.dPadDownDown() && !myController.dPadUpDown() && !myController.dPadRightDown() && !myController.dPadLeftDown() )
   {
      myControllerNotPressed = true;
   }


   myBGSprite.update();
   myStartSprite.update();
   myController.update();
   
   // start rendering

   if ( SUCCEEDED(window->device()->BeginScene()) )
   {
      //non-sprite rendering....    
      if ( SUCCEEDED(window->spriteInterface()->Begin( D3DXSPRITE_ALPHABLEND )) )
      {
         for( int i = 0; i < SIZEOFARRAY(myNameText); i++)
         {
            myNameText[i].draw( window->spriteInterface() );
            myScoreText[i].draw( window->spriteInterface() );
         }
         if ( !myTextEntered && myInHighScoreList )
         {
            myEnterHiScoreText1.draw( window->spriteInterface() );
            myEnterHiScoreText2.draw( window->spriteInterface() );
            myEnterHiScoreText3.draw( window->spriteInterface() );
            myEnterCursor.draw( window->spriteInterface() );
         }
         else if ( myTextEntered )
         {
            myStartSprite.draw( window->spriteInterface() );
         }

         
         myBGSprite.draw( window->spriteInterface() );
         myHSSprite.draw( window->spriteInterface() );

         // stop rendering
         window->spriteInterface()->End();
      }

      // End rendering:
      window->device()->EndScene();
      window->device()->Present( NULL, NULL, NULL, NULL );
   }

   if( ( myKeyboard.keyPressed( VK_RETURN ) || myController.buttonStartDown() ) && myTextEntered )
   {
      DxSound::getInterface( DxSound::fmod )->stop( DxAssetManager::getInstance().getSound( "GAME_OVER" ) );
      this->shutdown( window );
      window->loadInterface( "GameCredits", State::INIT );
   }

}

//=======================================================================
//Resets GameLose
void GameLose::reset ( Game* window )
{
   shutdown( window );
   init( window );
}

//=======================================================================
//Shuts down GameLose by destroying text and sprites
void GameLose::shutdown ( Game* window )
{
   myBGSprite.destroy();
   myHSSprite.destroy();

   myController.shutdown();
   myStartSprite.destroy();

   for(int i = 0; i < SIZEOFARRAY(myNameText); i++)
   {
      myNameText[i].destroy();
   }

   for ( size_t i = 0; i < SIZEOFARRAY(myScoreText); i++ )
   {
      myScoreText[i].destroy();
   }
   
   myEnterHiScoreText1.destroy();
   myEnterHiScoreText2.destroy();
   myEnterHiScoreText3.destroy();

   myEnterCursor.destroy();
   myScore.destroy();
   myGameUI.destroy();

   for(int i = 0; i < SIZEOFARRAY( myInitials ); i++)
   {
      myInitials[i] = NULL;
   }

   myPlayerScore = 0;
   myIndex = 0;
   myHighScoreIndex = 0;
   myTextEntered = false;
   myInHighScoreList = false;;
   myString.clear();
   myEnterHiScoreString1.clear();
   myEnterHiScoreString2.clear();
   myEnterHiScoreString3.clear();
   myControllerNotPressed = false;
}

//=======================================================================
//Boolean check to see if player makes it into top 5 scores, and establishes index in the 
//vector at which they will enter their initials
bool GameLose::checkIfHighScore ( int playerScore )
{
    //check if in top 5 scores
   if ( playerScore >= myScore.getScores( 4 ).score )
   {
      myInHighScoreList = true;
      for ( int j = 4; j >= 0; j-- )
      {
         if ( playerScore >= myScore.getScores( j ).score )
         {
            myHighScoreIndex--;
         }
      }
   }
   return myInHighScoreList;
}