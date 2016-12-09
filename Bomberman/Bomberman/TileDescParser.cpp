#include "stdafx.h"
#include "Utilities/TTypes.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/ConfigParser.h"
#include "DxFramework/DxAssetManager.h"
#include "Bomberman/TileDescParser.h"

//===================================================================================================>
//
TileDescParser::TileDescParser()
{
   myLevelTileDescrs = NULL;
}

//===================================================================================================>
//
TileDescParser::~TileDescParser()
{
   shutdown();
}

//===================================================================================================>
//
bool TileDescParser::init ( const tstring& configFilename )
{
   bool b = parseLevelConfig( configFilename );
   return b;
}


//===================================================================================================>
//

bool TileDescParser::parseLevelConfig ( const tstring& configFilename )
{
   TCHAR state = _T('\0');
   const TCHAR* whitespace = _T(" \t\n\r\b\v" );
   const TCHAR  sectionChar = _T('@');

   struct SourceTileDescr 
   {
      tstring type, skin;
      int     idNum;
      float   aniSpeed;
   };
   std::map<int, SourceTileDescr>  srcTileDescrMap;



   myFilePath = DxAssetManager::getInstance().getConfigAssetPath( configFilename );
   tifstream      configFile( myFilePath.c_str() );
   DWORD gle = GetLastError();

   if ( !configFile.is_open() || configFile.bad() )
   {
      // Print GLE, Print log message, fail.
      assert( false );
      return false;
   }

   ConfigParser   parser;
   tstring        line, verb, arg;

   //--------- Parse @FILE <filename>
   if ( !parser.getNextLine( configFile, line ) ) 
      return false;
   if ( !parser.parsePairString( line, verb, arg ) )
      return false;

   //-------- Parse @ENEMIES:
   if ( !parser.getNextLine( configFile, line ) ) 
      return false;  //todo
   if ( line != _T("@ENEMIES") == 0  )
      return false;  //todo
         tstringstream ss( line );
         tstring enemy;
         float speed = 0.0;
         float scale = 0.0;

         ss >> verb >> enemy >> speed >> scale;
         if ( ss.fail() )
            return false;
         myEnemyType = enemy;
         myEnemySpeed = speed;
         myEmemyScale = scale;

   
   //Parse @TILES: n instances of "label idNum"
   if ( !parser.getNextLine( configFile, line ) ) 
      return false;  //todo
   if ( line.find( "@TILES" ) == tstring::npos )
      return false;  //todo

   while ( parser.getNextLine( configFile, line ) && (line[0] != sectionChar) )
   {
      tstringstream ss( line );
      int     idNum; 
      float   aniSpeed;
      tstring type, skin;

      ss >> type >> skin >> aniSpeed >> idNum;
      if ( ss.fail() )
         return false;

      SourceTileDescr srcDescr;

      srcDescr.idNum     = idNum;
      srcDescr.type      = type;
      srcDescr.skin      = skin;
      srcDescr.aniSpeed  = aniSpeed;
      srcTileDescrMap[idNum] = srcDescr;
   }


   // Exits above loop when it finds "@" (sectionChar) if all is well
   // so we should be on @DIMENSIONS mapWidth mapHeight tileWidth tileHeight
   myNumTilesHoriz = myNumTilesVert = myTileWidth = myTileHeight = 0;
   if ( line.find( _T("@DIMENSIONS") == 0 ) )
   {
      tstringstream ss( line );

      ss >> verb >> myNumTilesHoriz >> myNumTilesVert >> myTileWidth >> myTileHeight;
      if ( ss.fail() )
         return false;
   }
   else
   {
      return false; //todo - something wrong
   }

   // compute bg size
   int numTiles   = myNumTilesHoriz * myNumTilesVert;
   myLevelTileDescrs = new BgTileDescr[ numTiles ];

   if ( !parser.getNextLine( configFile, line ) ) 
      return false;  //todo
   if ( line != _T("@LEVELMAP") )
      return false;  //todo

   // for each row....
   for ( int row = 0; row < myNumTilesVert; row++ )
   {
      if ( !parser.getNextLine( configFile, line ) ) 
         assert( false );

      tstringstream ss( line );

      for ( int col = 0; col < myNumTilesHoriz; col++ )
      {
         int idNum;

         ss >> idNum;
         if ( ss.fail() )
            assert( false );

         tstring&   skin      = srcTileDescrMap[idNum].skin;
         tstring&   type      = srcTileDescrMap[idNum].type;
         float      aniSpeed  = srcTileDescrMap[idNum].aniSpeed;
         BgTileDescr& descr = getDescr(row,col);

         descr.skin     = skin;
         descr.type     = type;
         descr.idNum    = idNum;
         descr.aniSpeed = aniSpeed;
               
      }
   }


   return true;
}


//===================================================================================================>
//
tstring TileDescParser::getTileSkin (int row, int col) 
{

   return getDescr(row,col).skin;
}
//===================================================================================================>
//
tstring TileDescParser::getTileType (int row, int col) 
{

   return getDescr(row,col).type;
}

//===================================================================================================>
//
float TileDescParser::getTileSpeed (int row, int col) 
{

   return getDescr(row,col).aniSpeed;
}

//===================================================================================================>
//  
int TileDescParser::getTileID (int row, int col) 
{
   return getDescr(row,col).idNum;
}
//===================================================================================================>
//
TileDescParser::BgTileDescr& TileDescParser::getDescr ( int row, int col )
{
   //TODO: DO VALIDATION ON ROW COL HERE
   int li = (row * myNumTilesHoriz) + col;
   return myLevelTileDescrs[li];
}

//===================================================================================================>
//
void TileDescParser::shutdown ( )
{
   delete [] myLevelTileDescrs;
   myLevelTileDescrs = NULL;

   myFilePath.clear();
   myImageFilePath.clear();
   myEnemyType.clear();
}
