#include "stdafx.h"
#include "Bomb.h"

float Bomb::myExplLength = 4.0f;
//========================================================================================
//Constructor
Bomb::Bomb()
{
   myTimeToDelay = 1;
   myBombCount = 10;
   myBombExploding = false;
   myIsEnemyBombFlag = false;
   myFirstTimeAnimFlag = true;
   myFirstTimeSoundFlag = true;
   myIsBombSet = false;
   myBombState = UNSET;
   myExplSize = 0.0f;
   myBmbFSpeed = 10.0f;
   myExpFSpeed = 10.0f;
   myExplosionScale = 1;
   myScale = 0.2f;
	myPosition.x = myPosition.y = myPosition.z = 0;
	myLastPosition.x = myLastPosition.y = myLastPosition.z = 0;
	myPowerupChance = 3;
}
//========================================================================================
//Deconstructor
Bomb::~Bomb()
{ 
   shutdown();
}
//========================================================================================
//Initializes Bomb, sets initial vector size for bomb explosions, sets scale for bomb and explosion sprites
bool Bomb::init( int xPos, int yPos )
{
   if ( mySound )
      mySoundInterface = DxSound::getInterface( DxSound::fmod );
   loadCharacterSounds();
   mySprite.setScale( 0.2f, 0.2f );
   myExplosions.resize( 4 );
   bool result = mySprite.create( "BBOMB-STATIC" );
   assert(result);
   myPosition.x = float(xPos);
   myPosition.y = float(yPos);
   mySprite.collidable( false );
   mySprite.setPosition( myPosition.x, myPosition.y );

   myExplosionScale = (0.03125f * myExplLength);
   myExplosions[0].setScale( myScale, myExplosionScale );
   bool result0 = myExplosions[0].create( "BBOMB-STATIC" );
   assert(result0);

   myExplosions[1].setScale( myScale, -myExplosionScale );
   bool result1 = myExplosions[1].create( "BBOMB-STATIC" );
   assert(result1);

   myExplosions[2].setRotation(D3DXToRadian(90));
   myExplosions[2].setScale( myScale, -myExplosionScale );
   bool result2 = myExplosions[2].create( "BBOMB-STATIC" );
   assert(result2);

   myExplosions[3].setRotation(D3DXToRadian(90));
   myExplosions[3].setScale( myScale, myExplosionScale );
   bool result3 = myExplosions[3].create( "BBOMB-STATIC" );
   assert(result3);
 
   return result;
}
//========================================================================================
//updates the sprites and timer that changes the state of the bomb depending on the timer
void Bomb::update()
{
   mySprite.update();

   if( !myTimer.isRunning() )
   {
      myTimer.start();   
   }

   float time = ( myTimer.elapsedTime() / 1000.0f );
   int doubleCheck = (int)time * 2;

   if ( time > myTimeToDelay )
   {
      if ( myBombState == UNSET )
      {
         selectAnimation( SET );
         myTimer.restart();
      }
      else if ( myBombState == SET )
      {
         selectAnimation( LIGHT );
         myTimer.restart();
      }
      else if ( myBombState == LIGHT )
      {
         selectAnimation( FLASH );
         myTimer.restart();
      }
      else if ( myBombState == FLASH )
      {         
         selectAnimation( EXPLODE );
         myBombExploding = true;
         if ( mySound )
            mySoundInterface->play( mySound );
         myTimer.restart();
      }
   }

   if ( myBombState == EXPLODE && explosionHasEnded() && time > 1.5f )
   {
      if ( mySound )
         mySoundInterface->stop( mySound );
      myTimer.stop();
      reset();
      selectAnimation( UNSET );
      myTimer.restart();
      myIsBombSet = false;
   }

   for( size_t i = 0; i < myExplosions.size(); i++)
   {
      myExplosions[i].update();

      if ( !myExplFlag[i] )
         myExplosions[i].getAnimation().stop();
   }
   grow();
}
//========================================================================================
//Destroys Bomb
void Bomb::shutdown()
{
   if ( mySprite )
   {
      mySprite.destroy();
      for( unsigned int i = 0; i < myExplosions.size(); i++)
      {
        myExplosions[i].destroy();
      } 
   }

   myMaxExpLength = 0.0f;
   myBmbFSpeed = 0.0f;
   myExpFSpeed = 0.0f;
   myScale = 0.0f;
   myExplSize = 0.0f;
   myBombCount = 0;
   myPowerupChance = 0;
   myExplosionScale = 0.0f;
   myTimeToDelay = 0.0f;
   myBombExploding = false;
   myFirstTimeSoundFlag = false;
   myIsBombSet = false;
   myIsEnemyBombFlag = false;
   myDownExpl = Rect(0, 0, 0, 0);
   myUpExpl = Rect(0, 0, 0, 0);
   myRightExpl = Rect(0, 0, 0, 0);
   myLeftExpl = Rect(0, 0, 0, 0);

   for(int i = 0; i < SIZEOFARRAY( myExplFlag ); i++)
   {
      myExplFlag[i] = false;
   }

   myExplosions.clear();
   myExplosions.resize(0);
   myTimer.restart();
   myTimer.stop();
   myPosition = D3DXVECTOR3(0, 0, 0);
   myLastPosition = D3DXVECTOR3(0, 0, 0);
   myCollisionManager.shutdown();

}
//========================================================================================
//Loads explosion sound
void Bomb::loadCharacterSounds()
{
   if( myFirstTimeSoundFlag )
   {
      //Change firstTimeFlag
      myFirstTimeSoundFlag = false;
      mySound = DxAssetManager::getInstance().getSound( "EXPLOSION" );
      //mySoundInterface->load( _T("Assets\\Explosion.wav"), mySound );
   }
}
//========================================================================================
// Draws a bomb if the bomb has been set also draws the four other bomb sprites when it explodes 
void Bomb::draw ( IDXSPRITE spriteObj )
{  
   if( myIsBombSet )
   {
      mySprite.draw( spriteObj );
  
      if( myBombState == EXPLODE )
      {
         for( unsigned int i = 0; i < myExplosions.size(); i++)
         {
            myExplosions[i].draw( spriteObj );
         }
      }
   }
}
//========================================================================================
// used to place the bomb and only allow you to sets a bomb if a bomb had not already been set
// also sets the position for the bomb to be placed and it four points of explosions
void Bomb::place( D3DXVECTOR3& position, float kittyWidth, float kittyHeight )
{
   if( !myIsBombSet )
   {       
      myPosition = position;
      myPosition.x = float( ( (int)( myPosition.x + kittyWidth/2 ) - ( (int)( myPosition.x + kittyWidth/2 ) % 32) ) + 3 );
      myPosition.y = float( ( (int)( myPosition.y + kittyHeight/2 ) - ( (int)( myPosition.y + kittyHeight/2 ) % 32) ) + 3 );
      mySprite.setPosition( myPosition.x, myPosition.y );
      myIsBombSet = true;
   }
}
//========================================================================================
// Changes the state and animation of the bamb Based on the timer 
void Bomb::selectAnimation( BombState state )
{
   if( state != myBombState )
   {
      switch ( state )
      {
      case SET:
         mySprite.changeAnimation( "BBOMB-STATIC", myBmbFSpeed, D3DCOLOR_XRGB( 170, 181, 129 ) );
         myBombState = SET;
         break;
      case LIGHT:
         mySprite.changeAnimation( "BBOMB-LIGHT", myBmbFSpeed, D3DCOLOR_XRGB( 170, 181, 129 ) );
         mySprite.collidable(true);
         myBombState = LIGHT;
         break;
      case FLASH:
         mySprite.changeAnimation( "BBOMB-FLASH", myBmbFSpeed, D3DCOLOR_XRGB( 170, 181, 129 ) );
         myBombState = FLASH;
         break;
      case EXPLODE:
         myExpFSpeed *= 5;
         mySprite.changeAnimation( "BBOMB-EXPLCENTER", myExpFSpeed, D3DCOLOR_XRGB( 170, 181, 129 ) );
         onExplode();
         myBombState = EXPLODE;
         break;
      default:
         myBombState = UNSET;
         mySprite.changeAnimation( "BBOMB-STATIC", myBmbFSpeed, D3DCOLOR_XRGB( 170, 181, 129 ) );
         break;
      }
   }
}

