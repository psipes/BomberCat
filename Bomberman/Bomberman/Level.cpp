#include "stdafx.h"
#include "Bomberman/Level.h"

//=======================================================================
//
Level::Level ( DxKeyboard& keyboard, DxController& controller )
:myKeyboard(keyboard), myController(controller)
{
   //myEnemy.reserve( 10 );
}

//=======================================================================
//
Level::~Level ()
{

}

//=======================================================================
//
void Level::init( DxKeyboard& keyboard, DxController& controller )
{
   UNREFERENCED_PARAMETER( keyboard );
   UNREFERENCED_PARAMETER( controller );

   bool result = true;
   time = 0;
   myLevelNum = 0;
   dFlag = false;
   cFlag = false;
   //myEnemyCount = 3;
   myBombMaxCount = 1;
   powerupBombCount = 0;
   myCollisionManager.init();

   myWinFlag = false;

   // Sound Interface Init
   mySoundInterface = DxSound::getInterface( DxSound::InterfaceTypes::fmod );
   // set initial song
   myCurrentBGM = DxAssetManager::getInstance().getSound( _T("HAMSTERDANCE") );

   //Level init

         myLevelBgnds[0].init( _T("brick_level_one.config") );
         myLevelBgnds[1].init( _T("brick_level_two.config") );
         myLevelBgnds[2].init( _T("mushroom_level_one.config") );
         myLevelBgnds[3].init( _T("mushroom_level_two.config") );
         myLevelBgnds[4].init( _T("rock_level_one.config") );
         myLevelBgnds[5].init( _T("rock_level_two.config") );
         myLevelBgnds[6].init( _T("school_level_one.config") );
         myLevelBgnds[7].init( _T("ice_level_one.config") );
         myLevelBgnds[8].init( _T("ice_level_two.config") );
         myLevelBgnds[9].init( _T("water_level_one.config") );
         myLevelBgnds[10].init( _T("water_level_two.config") );
         myLevelBgnds[11].init( _T("space_level_one.config") );
         myLevelBgnds[12].init( _T("icecream_level_one.config") );
         myLevelBgnds[13].init( _T("icecream_level_two.config") );
         myLevelBgnds[14].init( _T("lava_level_one.config") );
         myLevelBgnds[15].init( _T("forest_level_one.config") );
         myLevelBgnds[16].init( _T("rainbow_level_two.config") );
         myLevelBgnds[17].init( _T("graveyard_level_one.config") );


   //Level Pics init
   myLevelNumPics.init();
   isLevelPicFlag = true;
   isWinLoseFlag = false;

   //Character inits
   setEnemyPositions();
}

