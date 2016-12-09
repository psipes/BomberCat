#include "stdafx.h"
#include "Bomberman/Bomberman.h"
#include "Bomberman/GameStart.h"

//=======================================================================
//GameStart constructor
GameStart::GameStart ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller )
:GameInterface( gameUI, keyboard, controller )
{
   myState = State::INIT;
   myController = controller;
}

//=======================================================================
//GameStart deconstructor
GameStart::~GameStart ()
{
}

//=======================================================================
//Initializes GameStart sprites and sound
bool GameStart::init ( Game* window )
{
   myLogoSprite.create( "LOGO", 2.5f );
   myLogoSprite.setPosition( 225.0f, 100.0f);
   myLogoSprite.setScale( 0.50F, 0.50f );
   
   myTitleSprite.create( "TITLE" );
   myTitleSprite.setPosition( 200.0f, 50.0f);
   myTitleSprite.setScale( 0.50F, 0.50F);

   myStartSprite.create( "START", 1.8f );
   myStartSprite.setPosition( 125.0f, 500.0f);
   myStartSprite.setScale( 0.50F, 0.50F);

   DxSound::getInterface( DxSound::fmod )->play( DxAssetManager::getInstance().getSound( "HAMSTERDANCE" ) );

   state( State::RUN );
   return true;
}

//=======================================================================
//Run loop for the GameStart screen
void GameStart::run ( Game* window )
{
   // start rendering
   myLogoSprite.update();
   myStartSprite.update();

   myController.update();

   if ( SUCCEEDED(window->device()->BeginScene()) )
   {
      //non-sprite rendering....    
      if ( SUCCEEDED(window->spriteInterface()->Begin( D3DXSPRITE_ALPHABLEND )) )
      {
         
         myLogoSprite.draw( window->spriteInterface() );
         myTitleSprite.draw( window->spriteInterface() );
         myStartSprite.draw( window->spriteInterface() );

         // stop rendering
         window->spriteInterface()->End();
      }

      // End rendering:
      window->device()->EndScene();
      window->device()->Present( NULL, NULL, NULL, NULL );
   }

   if( myKeyboard.keyPressed( VK_RETURN ) || myController.buttonStartDown() )
   {
      DxSound::getInterface( DxSound::fmod )->stop( DxAssetManager::getInstance().getSound( "HAMSTERDANCE" ) );
      window->loadInterface( "GameRun", State::INIT );
   }

}

//=======================================================================
//Resets GameStart
void GameStart::reset ( Game* window )
{
   shutdown( window );
   init( window );
}

//=======================================================================
//Shuts down GameStart
void GameStart::shutdown ( Game* window )
{
   myLogoSprite.destroy();
   myTitleSprite.destroy();
   myStartSprite.destroy();
}
