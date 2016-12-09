#include "stdafx.h"
#include "Utilities/TTypes.h"
#include "Utilities/Logger.h"
#include "Utilities/PathUtilities.h"
#include "Utilities/ConfigParser.h"
#include "DxFramework/DxFramework.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxAssetManager.h"
#include "Bomberman/LevelParser.h"


LevelParser::LevelParser()
{

}

//===================================================================================================>
//
LevelParser::~LevelParser()
{
   
}
//===================================================================================================>
//
void LevelParser::shutdown()
{
   myFilePath.clear();
   myLevelName.clear();
   myLevelMusic.clear();
}
//===================================================================================================>
//
void LevelParser::loadLevel( int levelNum )
{
   tstring configFilename = "Levels.config";
   myFilePath = DxAssetManager::getInstance().getConfigAssetPath( configFilename );
   tifstream      configFile( myFilePath.c_str() );

   if ( !configFile.is_open() || configFile.bad() )
      assert( false );
  
   ConfigParser   parser;
   tstring        line;
   int index = 0;

   while( parser.getNextLine( configFile, line ) || index > levelNum )
   {
     // parser.getNextLine( configFile, line );
      
      tstringstream ss( line );
      tstring name;
      tstring music;
      int num = 0;
      ss >> num >> name >> music;

      if( num == levelNum)
      {
         myLevelName = name;
         myLevelMusic = music;
      }
   }

   configFile.close();
}
