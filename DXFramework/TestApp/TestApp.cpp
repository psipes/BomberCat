// TestApp.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TestApp.h"
#include "Utilities/Point.h"
#include "Utilities/Rect.h"
#include "DxFramework/DxAssetManager.h"
#include "DxFramework/DxKeyboard.h"

//=======================================================================
int APIENTRY _tWinMain ( HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                        LPTSTR    lpCmdLine, int       nCmdShow)
{
   Game app;
   return app.winMain ( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
}

//=======================================================================
Game::Game ( )
//://myTexture(NULL)
{
}

//=======================================================================
Game::~Game ( )
{
}

//=======================================================================
bool Game::gameInit ( )
{
   DxAssetManager::getInstance().init( "config.txt" );
   DxAssetManager::getInstance().parseConfig( "animations.txt");
   
   bgColor = D3DCOLOR_XRGB( 0, 0, 100 );

   bgColor2 = D3DCOLOR_XRGB( 0, 0, 0 );

   myTexture.create( device(), DxAssetManager::getInstance().getConfigAssetPath("spritesheet.png"), Rect(0, 0, 100, 100) );//DxAssetManager::getInstance().getTexture("GHOST");


   return myTexture != NULL;
}

//=======================================================================
void Game::gameRun ( )
{
   // pre-render

   // clear the backbuffer
   device()->ColorFill( backBuffer(), NULL, bgColor );

   // start rendering
   if ( SUCCEEDED(device()->BeginScene()) && SUCCEEDED(spriteInterface()->Begin( D3DXSPRITE_ALPHABLEND )) )
   {

      //render
	   /*if(myKeyboard.keyDown(VK_SPACE))
	   {

		 bgColor = D3DCOLOR_XRGB( rand()% 255 , rand()% 255, rand()% 255);
	   }*/
      //myAnimation.drawFrame( spriteInterface(), NULL, &D3DXVECTOR2(5,5), 0, NULL, D3DCOLOR_ARGB( 255,255,255,255 ) );
      //myAnimation.update();
      myTexture.draw( spriteInterface(), 0, 0 );//&Rect(0,0,100,100), &Rect(0,0,50,50), 1.0f );
	   if(myKeyboard.keyPressed(VK_SPACE))
	   {
	      bgColor = D3DCOLOR_XRGB( rand()% 255 , rand()% 255, rand()% 255);
	   }
	   
      // stop rendering
	   spriteInterface()->End();
      device()->EndScene();

      //switch screen to the next backbuffer
      device()->Present( NULL, NULL, NULL, NULL );
   }
}

//=======================================================================
void Game::gameExit ( )
{
}