#include "stdafx.h"
#include "Bomberman/Enemy.h"
#include <cassert>

//Check on device
//=======================================================================
//Enemy constructor
Enemy::Enemy ( )
{
    myFirstTimeFlag = true;
    myIsDyingFlag = false;
    myIsDeadFlag = false;
    myIsNotMoveingFlag = false;
    mySpeed = 0.0F;
    myPosition.x = myPosition.y = myPosition.z = 0;
    myLastPosition.x = myLastPosition.y = myLastPosition.z = 0;
    myPointValue = 125;
    
}
//=======================================================================
//Enemy deconstructor
Enemy::~Enemy ( )
{
    
}

//=======================================================================
//Intializes Enemy, takes in type, position, speed, and scale
bool Enemy::init(int xPos, int yPos, tstring eType, float eSpeed, float eScale )
{
   //**if enemy is penguin, the scale can be changed, if wanted
   //scale of .4 would make it 32x32. at a scale of .25, they're 20 x 20,
   //wheras other enemies are 20 x27
   myEnemyType = eType;
   mySprite.setScale( eScale, eScale );
   bool result = mySprite.create( (eType + "-DOWN"), 10 );
   assert(result);
   mySpeed = eSpeed;
   mySprite.setPosition( float(xPos), float(yPos) );
   myPosition.x = mySprite.getXPosition();
   myPosition.y = mySprite.getYPosition();
   mySprite.setXVel(mySpeed - mySpeed);
   mySprite.setYVel(mySpeed);
   myIsNotMoveingFlag = false;
   mySprite.collidable( true );

   return true;
}
//=======================================================================
//Updates Enemy position, and updates corresponding sprite.  Checks if enemy
//is dead or dying to send to die function for death animation.  Also checks
//flag to randomly change enemy walking direction
void Enemy::update()
{
   myLastPosition.x = myPosition.x;
   myLastPosition.y = myPosition.y;

   mySprite.update();

   myPosition.x = mySprite.getXPosition();
   myPosition.y = mySprite.getYPosition();

   if( myIsDyingFlag || myIsDeadFlag )
   {
      died();
   }
   if( !myIsNotMoveingFlag )
   {
      if( wait( 1 ) )
         changeDirection();
   }

}

//=======================================================================
//Draws enemy sprite
void Enemy::shutdown ( )
{
   myPosition = D3DXVECTOR3(0, 0, 0);
   myLastPosition = D3DXVECTOR3(0, 0, 0);

   mySprite.destroy();
   mySpeed = 0.0f;
   myTimer.restart();
   myTimer.stop();
   myEnemyType.clear();
   myPointValue = 0;;

   myFirstTimeFlag = false;
   myIsDyingFlag = false;
   myIsDeadFlag = false;
   myIsNotMoveingFlag = false;

}

//=======================================================================
//
bool Enemy::draw ( IDXSPRITE spriteObj )
{
   if( !myIsDeadFlag )
   {
      mySprite.draw( spriteObj );
   }

   return true;
}
//=======================================================================
//Changes animation to death animation for enemy and flags that enemy is dead
void Enemy::died()
{
   mySprite.setXVel(0.0f);
   mySprite.setYVel(0.0f);
   if( myIsDyingFlag )
   {  
      mySprite.collidable ( false);
      mySprite.changeAnimation( ( myEnemyType + "-DEAD"), 5, D3DCOLOR_XRGB( 170, 181, 129 )  );
      myIsDyingFlag = false;
      myIsNotMoveingFlag = true;
   }
   if( !mySprite.getAnimation().isPlaying() )
   { 
      myIsDeadFlag = true;
   }
}
//=======================================================================
//Stops enemy sprite movement
bool Enemy::stop ()
{    
    // Stop actual sprite motion.
    mySprite.setXVel(0.0f);
    mySprite.setYVel(0.0f);

    return true;
}
//=======================================================================
//Randomly generates direction of enemy
void Enemy::changeDirection()
{
   int ranNum = rand() % 4 + 1;

   switch( ranNum )
   {
   case 1:
      //DOWN
      mySprite.changeAnimation(  ( myEnemyType + "-DOWN" ), 10, D3DCOLOR_XRGB( 170, 181, 129 )  );
      mySprite.setXVel( mySpeed - mySpeed );
      mySprite.setYVel( mySpeed );
      break;
   case 2:
      //UP
      mySprite.changeAnimation(   ( myEnemyType + "-UP"), 10, D3DCOLOR_XRGB( 170, 181, 129 )  );
      mySprite.setXVel( mySpeed - mySpeed );
      mySprite.setYVel( -mySpeed);
      break;
   case 3:
      //Right
      mySprite.changeAnimation( ( myEnemyType + "-RIGHT"), 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
      mySprite.setXVel( mySpeed );
      mySprite.setYVel( mySpeed - mySpeed );
      break;
   case 4:
      //LEFT
      mySprite.changeAnimation(  ( myEnemyType + "-LEFT"), 10, D3DCOLOR_XRGB( 170, 181, 129 )  );
      mySprite.setXVel( -mySpeed );
      mySprite.setYVel( mySpeed - mySpeed );
      break;
   }
}
//=================================================================================================================================
//Allows NegaKitty to place bombs (currently not used)
bool Enemy::negaKittyPlaceBomb(int time)
{
   if( wait( time ) )
   {
      return true;
   }

   return false;
}
//=================================================================================================================================
//Wait function to check timer versus enemy timer to stop timer for enemy pause
bool Enemy::wait( int timer )
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
