#pragma once

#if !defined( _TILEDBACKGROUND_H_ )
#define _TILEDBACKGROUND_H_


#include "DxFramework/DxFramework.h"
#include "DxFramework/DxCommon.h"
#include "Utilities/Logger.h"
#include "DxFramework/DxAssetManager.h"
#include "Utilities/Point.h"
#include "Bomberman/Powerup.h"



//===================================================================================================
// TiledBackground class opens a spriteSheet, Loads a description of tile distribution,
// Generates a tiled background as a texture, Provides access to the tiled background
// for use by gameRun OR draws the background
class TiledBackground
{
public:

   //===================================================================================================
   // ctor
   TiledBackground();

   //===================================================================================================
   // dtor
   ~TiledBackground();

   //===================================================================================================
   // structure representing spriteTiles containing a DxGameSprite: tileSprite.
   // tstring: tyleType.  booleans: doorFlag, enemyFlag, and brickFlag.  integers:
   struct spriteTiles 
   {
      DxGameSprite tileSprite;
      tstring tileType;
      bool doorFlag, enemyFlag, brickFlag;
      int idNum;
      int xPos, yPos, pixWidth, pixHeight;
   };

   //===================================================================================================
   // Initializes this TiledBackground object.
   bool init ( const tstring& bgConfigFilename );
   
   //===================================================================================================
   // Updates the sprites within mySpriteMap.
   bool update ();
   
   //===================================================================================================
   // Prepares this TiledBackground for destruction.
   void shutdown ();

   //===================================================================================================
   // Calls shutdown, and re-initializes this TiledBackground.
   void reset();

   //===================================================================================================
   // Draws each "tile" within mySpriteMap using the passed spriteObj.
   bool drawMySpriteMap( IDXSPRITE spriteObj );
   
   //===================================================================================================
   // Switches the animation with the passed sprite animation.
   void switchAnimation( DxGameSprite& sprite );

   //===================================================================================================
   // Returns the index position of the door.
   int getDoorIndex();
   
   //===================================================================================================
   // Returns the number of columns in the level.
   int numColumns () const { return myNumTilesHoriz; }
   
   //===================================================================================================
   // Returns the number of rows in the level.
   int numRows () const { return myNumTilesVert; }
   
   //===================================================================================================
   // Returns mySprite.
   DxGameSprite getSprite (){ return mySprite; }
   
   //===================================================================================================
   // Returns the type of enemy found on this level.
   tstring getEnemyType() const{ return myEnemyType; }
   
   //===================================================================================================
   // Returns the speed of the enemies found on this level.
   float getEnemySpeed ()    const { return myEnemySpeed; }
   
   //===================================================================================================
   // Returns the scale of the enemies found on this level.
   float getEnemyScale ()    const { return myEmemyScale; }
   
   //===================================================================================================
   // Returns myDestroyedTileAnim.
   tstring getDestroyedTile() const { return myDestroyedTileAnim; }
   
   //===================================================================================================
   // Returns myDestroyedTileAnimSpeed.
   float getDestroyedTileSpeed() const { return myDestroyedTileAnimSpeed; }

   //===================================================================================================
   // Resets the collisions of each tile within mySpriteMap.
   void resetCollisions();

   std::vector<spriteTiles> mySpriteMap;
   std::vector<Powerup>     myPowerups;

private:
   bool buildBackground ( const tstring& configFilename );


private:
   tstring        myDestroyedTileAnim;
   float          myDestroyedTileAnimSpeed;
   tstring        myFilePath;
   tstring        myImageFilePath;
   tstring        myEnemyType;
   DxGameSprite   mySprite;

   float          myEnemySpeed;
   float          myEmemyScale;
   int            myTileWidth, myTileHeight;
   int            myNumTilesHoriz, myNumTilesVert;
   int            myDoorIndex;
   int            myEnemyIndex1, myEnemyIndex2, myEnemyIndex3;

   RECT           myBgSrcRect;
   int            myBgWidth, myBgHeight;

};


#endif //_TILEDBACKGROUND_H_