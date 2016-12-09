#pragma once
#if !defined(_BOMB_H_)
#define _BOMB_H_

#include "Utilities/Rect.h"
#include "Utilities/Timer.h"
#include "DxFramework/DxGameSprite.h"
#include "DxFramework/DxImage.h"
#include "DxFramework/DxSound.h"
#include "DxFramework/DxSoundIdentifier.h"
#include "Bomberman/CollisionManager.h"
#include "Bomberman/TiledBackground.h"

//need variableness
//========================================================================================
class Bomb
{
//constructers
public:

   //========================================================================================
   //
   Bomb();

   //========================================================================================
   //
   ~Bomb();

   //========================================================================================
   //State for the bomb
   enum BombState{ UNSET, SET, LIGHT, FLASH, EXPLODE };

//Methods used outside of bomb
public:
   //========================================================================================
   //Initiliazes the bomb using an x position and a y position
   bool init( int xPos, int yPos  );

   //========================================================================================
   //Updates the bomb
   void update();

   //========================================================================================
   //Shutdowns the bomb
   void shutdown();

   //========================================================================================
   //Place the bomb in the desired position
   //    using kitty's width and height to make sure it's placed in the center
   void place( D3DXVECTOR3& position, float kittyWidth, float kittyHeight );

   //========================================================================================  
   //Draws the bomb sprite
   void draw( IDXSPRITE spriteObj );

   //========================================================================================
   //Todo
   void shrink();

   //========================================================================================
   //Select the bomb sprites explode animation
   void explode ();

   //========================================================================================
   //Returns the bomb sprite
   DxGameSprite& getSprite(){ return mySprite; }

   //========================================================================================
   //Returns the bomb explosion sprite at the specified index
   DxGameSprite& getSprite( int num ) { return myExplosions[num]; }

   //========================================================================================
   //Return true if the bomb is exploding
   bool getIsExploding(){ return myBombExploding; }

   //========================================================================================
   //Return true if the bomb is placed
   bool getIsBombPlaced(){ return myIsBombSet; }

   //========================================================================================
   //Return the number of bombs
   int  getBombcount(){ return myBombCount; }

   //========================================================================================
   //Sets the number of bombs
   void setBombCount(int bombCount ){ myBombCount = bombCount; }

   //========================================================================================
   //Increases the number of exoplosion sprites on a bomb
   static void increaseExplosionSize(){ if(myExplLength<8)myExplLength++; }

   //========================================================================================
   //Decreases the number of exoplosion sprites on a bomb
   static void decreaseExplosionSize(){ if(myExplLength>1)myExplLength--; }

   //========================================================================================
   //Return the enum state of the bomb
   BombState state (){ return myBombState; }

   //========================================================================================
   //Todo
   bool getEnemyBombFlag(){ return myIsEnemyBombFlag; }

   //========================================================================================
   //Todo
   void setEnemyBombFlag( bool isEnemyBomb ){ myIsEnemyBombFlag = isEnemyBomb; }

   //========================================================================================
   //Handles the bomb when it collides with the level
   void resolveCollision ( TiledBackground& level );

   //========================================================================================
   //Handles the bomb when the explosion collides with another sprite
   bool collision ( DxGameSprite& sprite );


   //========================================================================================
   //Methods used by bomb only
private:

   //========================================================================================
   //Loads all the sounds for the bomb
   void loadCharacterSounds();

   //========================================================================================
   //Initiliazes the explosion pillar size
   void initPillars();

   //========================================================================================
   //Changes the animation of the bomb depending on state
   void selectAnimation( BombState State );

   //========================================================================================
   //Controls what happens when the bomb explodes
   void onExplode ();

   //========================================================================================
   //Controls how the explosions spread out
   void grow();

   //========================================================================================
   //Resets bomb to flag
   void reset();

   //========================================================================================
   //Returns true if the explosion sprite animations have ended
   bool explosionHasEnded ();

//member variables
private:
   float        myMaxExpLength;
   float        myBmbFSpeed;
   float        myExpFSpeed;
   float        myScale;
   float        myExplosionScale;
   float        myTimeToDelay;
   float        myExplSize;
   
   static float myExplLength;
   
   int          myBombCount;
   int			 myPowerupChance;

   bool         myExplFlag[4];
   bool         myBombExploding;
   bool         myFirstTimeAnimFlag;// need to be static to work
   bool         myFirstTimeSoundFlag;// need to be static to work
   bool         myIsBombSet;
   bool         myIsEnemyBombFlag;
   
   Rect         myDownExpl;
   Rect         myUpExpl;  
   Rect         myRightExpl;
   Rect         myLeftExpl;
   
   BombState    myBombState;
   
   DxGameSprite mySprite;
   
   std::vector<DxGameSprite> myExplosions;
   
   Timer        myTimer;

   D3DXVECTOR3  myPosition;
   D3DXVECTOR3  myLastPosition;

   CollisionManager myCollisionManager;

   DxSound* mySoundInterface;
   DxSoundIdentifier* mySound;
};

#endif