#include "stdafx.h"


#include "DxFramework/DxAssetManager.h"
#include "Bomberman/Bomberman.h"
#include "Bomberman/GameCredits.h"

//=======================================================================
//Establish matrices for 3d cube
VERTEX_TEXTURE cubeFaceFront[] =
{
   { -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, },
   { -1.0f,-1.0f, -1.0f, 0.0f, 1.0f, },
   {  1.0f, 1.0f, -1.0f, 0.25f, 0.0f, },
   {  1.0f,-1.0f, -1.0f, 0.25f, 1.0f, },
};

VERTEX_TEXTURE cubeFaceBack[] =
{
   { -1.0f,-1.0f, 1.0f, 0.25f, 0.0f, },
   { -1.0f, 1.0f, 1.0f, 0.25f, 1.0f, },
   {  1.0f,-1.0f, 1.0f, 0.50f, 0.0f, },
   {  1.0f, 1.0f, 1.0f, 0.50f, 1.0f, },
};

VERTEX_TEXTURE cubeFaceTop[] =
{
   { -1.0f, 1.0f,  1.0f, 0.50f, 0.0f, },
   { -1.0f, 1.0f, -1.0f, 0.50f, 1.0f, },
   {  1.0f, 1.0f,  1.0f, 0.75f, 0.0f, },
   {  1.0f, 1.0f, -1.0f, 0.75f, 1.0f, },
};

VERTEX_TEXTURE cubeFaceBottom[] =
{
   { -1.0f, -1.0f, -1.0f, 0.75f, 0.0f, },
   { -1.0f, -1.0f,  1.0f, 0.75f, 1.0f, },
   {  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, },
   {  1.0f, -1.0f,  1.0f, 1.0f, 1.0f, },
};

//=======================================================================
//GameCredits constructor
GameCredits::GameCredits ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller )
:GameInterface( gameUI, keyboard, controller )
{
   myState = State::INIT;
   myBuffer = NULL;
   myHAngle = myVAngle = 0;
   canRotate = false;
}

//=======================================================================
//GameCredits deconstructor
GameCredits::~GameCredits ()
{
}

//=======================================================================
//Initializes GameCredits along with textures to be used on the cube
bool GameCredits::init ( Game* window )
{
   myTexture = DxAssetManager::getInstance().getTexture( "CUBE" );
   
   myQuads[0].create( window->device(), &myBuffer, cubeFaceFront, SIZEOFARRAY(cubeFaceFront), D3DFVF_XYZ_TEXTURE );
   myQuads[1].create( window->device(), &myBuffer, cubeFaceBack, SIZEOFARRAY(cubeFaceBack), D3DFVF_XYZ_TEXTURE );
   myQuads[2].create( window->device(), &myBuffer, cubeFaceTop, SIZEOFARRAY(cubeFaceTop), D3DFVF_XYZ_TEXTURE );
   myQuads[3].create( window->device(), &myBuffer, cubeFaceBottom, SIZEOFARRAY(cubeFaceBottom), D3DFVF_XYZ_TEXTURE );

   window->device()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );

   // Turn off culling, so we see the front and back of the triangle
   window->device()->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

   // Turn off D3D lighting, since we are providing our own vertex colors
   window->device()->SetRenderState( D3DRS_LIGHTING, FALSE );

   myStartSprite.create( "START", 1.8f );
   myStartSprite.setPosition( 275.0f, 550.0f);
   myStartSprite.setScale( 0.25F, 0.25F);

   myHAngle = myVAngle = 0;
   canRotate = false;

   myTimer.start();
   state( State::RUN );
   return (myTexture != NULL);
}

