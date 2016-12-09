#include "stdafx.h"
#include "Utilities/Point.h"
#include "Utilities/Rect.h"
#include "Utilities/ConfigParser.h"
#include "Bomberman/TiledBackground.h"
#include "Bomberman/TileDescParser.h"

//===================================================================================================>
//
TiledBackground::TiledBackground()
{
   myNumTilesVert = 0;
   myNumTilesHoriz = 0;
}

//===================================================================================================>
//
TiledBackground::~TiledBackground()
{

}

//===================================================================================================>
//
bool TiledBackground::init ( const tstring& configFilename )
{
   myFilePath = configFilename;

   DxAssetManager& assetManager = DxAssetManager::getInstance();

   bool b = buildBackground( configFilename );

   return b;
}

//===================================================================================================>
//
bool TiledBackground::buildBackground ( const tstring& configFilename )
{
   TileDescParser fileInfo;
   bool result = true;

   //door work
   srand( unsigned int( time( NULL ) ) );

   if ( !fileInfo.init( configFilename ) )
   {
      assert(false);
      return false;
   }

   myNumTilesVert  = fileInfo.numTileRows();
   myNumTilesHoriz = fileInfo.numTileCols();
   myEnemyType     = fileInfo.getEnemyType();
   myEnemySpeed    = fileInfo.getEnemySpeed();
   myEmemyScale    = fileInfo.getEnemyScale();

   mySpriteMap.resize( myNumTilesVert * myNumTilesHoriz );
      
   int index = 0;

   int tilePixWidth  = fileInfo.getTilePixWidth();
   int tilePixHeight = fileInfo.getTilePixHeight();

   std::vector<int> brickIndexVector;
   brickIndexVector.resize( myNumTilesVert * myNumTilesHoriz );
   int brickCount = 0;

   std::vector<int> grassIndexVector;
   grassIndexVector.resize( myNumTilesVert * myNumTilesHoriz );
   int grassCount = 0;


   for (int row = 0; row < fileInfo.numTileRows(); row++)
   {
      for (int col = 0; col < fileInfo.numTileCols(); col++)
      {
         int xPos = col*tilePixWidth;
         int yPos = row*tilePixHeight; 

         mySpriteMap[index].tileSprite.setScale( .25, .25 );
         mySpriteMap[index].idNum = fileInfo.getTileID(row, col);
         mySpriteMap[index].xPos = xPos;
         mySpriteMap[index].yPos = yPos;


         if(  fileInfo.getTileType( row, col ) == _T("DESTROYABLE") )
         {
            mySpriteMap[index].tileSprite.create( fileInfo.getTileSkin( row, col ), fileInfo.getTileSpeed( row, col ) );
            mySpriteMap[index].tileSprite.getAnimation().stop();
            mySpriteMap[index].brickFlag = true;
            mySpriteMap[index].tileSprite.setDestroyable ( true );
            mySpriteMap[index].tileSprite.collidable ( true );

            brickIndexVector[brickCount] = index;
            brickCount++;
         }
         else if ( fileInfo.getTileType( row, col ) == _T("UNDESTROYABLE") )
         {
            mySpriteMap[index].tileSprite.create( fileInfo.getTileSkin( row, col ), fileInfo.getTileSpeed( row, col ) );
            mySpriteMap[index].tileSprite.setDestroyable ( false );
            mySpriteMap[index].tileSprite.collidable ( true );
         }
         else if ( fileInfo.getTileType( row, col ) == _T("PASSABLE") )
         {
            mySpriteMap[index].tileSprite.create( fileInfo.getTileSkin( row, col ), fileInfo.getTileSpeed( row, col ) );
            myDestroyedTileAnim = fileInfo.getTileSkin( row, col );
            myDestroyedTileAnimSpeed = fileInfo.getTileSpeed( row, col );
            mySpriteMap[index].tileSprite.collidable(false);
            if ( mySpriteMap[index].xPos > 192 && mySpriteMap[index].yPos > 256  )
            {
                grassIndexVector[ grassCount ] = index;
                grassCount++;
            }
         }
         
         mySpriteMap[index].tileSprite.setPosition( float(xPos), float(yPos) );

         index++;
      }
   }

   int randomBrickIndex = rand() % brickCount; 
   myDoorIndex = brickIndexVector[ randomBrickIndex ];
   mySpriteMap[ myDoorIndex ].doorFlag = true;
   
   int grassEnemyFactor = grassCount / 3;
   int randomGrassIndex = rand() % grassEnemyFactor;
   myEnemyIndex1 = grassIndexVector[ randomGrassIndex ];
   mySpriteMap[ myEnemyIndex1 ].enemyFlag = true;
   myEnemyIndex2 = grassIndexVector[ randomGrassIndex + grassEnemyFactor];
   mySpriteMap[ myEnemyIndex2 ].enemyFlag = true;
   myEnemyIndex3 = grassIndexVector[ randomGrassIndex + (2*grassEnemyFactor)];
   mySpriteMap[ myEnemyIndex3 ].enemyFlag = true;


   return true;
}

//===================================================================================================>
//
bool TiledBackground::update ( )
{
   for( unsigned int i = 0; i < mySpriteMap.size(); i++)
   {
      mySpriteMap[i].tileSprite.update();
   }
   for( unsigned int i = 0; i < myPowerups.size(); i++)
   {
      myPowerups[i].update();
   }
   
   return true;
}

//===================================================================================================>
//
void TiledBackground::reset ( )
{
   shutdown( );
   init( myFilePath );
}


//===================================================================================================>
//
void TiledBackground::shutdown ( )
{
   resetCollisions();

   myDestroyedTileAnim.clear();
   myImageFilePath.clear();
   myEnemyType.clear();
   mySprite.destroy();

   for(unsigned int i = 0; i < myPowerups.size(); i++)
   {
      myPowerups[i].destroy();
   }

   // reset flags
   for ( unsigned int i = 0; i < mySpriteMap.size(); i++ )
   {
      mySpriteMap[i].enemyFlag = false;
      mySpriteMap[i].brickFlag = false;
      mySpriteMap[i].doorFlag = false;
   }

   myBgSrcRect = Rect(0, 0, 0, 0);
}

//===================================================================================================>
//
bool TiledBackground::drawMySpriteMap( IDXSPRITE spriteobj )
{
   for( size_t i = 0; i < mySpriteMap.size(); i++ )
   {
      mySpriteMap[i].tileSprite.draw( spriteobj );
   }
   for( size_t i = 0; i < myPowerups.size(); i++ )
   {
      myPowerups[i].draw( spriteobj );
   }
   return true;
}

//===================================================================================================>
//
void TiledBackground::switchAnimation( DxGameSprite& sprite )
{
   //sprite.changeAnimation( ( tileType + "-DESTROY" ) );
}

//===================================================================================================>
//
void TiledBackground::resetCollisions()
{
   for( unsigned int i = 0; i < mySpriteMap.size(); i++ )
   {
      if( mySpriteMap[i].tileType == _T("DESTROYABLE") )
      {
         mySpriteMap[i].tileSprite.collidable( true );
      }
   }
}

