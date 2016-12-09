#pragma once
#if !defined(_KITTY_H_)
#define _KITTY_H_

#include "Utilities/TTypes.h"
#include "Utilities/Timer.h"
#include "DxFramework/DxImage.h"
#include "DxFramework/DxGameSprite.h"
#include "DxFramework/DxAnimation.h"
#include "DxFramework/DxSound.h"
#include "DxFramework/DxSoundIdentifier.h"
#include "DxFramework/DxKeyboard.h"
#include "Bomberman/Bomb.h"
#include "Bomberman/Powerup.h"



class Kitty
{
public:
   Kitty ( );
   ~Kitty ( );

   //========================================================================================
   //Direction kitty is facing
   enum Direction{ STILL, STILLUP, STILLDOWN, STILLLEFT, STILLRIGHT, UP, DOWN, LEFT, RIGHT };
   // enum Direction{ UP = 1, DOWN, LEFT, RIGHT, STILL = (1 << 8) };  //SAS: Idea on how STILL is a "qualifier" on the ACTUAL 4 direction STATES; if ( myState & STILL )
   // And... STATE might be more than just direction and motion....



   //========================================================================================
   //Initializes the kitty
   bool init ( int xPos, int yPos );

   //========================================================================================
   //Updates kitty
   void update ();

   //========================================================================================
   //Shutdowns kitty
   void shutdown ();

   //========================================================================================
   //Draw kitty
   bool draw ( IDXSPRITE spriteobj );

   //========================================================================================
   //Stop kitty
   bool goStop();

   //========================================================================================
   //Move kitty upward
   bool goUp();

   //========================================================================================
   //Move kitty downward
   bool goDown();  

   //========================================================================================
   //Moves kitty left
   bool goLeft();

   //========================================================================================
   //Moves kitty right
   bool goRight();

   //========================================================================================
   //Reverse kitty direction
   bool goReverse();

   //========================================================================================
   //Todo
   bool goStillUp();

   //========================================================================================
   //Todo  
   bool goStillDown();

   //========================================================================================
   //Todo
   bool goStillLeft();

   //========================================================================================
   //Todo
   bool goStillRight();

   //========================================================================================
   //Place a bomb
   void place();

   //========================================================================================
   //Reset the animations
   void reset();

   //========================================================================================
   //Changes kitty speed
   void changeSpeed() { mySpeed = mySpeed + .25f; }

   //========================================================================================
   //Returns true if kitty speed is less than max speed
   bool checkSpeed() { return mySpeed < maxSpeed; }

   //========================================================================================
   //Checks collisions with other sprites
   bool collidesWith ( const DxGameSprite& other){ return mySprite.collidesWith( other ); };

   //========================================================================================
   //Checks radial collisions with other sprites
   bool radialCollidesWith ( const DxGameSprite& other){ return mySprite.radialCollidesWith( other ); };

   //========================================================================================
   //Set kitty position
   void setMyPosition( D3DXVECTOR3 pos ){ myPosition = pos; mySprite.setPosition(myPosition.x, myPosition.y ); }

   //========================================================================================
   //Returns kitty position
   D3DXVECTOR3 getMyPosition() { return myPosition; }

   //========================================================================================
   //Returns kitty last position
   D3DXVECTOR3 getLastPosition() { return myLastPosition; }

   //========================================================================================
   //Returns the kitty sprite
   DxGameSprite getSprite() { return mySprite; }

   //========================================================================================
   //Returns true if kitty is dead
   bool getDeathState(){ return myIsDeadFlag; }

   //========================================================================================
   //Changes the death flag for kitty
   void ToggleDeath( bool flag ){ myDyingFlag = flag; }

   //========================================================================================
   //Make kitty immune
   void Kitty::kittyImmuneStart();

   //========================================================================================
   //Stops kitty immunity
   void kittyImmuneStop( int time );

   //========================================================================================
   //Return power up
   Powerup::Power power () { return myPower; }

   //========================================================================================
   //Return power up after setting it equal to power
   Powerup::Power power ( Powerup::Power power ) { return (myPower = power); }

   //========================================================================================
   //Kitty lives code
   //Returns the number of lives
   int getLivesAmount(){ return myLivesAmount; }

   //========================================================================================
   //Subtracts one life from kitty
   void loseLife(){ myLivesAmount--; }

   //========================================================================================
   //Adds one life to kitty
   void gainLife(){ if(myLivesAmount < 9)myLivesAmount++; }

   //========================================================================================
   //Reset lives back to nine
   void resetLives() { myLivesAmount = 9; }


private:

   //========================================================================================
   //Pause kitty
   bool wait( int timer );

   //========================================================================================
   //Todo
   void died();

   //========================================================================================
   //Loads all kitty animations
   bool loadCharacterAnimations();

   //========================================================================================
   //Loads all kitty sounds
   bool loadCharacterSounds();

   //========================================================================================
   //Todo
   bool stillDirection( Direction prevDirection );


private:
   
   float          mySpeed;
   float          maxSpeed;
   float          myStartXPos;
   float          myStartYPos;

   int            myLivesAmount;
   int            index;
   
   bool           myFirstTimeAnimFlag;
   bool           myFirstTimeSoundFlag;
   bool           myIsImmuneFlag;
   bool           myIsDeadFlag;
   bool           myDyingFlag;
 
   //Animation may not be needed
   
   DxAnimation    myCatStillAnim;
   DxAnimation    myCatStillUpAnim;
   DxAnimation    myCatStillDownAnim;
   DxAnimation    myCatStillLeftAnim;
   DxAnimation    myCatStillRightAnim;
   DxAnimation    myCatWalkUpAnim;
   DxAnimation    myCatWalkDownAnim;
   DxAnimation    myCatWalkLeftAnim;
   DxAnimation    myCatWalkRightAnim;
   DxAnimation    myCatBlowUpAnim;
   
   D3DXVECTOR3    myPosition;
   D3DXVECTOR3    myLastPosition;   
   
   Direction      myDirection;
   Timer          myTimer;
   Powerup::Power myPower;
   
   DxGameSprite   mySprite;
   
   DxSound* mySoundInterface;
   DxSoundIdentifier* mySound;
   DxSoundIdentifier* myDeath;

};
#endif //_TESTAPP_H_