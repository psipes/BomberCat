
#pragma once
#if !defined(_LEVELSCREENS_H_)
#define _LEVELSCREENS_H_

#include "Utilities/Timer.h"
#include "DxFramework/DxText.h"
#include "Bomberman/TiledBackground.h"
#include "DxFramework/DxGameSprite.h"
   
//========================================================================================
// LevelScreens class represents the screens to be displayed before entering a level.
class LevelScreens 
{
public:

   //========================================================================================
   // ctor
   LevelScreens ();
   
   //========================================================================================
   // dtor
   ~LevelScreens ();

public:
   //========================================================================================
   // Initializes this LevelScreens object.
   void init( );

   //========================================================================================
   // Calls update methods of member variables that require an update.
   void update( );

   //========================================================================================
   // Calls update methods of member variables that require an update to draw
   // themselves on the screen.
   void draw( IDXSPRITE spriteObj );

   //========================================================================================
   // Prepares this object for destruction.
   void shutdown( );

   //========================================================================================
   // Returns the current level picture.
   DxGameSprite& getCurrentLevelPic( );

   //========================================================================================
   // Sets the current level picture to the desired level picture.
   void setCurrentLevelPic( DxGameSprite &newLevelPic );

   //========================================================================================
   // increments the index of the current picture index.
   void incCurrentLevelPic( );

   //========================================================================================
   // decrements the index of the current picture index.
   void decCurrentLevelPic( );

   //========================================================================================
   // Displays the win screen.
   void loadWinScreen( );

   //========================================================================================
   // Displays the lose screen.
   void loadLoseScreen( );

   //========================================================================================
   // Returns the number of the last level.
   int getLastLevelNum( ) { return myLastLevelNum; }

   //========================================================================================
   // Resets the screen to the first level and first screen.
   void resetScreens();


private:
   Timer                      myTimer;

   int                        myLevelNum;
   DxGameSprite               myLevelPics[19];
   static tstring             myLevelPicNames[19];

   DxGameSprite               myWinLosePics[2];
   static tstring             myWinLoseNames[2];

   DxGameSprite               pressStart;   

   DxGameSprite               myCurrentLevelPic;   
   int                        myCurrentPicIndex;

   int                        myLastLevelNum;

   

};                                        

#endif //_LEVEL_H_                        
