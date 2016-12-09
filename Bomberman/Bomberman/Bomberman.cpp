// Bomberman.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "WinApplFramework/WinApplFramework.h"
#include "Bomberman/ScoreParser.h"
#include "Bomberman/GameLoad.h"
#include "Bomberman/GameStart.h"
#include "Bomberman/GameRun.h"
#include "Bomberman/GameLose.h"
#include "Bomberman/GameWin.h"
#include "Bomberman/GameCredits.h"
#include "Bomberman/Bomberman.h"


//=======================================================================
//
int APIENTRY _tWinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                        LPTSTR    lpCmdLine, int       nCmdShow)
{
   WinApplFramework::enableCRTHeapDebugging( true );

   //WinApplFramework::breakOnCRTAlloc( 4459 );     


   Game app;
   return app.winMain( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
}

//=======================================================================
//Game constructor
Game::Game ( )
{
}

//=======================================================================
//Game deconstructor
Game::~Game ( )
{
}

//=======================================================================
//Loads the current game state (Load, Start, Run, or Lose)
void Game::loadInterface ( tstring name, GameInterface::State state )
{
   if ( myInterfaces.find( name ) != myInterfaces.end() )
   {
      myCurrentInterfaceName = name;
      myInterfaces[myCurrentInterfaceName]->state( state );
   }
}

//=======================================================================
//Initializes Game
bool Game::gameInit ( )
{
   myGameTitle.append("BomberCat");
   winSetTitle ( myGameTitle );

   DxAssetManager::getInstance().init();
   mySoundInterface = DxSound::getInterface( DxSound::fmod );
   mySoundInterface->init( hWnd() );
   
   //sets Background Color
   bgColor = D3DCOLOR_XRGB( 0, 0, 0 );

   myKeyboard.keyboardInit( hWnd() );
   myController.init( device() );
   
   myCurrentInterfaceName = "GameLoad";
   myInterfaces["GameLoad"] = new GameLoad( myGameUI, myKeyboard, myController );
   myInterfaces["GameStart"] = new GameStart( myGameUI, myKeyboard, myController );
   myInterfaces["GameRun"] = new GameRun( myGameUI, myKeyboard, myController );
   myInterfaces["GameLose"] = new GameLose( myGameUI, myKeyboard, myController );
   myInterfaces["GameWin"] = new GameWin( myGameUI, myKeyboard, myController );
   myInterfaces["GameCredits"] = new GameCredits( myGameUI, myKeyboard, myController );

   return true;
}

//=======================================================================
//Runs game
void Game::gameRun ( )
{
   if ( myInterfaces.find( myCurrentInterfaceName ) == myInterfaces.end() )
   {
      log( "Unable to load Interface:[%s]", myCurrentInterfaceName.c_str() );
      assert(false);
      return;
   }
   GameInterface* currentInterface = myInterfaces[myCurrentInterfaceName];

   switch ( currentInterface->state() )
   {
   case GameInterface::NONE:
      assert(false);
      break;
   case GameInterface::INIT:
      if ( !currentInterface->init( this ) )
      {
         onDestroy();
      }
      break;
   case GameInterface::RUN:
      // clear the backbuffer
      device()->ColorFill( backBuffer(), NULL, bgColor );
      currentInterface->run( this );
      break;
   case GameInterface::RESET:
      currentInterface->reset( this );
      break;
   case GameInterface::SHUTDOWN:
      currentInterface->shutdown( this );
      break;
   default:
      break;
   }

   myController.update();

   //if the escape key is pressed, destroy
   if ( DxKeyboard::keyPressed( VK_ESCAPE ) || myController.buttonBackDown() )
   {
      onDestroy();
   }

}

//=======================================================================
//Shuts down Game
void Game::gameExit ( )
{
   std::map<tstring,GameInterface*>::iterator item;
   for ( item = myInterfaces.begin(); item != myInterfaces.end(); item++ )
   {
      item->second->shutdown( this );
      delete item->second;
      item->second = NULL;
   }

   myGameUI.destroy();
   myController.shutdown();

   DxAssetManager::getInstance().shutdown();
   mySoundInterface->releaseInterface();
   mySoundInterface = NULL;   
}