//=======================================================================
//
void Level::update ( Kitty& kitty )
{
   // pre-render
   const int minMove = 2;

   myController.update();

   if( myKeyboard.keyDown( VK_SHIFT ) )
   {
       Bomb::increaseExplosionSize();
   }


   if( isLevelPic() == true )
   {
      myLevelNumPics.update();

      if( myKeyboard.keyDown( VK_RETURN ) || myController.buttonStartDown() )
      {
         if( isWinLoseFlag == false )
         {
            toggleLevelPicFlag();
         }

		   //play sound
         mySoundInterface->play( myCurrentBGM );
      }
   }
   else
   {
      // Objects update...
      myLevelBgnds[myLevelNum].update();
      bombUpdate( kitty );
      enemyUpdate();
      if ( myKeyboard.keyDown(VK_BACK) )
      {
         for ( unsigned int index = 0; index < myBombs.size(); index++ )
         {
            if ( myBombs[index] )
               myBombs[index]->explode();
         }
      }
   }
}
void Level::reloadLevels( int levelNum )
{
   int levels = levelNum;

      switch( levels )
      {
      case 0:
         myLevelBgnds[0].init( _T("brick_level_one.config") );
         break;
      case 1:
         myLevelBgnds[1].init( _T("brick_level_two.config") );
         break;      
      case 2:
         myLevelBgnds[2].init( _T("mushroom_level_one.config") );
         break; 
      case 3:
         myLevelBgnds[3].init( _T("mushroom_level_two.config") );
         break; 
      case 4:
         myLevelBgnds[4].init( _T("rock_level_one.config") );
         break; 
      case 5:
         myLevelBgnds[5].init( _T("rock_level_two.config") );
         break; 
      case 6:
         myLevelBgnds[6].init( _T("school_level_one.config") );
         break; 
      case 7:
         myLevelBgnds[7].init( _T("ice_level_one.config") );
         break; 
      case 8:
         myLevelBgnds[8].init( _T("ice_level_two.config") );
         break; 
      case 9:
         myLevelBgnds[9].init( _T("water_level_one.config") );
         break; 
      case 10:
         myLevelBgnds[10].init( _T("water_level_two.config") );
         break; 
      case 11:
         myLevelBgnds[11].init( _T("space_level_one.config") );
         break; 
      case 12:
         myLevelBgnds[12].init( _T("icecream_level_one.config") );
         break; 
      case 13:
         myLevelBgnds[13].init( _T("icecream_level_two.config") );
         break;
      case 14:
         myLevelBgnds[14].init( _T("lava_level_one.config") );
         break; 
      case 15:
         myLevelBgnds[15].init( _T("forest_level_one.config") );
         break;
      case 16:
         myLevelBgnds[16].init( _T("rainbow_level_two.config") );
         break; 
      case 17:
         myLevelBgnds[17].init( _T("graveyard_level_one.config") );
         break; 
      }

}
//=======================================================================
//
void Level::draw( IDXSPRITE spriteObj )
{
   if( isLevelPic() == true )
   {
      myLevelNumPics.draw( spriteObj );
   }
   else
   {
      // sprite rendering...       
      myLevelBgnds[myLevelNum].drawMySpriteMap( spriteObj );
      bombDraw( spriteObj );
      enemyDraw( spriteObj );

   }


}
//=======================================================================
void Level::placeBombs( Kitty& kitty )
{
   float width  = (float)kitty.getSprite().getWidth();
   float height = (float)kitty.getSprite().getHeight();

   // swaps null bombs to the end of the vertor
   if ( myBombs.size() > 0 )
   {
      for ( unsigned int end = (myBombs.size() - 1); end != 0; end-- )
      {
         if ( myBombs[end] )
         {
            for ( unsigned int start = 0; start < end; start++ )
            {
               if ( !myBombs[start] )
               {
                  int pStart = (int)myBombs[start],
                      pEnd = (int)myBombs[end];

  				//myBombs.push_back( temp );

                  pStart ^= pEnd;
                  pEnd ^= pStart;
                  pStart ^= pEnd;
                  myBombs[start] = (Bomb*)pStart;
                  myBombs[end]   = (Bomb*)pEnd;
               }
            }
         }
      }

      while ( myBombs.size() > 0 && !myBombs.back() )
      {
         myBombs.pop_back();
      }
   }
 
   if( cFlag == false )
   {
      if ( myBombs.size() < myBombMaxCount || myBombs.size() > 80 )
      {
         Bomb* temp = new Bomb();
         temp->init(0,0);
         temp->place( kitty.getMyPosition(), width, height );
         myBombs.push_back( temp );  
      }
   }
   else if( cFlag == true )
   {
      if( !myTimer.isRunning() )
         {
            myTimer.start();   
         }

      float time = ( myTimer.elapsedTime() / 1000.0f );
      
      if( time > 5 )
      {
         myTimer.stop();
         cFlag = false;
      }
   }
}

//=======================================================================
void Level::bombUpdate( Kitty& kitty )
{
   if( dFlag == true )
   {
      if( !myTimer.isRunning() )
      {
         powerupBombCount = myBombMaxCount;
         myTimer.start();   
      }

      myBombMaxCount = 5;
      time = time + ( myTimer.elapsedTime() / 1000.0f );
      if( wait( (int)time % 1 ) )
      { 
         placeBombs( kitty );
         if( time > 150 && myBombs.size() == myBombMaxCount )
         {
            time = 0;
            myTimer.stop();
            dFlag = false;
         }
      }
      myBombMaxCount = powerupBombCount;
   }

   for( unsigned int index = 0; index < myBombs.size(); index++ )
   {
      if ( myBombs[index] )
      {
         myBombs[index]->update();

         if ( !myBombs[index]->getSprite().isVisible() )
         {
            delete myBombs[index];
            myBombs[index] = NULL;
         }
      }
   }
}
//=======================================================================
void Level::bombDraw( IDXSPRITE spriteObj )
{
   for( unsigned int index = 0; index < myBombs.size(); index++ )
   {
      if ( myBombs[index])
      myBombs[index]->draw( spriteObj );
   }
}
//=======================================================================
void Level::shutdown()
{
   myController.shutdown();
   myCollisionManager.shutdown();

   for( unsigned int i = 0; i < myEnemy.size(); i++)
   {
      myEnemy[i].shutdown();
   }
   myEnemy.clear();

   refGameUI.destroy();
   myLevelNumPics.shutdown();

   for(unsigned int i = 0; i < myBombs.size(); i++)
   {
      if ( myBombs[i] )
      {
         myBombs[i]->shutdown();
         delete myBombs[i];
         myBombs[i] = NULL;
      }
   }
   myBombs.clear();

   for(int i = 0; i < SIZEOFARRAY(myLevelBgnds); i++)
   {
      myLevelBgnds[i].shutdown();
   }

   myTimer.restart();
   myTimer.stop();
   myGameTitle.clear();
}