//========================================================================================
//Selects the bomb explosion animation and plays sound once bomb timer allows explosion
void Bomb::explode()
{
   selectAnimation( EXPLODE );
   myBombExploding = true;
   if ( mySound )
      mySoundInterface->play( mySound );
   myTimer.restart();
}

//========================================================================================
//Initializes the pillars of explosion fire from the bomb positions by setting the RECT based on 
//the bomb position
void Bomb::initPillars()
{
   //Bottom pillar 
   myDownExpl.left = (LONG)myPosition.x;
   myDownExpl.top =  (LONG)(myPosition.y + (float)mySprite.getHeight());
   myDownExpl.right = (LONG)(myPosition.x + (float)mySprite.getWidth());
   myDownExpl.bottom = (LONG)(myPosition.y + (float)mySprite.getHeight());

   //top pillar   
   myUpExpl.left = (LONG)myPosition.x;
   myUpExpl.top = (LONG)myPosition.y;
   myUpExpl.right = (LONG)(myPosition.x + (float)mySprite.getWidth());
   myUpExpl.bottom = (LONG)(myPosition.y);

   //right pillar        ;
   myRightExpl.left = (LONG)(myPosition.x + (float)mySprite.getWidth());
   myRightExpl.top = (LONG)(myPosition.y);
   myRightExpl.right = (LONG)(myPosition.x + (float)mySprite.getWidth());
   myRightExpl.bottom = (LONG)(myPosition.y + (float)mySprite.getHeight());


   //left pillar;
   myLeftExpl.left = (LONG)myPosition.x;
   myLeftExpl.top = (LONG)myPosition.y;
   myLeftExpl.right = (LONG)myPosition.x;
   myLeftExpl.bottom = (LONG)(myPosition.y + (float)mySprite.getHeight());


}
//========================================================================================
//Once bomb is set to explode, sets explosion state to true for each direction, initializes explosion pillars, 
//sets position of explosion sprites, and sets the animation of the explosion to the initial pillar flame
void Bomb::onExplode()
{ 
   myExplFlag[0] = true;
   myExplFlag[1] = true;
   myExplFlag[2] = true;
   myExplFlag[3] = true;

   initPillars();
   //Bottom pillar
   myExplosions[0].setPosition( (float)myDownExpl.left, (float)myDownExpl.top );
   //top pillar
   myExplosions[1].setPosition( (float)myPosition.x, (float)myPosition.y  );
   //right pillar
   myExplosions[2].setPosition( (float)myRightExpl.left, (float)myRightExpl.top  );
   //left pillar
   myExplosions[3].setPosition( (float)myPosition.x, (float)myPosition.y );

   //Bottom pillar
   myExplosions[0].changeAnimation( "BOOM-DOWN",  myExpFSpeed, D3DCOLOR_XRGB( 170, 181, 129 )  );
   //top pillar
   myExplosions[1].changeAnimation( "BOOM-DOWN",    myExpFSpeed, D3DCOLOR_XRGB( 170, 181, 129 )  );
   //right pillar
   myExplosions[2].changeAnimation( "BOOM-DOWN", myExpFSpeed, D3DCOLOR_XRGB( 170, 181, 129 )  );
   //left pillar
   myExplosions[3].changeAnimation( "BOOM-DOWN",  myExpFSpeed, D3DCOLOR_XRGB( 170, 181, 129 )  );
}
//========================================================================================
//Grows the explosion pillars in each direction and changes the collision area as each pillar grows
void Bomb::grow()
{
   if( myBombState == EXPLODE )
   {     
      myDownExpl.bottom += long( myExplLength );
      myUpExpl.top      -= long( myExplLength );
      myRightExpl.right += long( myExplLength );
      myLeftExpl.left   -= long( myExplLength );
      
      float maxWidth = myExplLength * getSprite().getWidth();
      float maxHeight = myExplLength * getSprite().getHeight();
      int randomW = rand() % (int)maxWidth + getSprite().getWidth();
      int randomH = rand() % (int)maxHeight + getSprite().getHeight();
      myExplFlag[0] = myDownExpl.height() < (randomH) && myExplFlag[0];
      randomH = rand() % (int)maxHeight + getSprite().getHeight();
      myExplFlag[1] = myUpExpl.height()   < (randomH) && myExplFlag[1];
      myExplFlag[2] = myRightExpl.width() < (randomW) && myExplFlag[2];
      randomW = rand() % (int)maxWidth + getSprite().getWidth();
      myExplFlag[3] = myLeftExpl.width()  < (randomW) && myExplFlag[3];

      if ( myExplFlag[0] )
      {
         myExplosions[0].setCollisionArea( myDownExpl );
      }
      if ( myExplFlag[1] )
      {
         myExplosions[1].setCollisionArea( myUpExpl );
      }
      if ( myExplFlag[2] )
      {
         myExplosions[2].setCollisionArea( myRightExpl );
      }
      if ( myExplFlag[3] )
      {
         myExplosions[3].setCollisionArea( myLeftExpl );
      }
   }
}

