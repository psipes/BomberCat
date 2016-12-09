#include "stdafx.h"

#include "Bomberman/Bomberman.h"
#include "Bomberman/GameLoad.h"


//=======================================================================
//GameLoad constructor
GameLoad::GameLoad ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller )
:GameInterface( gameUI, keyboard, controller )
{
   myState = State::INIT;
   myLoadingIndex  = 0;
}

//=======================================================================
//GameLoad deconstructor
GameLoad::~GameLoad()
{
}

//=======================================================================
//Initializes GameLoad by creating the "Loading..." sprite
bool GameLoad::init ( Game* window )
{
   DxAssetManager::getInstance().load( "Load.config" );
   
   myTitleSprite.create( "LOADING", 10 );
   myTitleSprite.setPosition(100.0f, 250.0f);

   state( State::RUN );
   return true;
}

//=======================================================================
//Run loop for the GameLoad screen
void GameLoad::run( Game* window )
{
   myTitleSprite.update();

   if ( SUCCEEDED(window->device()->BeginScene()) )
   {
      //non-sprite rendering....    
      if ( SUCCEEDED(window->spriteInterface()->Begin( D3DXSPRITE_ALPHABLEND )) )
      {
         myTitleSprite.draw( window->spriteInterface() );
         
         // stop rendering
         window->spriteInterface()->End();
      }

      // End rendering:
      window->device()->EndScene();
      window->device()->Present( NULL, NULL, NULL, NULL );
   }

   //todo - sas - cast away sign/unsign mismatch - verify
   if ( myTitleSprite.getAnimation().getCurrentFrame() > (int(myTitleSprite.getAnimation().getFrameCount()) - 3) )
   {
      switch ( myLoadingIndex )
      {
      case 0:
         DxAssetManager::getInstance().load( "Kitty.config" );
         break;
      case 1:
         DxAssetManager::getInstance().load( "Bomb.config" );
         break;
      case 2:
         DxAssetManager::getInstance().load( "Enemy.config" );
         break;
      case 3:
         DxAssetManager::getInstance().load( "LevelScreen.config" );
         break;
      case 4:
         DxAssetManager::getInstance().load( "Pickup.config" );
         break;
      case 5:
         DxAssetManager::getInstance().load( "Tile.config" );
         break;
      case 6:
         DxAssetManager::getInstance().load( "UI.config" );
         break;
      case 7:
         DxAssetManager::getInstance().load( "sounds.config" );
         break;
      case 8:
         DxAssetManager::getInstance().load( "Levels.config" );
         break;
      }
      myLoadingIndex++;

   }
   else if ( myLoadingIndex > 0 )
   {
      myTitleSprite.getAnimation().setFrame( myTitleSprite.getAnimation().getFrameCount() - 3 );
   }
   if ( myLoadingIndex >= 8 )
   {
      window->loadInterface( "GameStart", State::INIT );
   }
   
}

//=======================================================================
//Resets GameLoad
void GameLoad::reset ( Game* window )
{
}

//=======================================================================
//Shuts down GameLoad
void GameLoad::shutdown ( Game* window )
{
   myTitleSprite.destroy();
   myLoadingIndex = 0;
}