//=======================================================================
//
void Level::resetGame()
{
   time = 0;
   dFlag = false;
   cFlag = false;
   myBombMaxCount = 1;
   powerupBombCount = 0;
   myWinFlag = false;
   isLevelPicFlag = true;
   isWinLoseFlag = false;
   myEnemy.clear();
   for ( unsigned int i = 0; i < myBombs.size(); i++ )
   {
      delete myBombs[i];
      myBombs[i] = NULL;
   }
   myEnemy.clear();
   myLevelBgnds[myLevelNum].shutdown();
   
   while( myLevelNum > 0 )
   {
      reloadLevels( myLevelNum );
      myLevelNum--;
   }

   //Character inits
   setEnemyPositions();
}

//=======================================================================
//
void Level::enemyUpdate()
{
   for( unsigned int i = 0; i < myEnemy.size(); i++ )
   {
      myEnemy[i].update();

      if( myEnemy[i].getEnemyType() == "ENEGA" && !myEnemy[i].getisDeadFlag()  )
      {
         if( wait( 10 ) )
         {
            negaPlaceBombs();
         }
      }
      
   }   
}

//=======================================================================
//
void Level::enemyDraw( IDXSPRITE spriteObj )
{
   for( unsigned int i = 0; i < myEnemy.size(); i++ )
   {
      myEnemy[i].draw( spriteObj );
   }   
}

