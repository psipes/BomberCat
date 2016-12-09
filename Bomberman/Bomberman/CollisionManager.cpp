#include "stdafx.h"
#include "Bomberman/CollisionManager.h"

//========================================================================================
//CollisionManager constructor
CollisionManager::CollisionManager(){}
//========================================================================================
//CollisionManager deconstructor
CollisionManager::~CollisionManager(){}
//========================================================================================
//Initializes CollisionManager
void CollisionManager::init(){}
//========================================================================================
//Updates CollisionManager
void CollisionManager::update(){}
//========================================================================================
//Shuts down CollisionManager 
void CollisionManager::shutdown(){}
//========================================================================================
//Checks sprite with world items (bricks, grass, blocks) for collision
bool CollisionManager::worldCollisions( DxGameSprite& sprite, TiledBackground&  LevelRef, int* index )
{
   Rect collision = sprite.getCollisionArea();
   int indexTopLeft = ( (int)collision.left / 32 ) + 
                      ( (int)collision.top / 32 ) *  LevelRef.numColumns();

   int indexBottomLeft = ( (int)collision.left / 32 ) + 
                         ( (int)collision.bottom / 32 ) *
                         LevelRef.numColumns();

   int indexTopRight = ( (int)collision.right / 32 ) + 
                       ( (int)collision.top / 32 ) * LevelRef.numColumns();

   int indexBottomRight = ( (int)collision.right / 32 ) +
                          ( (int)collision.bottom / 32 ) * 
                          LevelRef.numColumns();

   if( ( sprite.collidesWith( LevelRef.mySpriteMap[indexTopLeft].tileSprite )&&  
         LevelRef.mySpriteMap[indexTopLeft].tileSprite.collidable() ) )
   {
      if ( index )
      {
         *index = indexTopLeft;
      }
      return true;
   }
   else if ( ( sprite.collidesWith( LevelRef.mySpriteMap[indexBottomLeft].tileSprite ) &&  
              LevelRef.mySpriteMap[indexBottomLeft].tileSprite.collidable() ) )
   {
      if ( index )
      {
         *index = indexBottomLeft;
      }
      return true;
   }
   else if( ( sprite.collidesWith( LevelRef.mySpriteMap[indexTopRight].tileSprite ) &&  
            LevelRef.mySpriteMap[indexTopRight].tileSprite.collidable() ) )
   {
      if ( index )
      {
         *index = indexTopRight;
      }
      return true;
   }
   else if( ( sprite.collidesWith( LevelRef.mySpriteMap[indexBottomRight].tileSprite ) &&  
             LevelRef.mySpriteMap[indexBottomRight].tileSprite.collidable() ) )
   {
      if ( index )
      {
         *index = indexBottomRight;
      }
      return true;
   }

   return false;
}

//========================================================================================
//Checks if sprite has collided with door for the level
bool CollisionManager::doorCollisions( DxGameSprite& sprite, TiledBackground&  LevelRef )
{
   Rect collision = sprite.getCollisionArea();
   int indexTopLeft = ( (int)collision.left / 32 ) + 
                      ( (int)collision.top / 32 ) *  LevelRef.numColumns();

   int indexBottomLeft = ( (int)collision.left / 32 ) + 
                         ( (int)collision.bottom / 32 ) *
                         LevelRef.numColumns();

   int indexTopRight = ( (int)collision.right / 32 ) + 
                       ( (int)collision.top / 32 ) * LevelRef.numColumns();

   int indexBottomRight = ( (int)collision.right / 32 ) +
                          ( (int)collision.bottom / 32 ) * 
                          LevelRef.numColumns();

   if( ( sprite.collidesWith( LevelRef.mySpriteMap[indexTopLeft].tileSprite )&&  
         LevelRef.mySpriteMap[indexTopLeft].brickFlag == false &&
         LevelRef.mySpriteMap[indexTopLeft].doorFlag == true ) )
   {
      return true;
   }
   else if ( ( sprite.collidesWith( LevelRef.mySpriteMap[indexBottomLeft].tileSprite ) &&  
              LevelRef.mySpriteMap[indexBottomLeft].brickFlag == false &&
              LevelRef.mySpriteMap[indexBottomLeft].doorFlag == true ) )
   {
      return true;
   }
   else if( ( sprite.collidesWith( LevelRef.mySpriteMap[indexTopRight].tileSprite ) &&  
              LevelRef.mySpriteMap[indexTopRight].brickFlag == false &&
              LevelRef.mySpriteMap[indexTopRight].doorFlag == true ) )
   {
      return true;
   }
   else if( ( sprite.collidesWith( LevelRef.mySpriteMap[indexBottomRight].tileSprite ) &&  
             LevelRef.mySpriteMap[indexBottomRight].brickFlag == false && 
             LevelRef.mySpriteMap[indexBottomRight].doorFlag == true) )
   {
      return true;
   }

   return false;
}

//========================================================================================
//Checks if 2 sprites have collided with each other
bool CollisionManager::spriteCollsions( DxGameSprite& sprite1, DxGameSprite& sprite2 )
{
   if( ( sprite1.collidesWith( sprite2 ) && sprite2.collidable() ) )
   {
      return true;
   }

   return false;
}