#include "stdafx.h"
#include "Bomberman/Kitty.h"

//Check on device
//=======================================================================
//
Kitty::Kitty ( )
{
   index = 0;
   myFirstTimeAnimFlag = true;
   myFirstTimeSoundFlag = true;
   myIsImmuneFlag = false;
   myIsDeadFlag = false;
   myDyingFlag = false;
   myDirection = STILLDOWN;
   mySpeed = 0.0F;
	myPosition.x = myPosition.y = myPosition.z = 0;
	myLastPosition.x = myLastPosition.y = myLastPosition.z = 0;
   myLivesAmount = 9;
   maxSpeed = 2.5f;
   myPower = Powerup::POWER_NONE;

}
//=======================================================================
//
Kitty::~Kitty ( )
{

}
//=======================================================================
//
bool Kitty::init( int xPos, int yPos )
{
   mySoundInterface = DxSound::getInterface( DxSound::fmod );
   myDirection = STILLDOWN;
   loadCharacterSounds();
   loadCharacterAnimations();
   mySprite.setScale( .25f, .25f );
   bool result = mySprite.create( "BCAT-STILL" );
   mySprite.setPosition( float( xPos ), float( yPos ) );
   myStartXPos = float( xPos );
   myStartYPos = float( yPos );
   myPosition.x = mySprite.getXPosition();
   myPosition.y = mySprite.getYPosition();

   RECT temp = mySprite.getCollisionArea();
   temp.top = temp.top + 8;

   mySprite.setCollisionArea( temp );

   mySpeed = 1.5f;

   assert(result);
   
   return true;
}
//=======================================================================
//
void Kitty::update()
{
   myLastPosition.x = myPosition.x;
   myLastPosition.y = myPosition.y;
   mySprite.update();
   myPosition.x = mySprite.getXPosition();
   myPosition.y = mySprite.getYPosition();
   mySoundInterface->update();
   if( myIsDeadFlag || myDyingFlag )
      died();
   if( myIsImmuneFlag )
      kittyImmuneStop( 5 );

}

//=======================================================================
//
void Kitty::shutdown ( )
{
   mySound = NULL;
   myDeath = NULL;

   myCatStillAnim.shutdown();
   myCatStillUpAnim.shutdown();
   myCatStillDownAnim.shutdown();
   myCatStillLeftAnim.shutdown();
   myCatStillRightAnim.shutdown();

   myCatWalkUpAnim.shutdown();
   myCatWalkDownAnim.shutdown();
   myCatWalkLeftAnim.shutdown();
   myCatWalkRightAnim.shutdown();
   myCatBlowUpAnim.shutdown();

   myPosition = D3DXVECTOR3(0, 0, 0);
   myLastPosition = D3DXVECTOR3(0, 0, 0);

   mySprite.destroy();
   mySpeed = 0.0f;
   maxSpeed = 0.0f;
   myStartXPos = 0.0f;
   myStartYPos = 0.0f;
   myTimer.restart();
   myTimer.stop();
   myLivesAmount = 0;
   index = 0;

   myFirstTimeAnimFlag = false;
   myFirstTimeSoundFlag = false;
   myIsImmuneFlag = false;
   myIsDeadFlag = false;
   myDyingFlag = false;
}

//=======================================================================
//
bool Kitty::draw ( IDXSPRITE spriteObj )
{ 
   mySprite.draw( spriteObj );


   return true;
}
//=======================================================================
bool Kitty::loadCharacterSounds ()
{
   if( myFirstTimeSoundFlag )
   {
      //Change firstTimeFlag
      myFirstTimeSoundFlag = false;
      mySound = DxAssetManager::getInstance().getSound( _T("CAT_STEPS") );
      myDeath = DxAssetManager::getInstance().getSound( _T("KITTY_DEATH") );
   }
   return true;
}
//=======================================================================
bool Kitty::loadCharacterAnimations ()
{
   if( myFirstTimeAnimFlag )
   {
      myFirstTimeAnimFlag = false;
      myCatStillAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-STILL", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );

      myCatWalkDownAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-DOWN", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      myCatWalkLeftAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-LEFT", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      myCatWalkRightAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-RIGHT", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      myCatWalkUpAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-UP", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      
      myCatStillDownAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-STILLDOWN", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      myCatStillLeftAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-STILLLEFT", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      myCatStillRightAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-STILLRIGHT", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      myCatStillUpAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-STILLUP", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      
      myCatBlowUpAnim = DxAssetManager::getInstance().getAnimationCopy( "BCAT-BLOWUP", 5, D3DCOLOR_XRGB( 170, 181, 129 ) );   
   }
   return true;
}
//=======================================================================
// TURNS OFF ALL KITTY DIRECTIONAL MOTION!!!@
bool Kitty::goStop ( )
{
   Direction prevDirection = myDirection;

   // Stop actual sprite motion.
   mySprite.setXVel(0.0f);
   mySprite.setYVel(0.0f);

   // Set next state based on Current state.
   switch ( myDirection )
   {
   case Direction::UP:
   case Direction::STILLUP:
      myDirection = Direction::STILLUP;
      break;
   case Direction::DOWN:
   case Direction::STILLDOWN:
      myDirection = Direction::STILLDOWN;
      break;
   case Direction::LEFT:
   case Direction::STILLLEFT:
      myDirection = Direction::STILLLEFT;
      break;
   case Direction::RIGHT:
   case Direction::STILLRIGHT:
      myDirection = Direction::STILLRIGHT;
      break;
   default:
      myDirection = Direction::STILL;
      break;
   }


   if ( prevDirection != myDirection )
        stillDirection( prevDirection );

   return true;
}
//=======================================================================
//TODO: Consider actual motion...
//  If current move state == up, nothing to do.
// otherwise....
//    myKitty state == UP : Immediate change
   // if myKitty is ANY OTHER STILL, change to STILLUP state