//========================================================================================
//Resets bomb to initial state
void Bomb::reset()
{
   myIsBombSet = false;
   myBombExploding = false;
   mySprite.collidable( false );
   mySprite.toggleVisible();
   myExplSize = 0.0f;

   myExplosions[0].collidable( false );
   myExplosions[1].collidable( false );
   myExplosions[2].collidable( false );
   myExplosions[3].collidable( false );      
}

//========================================================================================
//Checks if explosion has ended
bool Bomb::explosionHasEnded ()
{
   return !(myExplFlag[0] || myExplFlag[1] ||
            myExplFlag[2] || myExplFlag[3]);
}

//========================================================================================
//Checks the size of the growth size of the pillar and the direction it is headed then finds
//if there are any destroyable or non-destroyable objects in the path.  
void Bomb::resolveCollision ( TiledBackground& level )
{
   int index = 0;
   for ( unsigned int i = 0; i < myExplosions.size(); i++  )
   {
      if ( myCollisionManager.worldCollisions( myExplosions[i], level, &index ) )
      {
         if ( level.mySpriteMap[index].tileSprite.getDestroyable() )
         {
            if ( level.mySpriteMap[index].doorFlag == true )
            {
               level.mySpriteMap[index].tileSprite.setDestroyable( false );
               level.mySpriteMap[index].tileSprite.changeAnimation ( "DOOR", 10, D3DCOLOR_XRGB( 170, 181, 129 ) );
               level.mySpriteMap[index].tileSprite.collidable( false );
            }
            else
            {
               if ( rand() % myPowerupChance == 0 )
               {
                  Powerup powerup;
                  powerup.setScale( .25f, .25f );
                  powerup.create( (Powerup::Power)(rand() % (Powerup::Power::POWER_YARN + 1)), 
                                  (float)level.mySpriteMap[index].xPos, (float)level.mySpriteMap[index].yPos );
                  
                  level.myPowerups.push_back( powerup );
               }
               level.mySpriteMap[index].tileSprite.setDestroyable( false );
               level.mySpriteMap[index].tileSprite.getAnimation().play();
               level.mySpriteMap[index].tileSprite.getAnimation().speed(10.0f);
               level.mySpriteMap[index].tileSprite.collidable( false );
            }
            level.mySpriteMap[index].brickFlag = false;
         }
         myExplFlag[i] = false;
      }
   }
}


//========================================================================================
//Checks collision of explosion sprite with world
bool Bomb::collision ( DxGameSprite& sprite )
{
   for ( unsigned int i = 0; i < myExplosions.size(); i++  )
   {
      if ( myCollisionManager.spriteCollsions( myExplosions[i], sprite ) )
         return true;
   }
   return false;
}