
#pragma once
#if !defined(_LEVEL_H_)
#define _LEVEL_H_

#include "Utilities/Timer.h"
#include "Bomberman/Enemy.h"
#include "Bomberman/Bomb.h"
#include "Bomberman/Kitty.h"
#include "Bomberman/GameUI.h"
#include "Bomberman/CollisionManager.h"
#include "Bomberman/LevelScreens.h"
#include "Bomberman/GameUI.h"
#include "DXFramework/DxController.h"
#include "Bomberman/Powerup.h"
#include "DxFramework/DxSound.h"
#include "DxFramework/DxSoundIdentifier.h"

//========================================================================================
// Level class that manages the game level
class Level 
{
public:
   //========================================================================================
   // ctor
   Level ( DxKeyboard& keyboard, DxController& controller );

   //========================================================================================
   // dtor
   ~Level ();

public:
   //========================================================================================
   // initialize level object
   void init ( DxKeyboard& keyboard, DxController& controller );

   //========================================================================================
   // updates the level
   void update( Kitty& myKitty );

   //========================================================================================
   // updates the graphics within the level
   void draw ( IDXSPRITE spriteObj );

   //========================================================================================
   // releases all pointers, deletes objects, and sets values to NULL allowing for
   // proper deletion of this Level object
   void shutdown ();

   //========================================================================================
   // Calls shutdown and calls respective reset methods of member variables,
   // resetting the game.
   void resetGame();
   
   //========================================================================================
   // Places a bomb at the current x/y location of Kitty.
   void placeBombs( Kitty& kitty );
   
   //========================================================================================
   // Places a bomb at the current x/y location of the exisiting NegaKitties.
   void negaPlaceBombs();
   
   //========================================================================================
   // Restarts the current level.
   void restartLevel( Kitty& myKitty );
   
   //========================================================================================
   // Advances the game to the next level/stage.
   void nextLevel( Kitty& kitty );
   
   //========================================================================================
   // Checks all collisions between the player, enemies, and world objects.
   void Collisions( Kitty& kitty, GameUI& refGameUI );
   
   //========================================================================================
   // Returns the boolean value of isLevelPicFlag.
   bool isLevelPic();
   
   //========================================================================================
   // Switches the boolean value of isLevelPicFlag.
   void toggleLevelPicFlag();
   
   //========================================================================================
   // Sets an enemy to a pre-determined position.
   void setEnemyPositions();
   
   //========================================================================================
   // Updates the enemy units.
   void enemyUpdate();
   
   //========================================================================================
   // Updates the graphics of the enemy units.
   void enemyDraw( IDXSPRITE spriteObj );
   
   //========================================================================================
   // Increments the level number if there is a next level.
   void nextLevelNum(){ if( myLevelNum < SIZEOFARRAY(myLevelBgnds) - 1 ) myLevelNum++; }
   
   //========================================================================================
   // Decrements the level number if there is a previous level.
   void prevLevelNum(){ if( myLevelNum > 0 ) myLevelNum--; }

   //========================================================================================
   // Displays the game over screen.
   void loadGameOverScreen( );
   
   //========================================================================================
   // Displays the win screen.
   void loadWinScreen( );
   
   //========================================================================================
   // Returns the level number from myLevelNum
   int  getLevelNum( ) { return myLevelNum; }

   //========================================================================================
   // Return the size of the level array.
   int getSizeOfLevelArray();
   
   //========================================================================================
   // Returns a boolean value representing whether the player has won or lost.
   bool getWinLoseFlag();
   
   //========================================================================================
   // Sets the player's win/lose flag to the desired value.
   void setWinLoseFlag( bool newFlagValue );

   //========================================================================================
   // Stops the song currently playing and sets it to the next song based on the
   // current level number.
   void changeSong ();
   
   //========================================================================================
   // Stops the song currently playing.
   void stopSong ();
   
   //========================================================================================
   // Returns a boolean value representing whether or not the game is won.
   bool isGameWon() { return myWinFlag; }

   void reloadLevels( int levelNum );

private:

   unsigned int         myBombMaxCount;   
   unsigned int         powerupBombCount;
   
   float                time;
   
   void                 bombUpdate( Kitty& myKitty );
   void                 bombDraw  ( IDXSPRITE spriteObj );
   void                 powers    ( Kitty& kitty, GameUI& game );
  
   bool                 wait( int timer );


   bool                 isLevelPicFlag;  
   bool                 isWinLoseFlag;
   bool                 myWinFlag;
   bool                 dFlag;
   bool                 cFlag;
   
   int                  myLevelNum;   
   int                  myCurrentLevelNum;
   

   DxKeyboard&          myKeyboard;
   DxController&        myController;
   DxSound*             mySoundInterface;
   DxSoundIdentifier*   myCurrentBGM;

   CollisionManager     myCollisionManager;
   
   std::vector<Enemy>   myEnemy;

   GameUI               refGameUI;

   LevelScreens         myLevelNumPics;
   
   std::vector<Bomb*>   myBombs;
   
   TiledBackground      myLevelBgnds[18];   
   
   Timer                myTimer;
   
   tstring              myGameTitle;
};

#endif //_LEVEL_H_