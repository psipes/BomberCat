#pragma once

#if !defined( _COLLISIONMANAGER_H_ )
#define _COLLISIONMANAGER_H_

#include "DxFramework/DxFramework.h"
#include "DxFramework/DxGameSprite.h"
#include "DxFramework/DxGameSprite.h"
#include "DxFramework/DxImage.h"
#include "Bomberman/TiledBackground.h"



//========================================================================================
//
class CollisionManager
{
public:

   //========================================================================================
   //
   CollisionManager();

   //========================================================================================
   //
   ~CollisionManager();

   //========================================================================================
   //Initiliazes the collision manager
   void init();

   //========================================================================================
   //Updates the collision manager
   void update();

   //========================================================================================
   //Shutdowns the collision manager
   void shutdown();
   
   //========================================================================================
   //Handles the collisions of a sprite and the level
   //Todo
   bool worldCollisions( DxGameSprite& sprite1, TiledBackground&  myLevelRef, int* index = NULL );

   //========================================================================================
   //Handles the collisions of a explosion sprite and the level
   bool exploisionCollisions( DxGameSprite sprite, TiledBackground&  LevelRef );

   //========================================================================================
   //Handles the collisions of a sprite and another sprite
   bool spriteCollsions( DxGameSprite& sprite1, DxGameSprite& sprite2 );

   //========================================================================================
   //Handles the collisions of a sprite and the level's door
   bool doorCollisions( DxGameSprite& sprite1, TiledBackground&  myLevelRef );

private:

   //========================================================================================
   //Loads the animations
   bool loadAnimation();

private:
   bool myFirstTimeFlag;

   int myDoorIndex;

   DxAnimation myGrassAnim;
   DxAnimation myBoomBrickAnim;


};

#endif 
