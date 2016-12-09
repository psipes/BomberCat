#include "stdafx.h"


#include "Bomberman/Bomberman.h"
#include "Bomberman/GameRun.h"

//=======================================================================
//GameRun constructor
GameRun::GameRun ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller )
:GameInterface( gameUI, keyboard, controller ), myLevel( keyboard, controller ) //todo
{
   myState = State::INIT;
}

//=======================================================================
//GameRun deconstructor
GameRun::~GameRun ()
{
}

//=======================================================================
//Initializes game by initializing kitty, levels, and user interface (clock, lives, and score)
bool GameRun::init ( Game* window )
{
   mySoundInterface = DxSound::getInterface( DxSound::fmod );

   bool result = true;
   myBombCount = 10;
   myBombNum = 0;


   //sets Background Color
   bgColor = D3DCOLOR_XRGB( 0, 0, 0 );

   //Misc init
   //to use the assetsManager it must be initialized via the following:

   myKitty.init( 36, 100 );
   myLevel.init( myKeyboard, myController );

   //GameUI init
   myGameUI.init( window->fontInterface(), 100, 15, 180, D3DCOLOR_XRGB(255, 255, 255) );

   state( State::RUN );
   return true;
}

//=======================================================================
//Run loop for GameRun
void GameRun::run ( Game* window )
{
   // pre-render
   const int minMove = 2;

   myKitty.update();
   myLevel.update( myKitty );

   myKeyboard.keyboardUpdate();
   myController.update();
   
   if( myLevel.isLevelPic() == false )
   {
      mySoundInterface->stop( DxAssetManager::getInstance().getSound( "GAME_OVER" ) );
      
      ourController.update( myKitty, myKeyboard, myController, myLevel, myGameUI );
      myGameUI.update( myKitty );
   }

   //Win and Lose state checks
   gameOverByDeath( window );
   winGameByLevels( window );
   loseLifeByTimer( );

   // start rendering

   if ( SUCCEEDED(window->device()->BeginScene()) )
   {
      //non-sprite rendering....    
      if ( SUCCEEDED(window->spriteInterface()->Begin( D3DXSPRITE_ALPHABLEND )) )
      {
         myLevel.draw( window->spriteInterface() );

         if( myLevel.isLevelPic() == false )
         {
            myGameUI.draw( window->spriteInterface() );      
            myKitty.draw( window->spriteInterface() );
            myLevel.Collisions( myKitty, myGameUI );
         }
         
         // stop rendering
         window->spriteInterface()->End();
      }

      // End rendering:
      window->device()->EndScene();
      window->device()->Present( NULL, NULL, NULL, NULL );
   }

}

//=======================================================================
//Resets GameRun
void GameRun::reset ( Game* window )
{
   myKitty.reset();
   myKitty.resetLives();
   myLevel.resetGame();
   myLevel.restartLevel( myKitty );
   myGameUI.resetTimer();
   myLevel.stopSong();
   state( State::RUN );
}

//=======================================================================
//Shuts down GameRun
void GameRun::shutdown ( Game* window )
{
   myGameUI.destroy();
   myLevel.shutdown();
   myKitty.shutdown();
   bgColor = NULL;
   myImage.destroy();
   myGameTitle.clear();
   myBombCount = 0;
   myBombNum = 0;
   myTitleSprite.destroy();
   myBgRect = Rect();

   mySoundInterface = NULL;
}

//=======================================================================
//Loads GameLose state if game ends by player losing all lives
void GameRun::gameOverByDeath( Game* window )
{
   if( myKitty.getLivesAmount() < 0 )
   {
      reset( window );

      myLevel.stopSong();
      window->loadInterface( "GameLose", State::INIT );
   }
}

//=======================================================================
//Restarts level and decrease player's lives if timer makes it to 0
void GameRun::loseLifeByTimer( )
{
   if ( myGameUI.getTime() < 0 )
   {
      myLevel.restartLevel( myKitty );
      myKitty.loseLife();
      myGameUI.resetTimer();
      //mySoundInterface->play( DxAssetManager::getInstance().getSound( "GAME_OVER" ) );
   }
}

//=======================================================================
//Loads GameWin state if player beats all levels
void GameRun::winGameByLevels( Game* window )
{
   if( myLevel.isGameWon() )
   {
      myLevel.stopSong();
      reset( window );
      window->loadInterface( "GameWin", State::INIT );
   }
}