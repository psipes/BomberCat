#pragma once
#if !defined(_ENEMY_H_)
#define _ENEMY_H_

#include "Utilities/TTypes.h"
#include "DxFramework/DxImage.h"
#include "DxFramework/DxGameSprite.h"
#include "DxFramework/DxAnimation.h"
#include "DxFramework/DxSound.h"
#include "DxFramework/DxSoundIdentifier.h"



//========================================================================================
//
class Enemy
{
public:

   //========================================================================================
   //
   Enemy ( );

   //========================================================================================
   //
   ~Enemy ( );

   //========================================================================================
   //Initializes an enemy using x position, y position and a speed and scale        
   bool init ( int xPos, int yPos, tstring eType, float eSpeed, float eScale );

   //========================================================================================
   //Update an enemy
   void update ();

   //========================================================================================
   //Shutdowns an enemy
   void shutdown ();

   //========================================================================================
   //Draws an enemy
   bool draw ( IDXSPRITE spriteobj );

   //========================================================================================
   //Todo
   void enemyType();

   //========================================================================================
   //Changes the direction an enemy is facing
   void changeDirection();

   //========================================================================================
   //Disables all enemy motion
   bool stop();

   //========================================================================================
   //Handles collision with another sprite
   bool collidesWith ( const DxGameSprite& other){ return mySprite.collidesWith( other ); };

   //========================================================================================
   //Sets an enemy position
   void setMyPosition( D3DXVECTOR3 pos ){ myPosition = pos; mySprite.setPosition( myPosition.x, myPosition.y ); }

   //========================================================================================
   //Places a bomb for an enemy
   bool negaKittyPlaceBomb(int time);

   //========================================================================================
   //Checks to see if enemy is dead
   void died();

   //========================================================================================
   //Method that waits for a certain amount of time
   bool wait( int timer );

   //========================================================================================
   //Returns enemy position
   D3DXVECTOR3 getMyPosition() { return myPosition; }

   //========================================================================================
   //Returns enemy last position
   D3DXVECTOR3 getLastPosition() { return myLastPosition; }

   //========================================================================================
   //Returns enemy sprite
   DxGameSprite& getSprite() { return mySprite; }

   //========================================================================================
   //Checks to see if enemy is dead
   void isEnemyDead( bool flag ) { myIsDyingFlag = flag; };

   //========================================================================================
   //Returns true if enemy is not moving
   bool getisDeadFlag() { return myIsNotMoveingFlag; };

   //========================================================================================
   //Return enemy type
   tstring getEnemyType(){ return myEnemyType; }

   //========================================================================================
   //Return enemy point value
   int& getPointValue() { return myPointValue; }

   //========================================================================================
   //Set enemy return value
   void setPointValue( int newPointValue ) { myPointValue = newPointValue; } 

private:   
 
   float          mySpeed;

   int            myPointValue;

   bool           myFirstTimeFlag;
   bool           myIsDyingFlag;
   bool           myIsDeadFlag;
   bool           myIsNotMoveingFlag;

   D3DXVECTOR3    myPosition;
   D3DXVECTOR3    myLastPosition;
    
   DxGameSprite   mySprite;
   
   Timer          myTimer;

   tstring        myEnemyType;

   DxSound* mySoundInterface;
   DxSoundIdentifier* mySound;
    
};
#endif //_TESTAPP_H_