//=======================================================================
//
void Level::Collisions( Kitty& myKitty, GameUI& refGameUI )
{
   TiledBackground&  levelRef = myLevelBgnds[myLevelNum];

   //Kitty vs. enemy collisions
   if( !myKitty.getDeathState() )
   {
      for(size_t i = 0; i< myEnemy.size(); i++)
      {
         if( myCollisionManager.spriteCollsions( myKitty.getSprite(), myEnemy[i].getSprite() ) )
         {
            myKitty.goStop();
            myKitty.ToggleDeath( true );
            myKitty.loseLife();
         } 
      }
   }
   //enemy vs world collisions
   for(size_t i = 0; i< myEnemy.size(); i++)
   {
      if( myCollisionManager.worldCollisions( myEnemy[i].getSprite(), levelRef ) )
      {
         myEnemy[i].stop();          
         D3DXVECTOR3 snEPos = myEnemy[i].getLastPosition();
         myEnemy[i].setMyPosition( snEPos );
         myEnemy[i].changeDirection();
      }  
   }  
   
   //kitty vs. world collisions
   if( myCollisionManager.worldCollisions( myKitty.getSprite(), levelRef ) )
   {
      myKitty.goStop();
      D3DXVECTOR3 snPos = myKitty.getLastPosition();
      myKitty.setMyPosition( snPos );
   }

   //powerup collisions
   for ( unsigned int index = 0; index < levelRef.myPowerups.size(); index++  )
   {
      if ( myCollisionManager.spriteCollsions( myKitty.getSprite(), levelRef.myPowerups[index] ) )
      {
         myKitty.power( levelRef.myPowerups[index].power() );
         powers( myKitty, refGameUI );
         if ( index != levelRef.myPowerups.size() - 1 )
         {
            levelRef.myPowerups[index] = levelRef.myPowerups.back();
         }
         levelRef.myPowerups.pop_back();
         //levelRef.myPowerups[index].toggleVisible();
         //levelRef.myPowerups[index].destroy();
      }
   }

   //door collisions
   if( myCollisionManager.doorCollisions( myKitty.getSprite(), levelRef ) )
   {
      nextLevel( myKitty );
      refGameUI.resetTimer();
   }

   //Bomb Collisions
   for ( unsigned int index = 0; index < myBombs.size(); index++ ) 
   {
      if ( !myBombs[index] || !myBombs[index]->getIsBombPlaced() )
         continue;

      Bomb& bomb = *myBombs[index];
      if ( bomb.getIsExploding() )
      {
         bomb.resolveCollision( levelRef );

         for ( unsigned int j = 0; j < myEnemy.size(); j++ )
         {
            if ( bomb.collision( myEnemy[j].getSprite() ) && !bomb.getEnemyBombFlag() )
            {
               myEnemy[j].isEnemyDead( true );
               myEnemy[j].getSprite().collidable( false);
               refGameUI.addPoints( myEnemy[j].getPointValue() );
            }
         }

         if ( bomb.collision( myKitty.getSprite() ) )
         {
            myKitty.ToggleDeath( true );
            refGameUI.subPoints( refGameUI.getPoints() / 10 );
            myKitty.loseLife();
         }

         for ( unsigned int j = 0; j < myBombs.size(); j++ )
         {
            if ( myBombs[j] && myBombs[j] != &bomb && !myBombs[j]->getIsExploding() )
            {
               if ( bomb.collision( myBombs[j]->getSprite() ) )
               {
                  myBombs[j]->explode();
               }
            }
         }
      }
   }
}
//=======================================================================
void Level::powers( Kitty& kitty, GameUI& game )
{
   Powerup::Power power = kitty.power();
   switch( power )
   {
      case Powerup::POWER_LIFE:
         kitty.gainLife();
         break;
      case Powerup::POWER_PASSTHRU:
         kitty.kittyImmuneStart();
         break;
      case Powerup::POWER_MULTIBOMB:
         myBombMaxCount++;
         break;
      case Powerup::POWER_FIREUP:
         Bomb::increaseExplosionSize();
         break;
      case Powerup::POWER_FIREDOWN:
         Bomb::decreaseExplosionSize();
         break;
      case Powerup::POWER_SPEED:
         if( kitty.checkSpeed() )
            kitty.changeSpeed();
         break;
      case Powerup::POWER_TIME:
         game.addTime(30);
         break;
      case Powerup::POWER_COOKIE:
         game.addPoints(125);
         break;
      case Powerup::POWER_TOYMOUSE:
         game.addPoints(250);
         break;
      case Powerup::POWER_YARN:
         game.addPoints(500);
         break;
      case Powerup::POWER_POOP:
      {
         int flag = rand() % 2;

         if( flag == 0 )
         {
            dFlag = true;
         }
         else if( flag == 1 )
         {
            cFlag = true;
         }
      }
      default:
         break;
   }
}
//=======================================================================
//
bool Level::wait( int timer )
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


//=======================================================================
//
void Level::restartLevel( Kitty& myKitty )
{
   toggleLevelPicFlag();

   if( myLevelNum < SIZEOFARRAY(myLevelBgnds) - 1 )
   {
      myKitty.goStop();

      for(size_t i = 0; i < myEnemy.size(); i++)
      {
         myEnemy[i].stop();      
      }

      for ( unsigned int i = 0; i < myBombs.size(); i++ )
      {
         delete myBombs[i];
         myBombs[i] = NULL;
      }

      for ( unsigned int i = 0; i < myLevelBgnds[myLevelNum].mySpriteMap.size(); i++ )
      {
         myLevelBgnds[myLevelNum].mySpriteMap[i].tileSprite.collidable(true);
      }
      myLevelBgnds[myLevelNum].reset();
      //myEnemyCount = 5;
      myKitty.reset();
      setEnemyPositions(); 

      changeSong();
   }
}

//=======================================================================
//
void Level::nextLevel( Kitty& myKitty )
{

   myLevelNumPics.incCurrentLevelPic();
   toggleLevelPicFlag();
   

   if( myLevelNum < SIZEOFARRAY(myLevelBgnds) - 1 )
   {
      myKitty.goStop();

      for(size_t i = 0; i < myEnemy.size(); i++)
      {
         myEnemy[i].stop();      
      }

      for ( unsigned int i = 0; i < myBombs.size(); i++ )
      {
         delete myBombs[i];
         myBombs[i] = NULL;
      }
      myEnemy.clear();
      myLevelBgnds[myLevelNum].shutdown();
      //myEnemyCount = 5;
      myLevelNum++;
      myKitty.reset();
      setEnemyPositions();

      changeSong();

   }
   else if ( myLevelNum == SIZEOFARRAY(myLevelBgnds) - 1 )
   {
      // load win screen
      myWinFlag = true;
   }
}