//    ??? Change from any to STILL UP on first keypress????
//    if myKitty is ANY STILL or STILLUP, change to UP state.
//For Kitty walking
//    Added sound->play and sound->stop to all go methods works good/ok.
//    When myKitty collides with a wall she moves then stops making the sound
//    play and stop within frames
bool Kitty::goUp ( )
{
   mySoundInterface->play( mySound ); 
   Direction temp = myDirection;
   myDirection = Direction::UP;
   mySprite.setXVel( mySpeed - mySpeed );
   mySprite.setYVel(-mySpeed);
   //mySprite.changeAnimation( "BCAT-UP" );

   if(myDirection != temp)
      mySprite.changeAnimation( myCatWalkUpAnim );

   return true;
}
//=======================================================================
//
bool Kitty::goDown ( )
{
   mySoundInterface->play( mySound ); 
   Direction temp = myDirection;
   myDirection = Direction::DOWN;
   mySprite.setXVel(mySpeed - mySpeed);
   mySprite.setYVel(+mySpeed);   //TODO :  Constant somewhere for myKitty speed
   //mySprite.changeAnimation( "BCAT-UP" );
   if(myDirection != temp)
      mySprite.changeAnimation( myCatWalkDownAnim );

   return true;
}
//=======================================================================
//
bool Kitty::goLeft()
{
   mySoundInterface->play( mySound ); 
   Direction temp = myDirection;
   myDirection = Direction::LEFT;
   mySprite.setXVel(-mySpeed);
   mySprite.setYVel(mySpeed - mySpeed);
   //mySprite.changeAnimation( "BCAT-UP" );
   if(myDirection != temp)
      mySprite.changeAnimation( myCatWalkLeftAnim );

   return true;
}
//=======================================================================
//
bool Kitty::goRight()
{
   mySoundInterface->play( mySound ); 
   Direction temp = myDirection;
   myDirection = Direction::RIGHT;
   mySprite.setXVel(+mySpeed);
   mySprite.setYVel( mySpeed - mySpeed ); //TODO :  Constant somewhere for myKitty speed
   if(myDirection != temp)
      mySprite.changeAnimation( myCatWalkRightAnim );

   return true;
}
//=======================================================================
//
bool Kitty::goReverse()
{
   mySprite.setXVel( -mySprite.getXVel() );
   mySprite.setYVel( -mySprite.getYVel() );
   return true;
}

//=======================================================================
//
bool Kitty::goStillUp()
{
   mySoundInterface->stop( mySound );
   mySprite.changeAnimation( myCatStillUpAnim );
   return true;
}

//=======================================================================
//
bool Kitty::goStillDown()
{
   mySoundInterface->stop( mySound );
   mySprite.changeAnimation( myCatStillDownAnim );
   return true;
}

//=======================================================================
//
bool Kitty::goStillLeft()
{
   mySoundInterface->stop( mySound );
   mySprite.changeAnimation( myCatStillLeftAnim );
   return true;
}

//=======================================================================
//
bool Kitty::goStillRight()
{
   mySoundInterface->stop( mySound );
   mySprite.changeAnimation( myCatStillRightAnim );
   return true;
}
//=======================================================================
//
void Kitty::died()
{
   mySprite.setXVel(0.0f);
   mySprite.setYVel(0.0f);
   
   if( myDyingFlag )
   {
      mySprite.collidable( false);
      mySoundInterface->play( myDeath );
      mySprite.changeAnimation( myCatBlowUpAnim );
      myDyingFlag = false;
      myIsDeadFlag = true;
   }
   if( mySprite.getAnimation().hasEnded() )
   {
      reset(); 

      if( wait( 1 ) )
      {  myIsDeadFlag = false;
         mySprite.collidable( true );   
      }
   }

}
//=======================================================================
//
bool Kitty::stillDirection( Direction prevDirection )
{
   switch( prevDirection )
   {
      case UP:
         myDirection = STILLUP;
         goStillUp();
         break;
      case DOWN:
         myDirection = STILLDOWN;
         goStillDown();
         break;
      case LEFT:
         myDirection = STILLLEFT;
         goStillLeft();
         break;
      case RIGHT:
         myDirection = STILLRIGHT;
         goStillRight();
         break;
      default:
         myDirection = STILL;
         assert( false );
         break;
   }
   return true;
}

//=======================================================================
//
void Kitty::reset()
{ 
   mySprite.setPosition( myStartXPos, myStartYPos ); 
   mySprite.changeAnimation( myCatStillAnim );
}

//=======================================================================
//   
void Kitty::kittyImmuneStart()
{
   mySprite.collidable( false );
   myIsImmuneFlag = true;
}

//=======================================================================
//   
void Kitty::kittyImmuneStop( int time )
{
   if ( wait( time ) )
   {
      mySprite.collidable( true );
      myIsImmuneFlag = false;
   }
}

//=======================================================================
//
bool Kitty::wait( int timer )
{
   if( !myTimer.isRunning() )
   {
      myTimer.start();   
   }

   int time = ( int( myTimer.elapsedTime() ) / 1000 );

   if( time > timer )
   {

      myTimer.stop();
      return true;
   }

   return false;
}