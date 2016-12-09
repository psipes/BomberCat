#pragma once

#if !defined( _LEVELPARSER_H_ )
#define _LEVELPARSER_H_

//========================================================================================
// LevelParser class parses the levelConfig files to construct levels
class LevelParser
{
public:
   //========================================================================================
   // ctor
   LevelParser();
   
   //========================================================================================
   // dtor
   ~LevelParser();
   
   //========================================================================================
   // Loads the desired level from the Levels.config file.
   void shutdown();

   void loadLevel(int levelNum);
   
   //========================================================================================
   // Returns the level name.
   tstring getLevelName() { return myLevelName; }
   
   //========================================================================================
   // Returns the level music.
   tstring getLevelMusic() { return myLevelMusic; }
   
private:
   
   tstring  myFilePath;
   tstring  myLevelName;
   tstring  myLevelMusic;

};
#endif //_LEVELPARSER_H_