//=======================================================================
//
bool Level::isLevelPic()
{
   return isLevelPicFlag;
}

//=======================================================================
//
void Level::toggleLevelPicFlag( )
{
   //if( myLevelNum != myCurrnetLevelNum )
   if( isLevelPic() == true )
   {
      isLevelPicFlag = false;
   }
   else
   {
      isLevelPicFlag = true;
   }
}

//=======================================================================
//
void Level::setEnemyPositions()
{
   myEnemy.clear();

   TiledBackground&  levelRef = myLevelBgnds[myLevelNum];
   
   for (unsigned int index = 0; index < levelRef.mySpriteMap.size(); index++)
   {
      if (levelRef.mySpriteMap[index].enemyFlag == true)
      {
         Enemy enemy; 
         enemy.init( levelRef.mySpriteMap[index].xPos, 
                     levelRef.mySpriteMap[index].yPos, 
                     levelRef.getEnemyType(), 
                     levelRef.getEnemySpeed(),
                     levelRef.getEnemyScale());

         myEnemy.push_back( enemy );




         //////////////////////// W.T.B.F ///////////////////////////
         //todo
         levelRef.mySpriteMap[index].enemyFlag = false;
         
      }
   }
}

//=======================================================================
//
void Level::negaPlaceBombs()
{
   for( size_t i = 0; i < myEnemy.size(); i++)
   {
      if ( !myEnemy[i].getisDeadFlag() )
      {
         float width  = (float)myEnemy[i].getSprite().getWidth();
         float height = (float)myEnemy[i].getSprite().getHeight();         

         Bomb* temp = new Bomb();
         temp->init(0,0);
         temp->setEnemyBombFlag( true );
         temp->place( myEnemy[i].getMyPosition(), width, height );

         myBombs.push_back( temp );
      }
   }
   
}

//=======================================================================
//
void Level::loadGameOverScreen() // never called
{
   if( isLevelPic() == false && isWinLoseFlag == false )
   {
      mySoundInterface->stop( myCurrentBGM );
      isWinLoseFlag = true;
      myLevelNumPics.loadLoseScreen();
      toggleLevelPicFlag();
   }
}

//=======================================================================
//
void Level::loadWinScreen() // never called
{
   if( isLevelPic() == false && isWinLoseFlag == false )
   {
      mySoundInterface->stop( myCurrentBGM );
      isWinLoseFlag = true;
      myLevelNumPics.loadWinScreen();
      toggleLevelPicFlag();
   }
}

int Level::getSizeOfLevelArray()
{
   return SIZEOFARRAY(myLevelBgnds);
}

bool Level::getWinLoseFlag()
{
   return isWinLoseFlag;
}

void Level::setWinLoseFlag( bool newFlagValue )
{
   isWinLoseFlag = newFlagValue;
}

void Level::changeSong ()
{
   // stop current song
   mySoundInterface->stop( myCurrentBGM );
   // pick background song for next level
   myCurrentBGM = NULL;
   switch ( myLevelNum )
   {
   // normal level
   case 0:
   case 1:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("HAMSTERDANCE") );
      break;

   // mushroom level
   case 2:
   case 3:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("FORESTREALMREVERSED") );
      break;

   // rock level
   case 4:
   case 5:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("ROCK") );
      break;

   // classroom level
   case 6:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("MOUNTAINKING") );
      break;

   // snow level
   case 7:
   case 8:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("SNOW") );
      break;

   // water level
   case 9:
   case 10:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("SEA") );
      break;

   // Space level
   case 11:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("SPACE") );
      break;

   //icecream Level
   case 12:
   case 13:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("ICECREAMSONG") );
      break;

   // lava level
   case 14:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("VOLCANO") );
      break;

   //Forest Level
   case 15:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("FORESTREALM") );
      break;

   // Rainbow level
   case 16:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("HAMSTERDANCEMINOR") );
      break;

   //graveyard level
   case 17:
      myCurrentBGM = DxAssetManager::getInstance().getSound( _T("HORROR") );
      break;

   default:
      break;
   }
}

void Level::stopSong ()
{
   mySoundInterface->stop( myCurrentBGM );
   myCurrentBGM = NULL;
}