//=======================================================================
//Run loop for GameCredits
void GameCredits::run ( Game* window )
{
   window->device()->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );
   
   setupMatrices( window );
   
   if ( SUCCEEDED(window->device()->BeginScene()) )
   {

      //non-sprite rendering....
      for ( int i = 0; i < SIZEOFARRAY( myQuads ); i++ )
         myQuads[i].draw( window->device(), D3DPT_TRIANGLESTRIP, 0, *myTexture );

      if ( SUCCEEDED(window->spriteInterface()->Begin( D3DXSPRITE_ALPHABLEND )) )
      {
         myStartSprite.draw( window->spriteInterface() );
         window->spriteInterface()->End();
      }

      window->device()->EndScene();
      window->device()->Present( NULL, NULL, NULL, NULL );
   }
   if ( DxKeyboard::keyDown( VK_RETURN ) || myController.buttonStartDown() )
   {
      window->loadInterface( "GameStart", State::RUN );
      myTimer.stop();
   }
}

//=======================================================================
//Resets GameCredits
void GameCredits::reset ( Game* window )
{

}

//=======================================================================
//Shuts down GameCredits
void GameCredits::shutdown ( Game* window )
{
   myTexture = NULL;
   for ( int i = 0; i < SIZEOFARRAY( myQuads ); i++ )
      myQuads[i].destroy();
   
   myStartSprite.destroy();
   IfRelease(&myBuffer); 

   canRotate = false;
   myHAngle = 0.0f;
   myVAngle = 0.0f;
   myStartSprite.destroy();

   for(int i = 0; i < SIZEOFARRAY( myQuads ); i++)
   {
      myQuads[4];
   }

   myTimer.restart();
   myTimer.stop();
}


//=======================================================================
//Establishs view, projection, and world for GameCredits
void GameCredits::setupMatrices( Game* window )
{
   static float lastRot = 0;
   // For our world matrix, we will just rotate the object about the y-axis.
   D3DXMATRIXA16 matWorld;

   // Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
   // every 1000 ms. To avoid the loss of precision inherent in very high 
   // floating point numbers, the system time is modulated by the rotation 
   // period before conversion to a radian angle.
   if ( canRotate )
   {
      myVAngle += ( 2.0f * D3DX_PI ) / 1000.0f;
   }
   
   D3DXMatrixRotationYawPitchRoll( &matWorld, 0, myVAngle, 0 );
   window->device()->SetTransform( D3DTS_WORLD, &matWorld );

   if ( (canRotate && myVAngle > lastRot + D3DXToRadian(90)) || 
        (!canRotate && myTimer.elapsedTime() >= 5000) )
   {
      lastRot = myVAngle;
      myTimer.restart();
      canRotate = !canRotate;
   }

   //statefull transformation
   //matWorld = matScale * matRot * matTrans * matOrbit;

   //matWorld = identity;
   //fwSetTranslation( &matWorld, x,y,z );        // mw * *D3DXMatrixTranslation( &tm, x,y,z );
   //fwSetRotation( &matWorld, rotAngle );
   //fwSetScaling( &matWorld, 2.0f );
   //fwSetRotation( &matWorld, orbitAngle );


   // Set up our view matrix. A view matrix can be defined given an eye point,
   // a point to lookat, and a direction for which way is up. Here, we set the
   // eye five units back along the z-axis and up three units, look at the
   // origin, and define "up" to be in the y-direction.
   D3DXVECTOR3 vEyePt( 0.0f, -0.15f,-4.0f );
   D3DXVECTOR3 vLookatPt( 0.0f, -0.15f, 0.0f );
   D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
   D3DXMATRIXA16 matView;
   D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
   window->device()->SetTransform( D3DTS_VIEW, &matView );

   // For the projection matrix, we set up a perspective transform (which
   // transforms geometry from 3D view space to 2D viewport space, with
   // a perspective divide making objects smaller in the distance). To build
   // a perpsective transform, we need the field of view (1/4 pi is common),
   // the aspect ratio, and the near and far clipping planes (which define at
   // what distances geometry should be no longer be rendered).
   D3DXMATRIXA16 matProj;
   D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f );
   window->device()->SetTransform( D3DTS_PROJECTION, &matProj );
}
