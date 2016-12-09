#pragma once

#if !defined( _TILEDESCPARSER_H_ )
#define _TILEDESCPARSER_H_

#include "Utilities/Logger.h"
#include "DxFramework/DxFramework.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxAssetManager.h"


//===================================================================================================
// TileDescParser class parses levelConfig for the description of 
// tiles needed for each level.
class TileDescParser 
{
public:
   //===================================================================================================
   // ctor
   TileDescParser();

   //===================================================================================================
   // dtor
   ~TileDescParser();

   //===================================================================================================
   // structure representing BgTileDescr containing tstrings: skin, type.  
   // int idNum. float aniSpeed.
   struct BgTileDescr 
   {
      tstring     skin;
      tstring     type;
      int         idNum;
      float       aniSpeed;
      //DxTexture   texture;
      // Do we NEED anything else here... not right now... 
      // We *could* put pixWidth,pixHeight - which would allow the bg tiles to be *any* size
      // and we *could* put xPos,yPos - but I think the Level would really be more likely to
      // *calculate* these based on the tile's logical position (row,col) and height,width
   };

   //===================================================================================================
   // Initializes this object.
   bool init ( const tstring& bgConfigFilename );
   
   //===================================================================================================
   // Prepares this object for destruction.
   void shutdown ();

   //===================================================================================================
   // returns the file path of the source file.
   tstring sourceFilename ( ) const { return myImageFilePath; }

   //===================================================================================================
   // returns the number of columns found within the source file.
   int numTileCols ( )        const { return myNumTilesHoriz; }
   
   //===================================================================================================
   // returns the number of rows found within the source file.
   int numTileRows ( )        const { return myNumTilesVert; }
   
   //===================================================================================================
   // returns the width of each tile in pixels.
   int getTilePixWidth  ( )   const { return myTileWidth; }
   
   //===================================================================================================
   // returns the height of each tile in pixels.
   int getTilePixHeight ( )   const { return myTileHeight; }
   
   //===================================================================================================
   // returns the enemy type.
   tstring getEnemyType ()    const { return myEnemyType; }
   
   //===================================================================================================
   // returns the enemy speed.
   float getEnemySpeed ()    const { return myEnemySpeed; }
   
   //===================================================================================================
   // returns the enemy scale.
   float getEnemyScale ()    const { return myEmemyScale; }
   
   //===================================================================================================
   // returns the tile skin found at row/col.
   tstring getTileSkin ( int row, int col );
   
   //===================================================================================================
   // returns the tile type found at row/col.
   tstring getTileType ( int row, int col );
   
   //===================================================================================================
   // returns the tile speed found at row/col.
   float getTileSpeed ( int row, int col );

   //===================================================================================================
   // returns the tile id found at row/col.
   int getTileID (int row, int col);

   //DxTexture& getTileTexture ( int row, int col );

private:

   //===================================================================================================
   // Parses the desired level configuration file.
   bool parseLevelConfig ( const tstring& configFilename );
   
   //===================================================================================================
   // Returns a BgTileDescr at the tile found at row/col.
   BgTileDescr&  getDescr ( int row, int col );

private:
   tstring     myFilePath;
   tstring     myImageFilePath;
   tstring     myEnemyType;
   float       myEnemySpeed;
   float       myEmemyScale;

   int         myTileWidth, myTileHeight;       // Actual size rqrd in the tiled bg, NOT image width/height
   int         myNumTilesHoriz, myNumTilesVert;

   BgTileDescr*  myLevelTileDescrs;


};


#endif 
