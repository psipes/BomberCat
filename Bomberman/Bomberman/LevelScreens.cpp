#include "stdafx.h"
#include "Bomberman/LevelScreens.h"


//TODO:MAKE LEVEL 11 - 14 PICTURES!!!!
tstring LevelScreens::myLevelPicNames[19] = {"LVL01", "LVL02", "LVL03", "LVL04", "LVL05", "LVL06", "LVL07", "LVL08", "LVL09", "LVL10", "LVL11", "LVL12", "LVL13", "LVL14", "LVL15", "LVL16", "LVL17", "LVL18", "LVL19" };

//index 0 = win
//index 1 = lose
tstring LevelScreens::myWinLoseNames[2] = {"GAMEWIN", "GAMELOSE"};


//=======================================================================
//
LevelScreens::LevelScreens ()
{

}

//=======================================================================
//
LevelScreens::~LevelScreens ()
{

}

//=======================================================================
//
void LevelScreens::init ()
{
   for(int i = 0; i < SIZEOFARRAY( myLevelPicNames ); i++)
   {
      myLevelPics[i].setScale( 0.6f, 0.6f );
      myLevelPics[i].create( myLevelPicNames[i], 1.0f );
      myLevelPics[i].setPosition( 170, 30 );
      // x pos = ((screen width- (logo*scale))/2)<-isn't quite right...center of circle gets offset.
   }

   for(int i = 0; i < SIZEOFARRAY( myWinLoseNames ); i++)
   {
      myWinLosePics[i].setScale( 0.65f, 0.65f );
      myWinLosePics[i].create( myWinLoseNames[i], 10 );
      myWinLosePics[i].setPosition( 10, 30 );
      
      // x pos = ((screen width- (logo*scale))/2)<-isn't quite right...center of circle gets offset.
   }

   pressStart.setScale( 0.5f, 0.5f );
   pressStart.create("START", 1.8f);
   pressStart.setPosition(133, 500);

   //set pic to first level pic
   myCurrentPicIndex = 0;
   setCurrentLevelPic( myLevelPics[ myCurrentPicIndex ] );

   //needs to get the size of the levelpic array and compare it to the lastlevelnum
   myLastLevelNum = SIZEOFARRAY( myLevelPicNames ) - 1;

}

//=======================================================================
//
void LevelScreens::update ()
{
   myCurrentLevelPic.update();
   myWinLosePics[0].update();
   myWinLosePics[1].update();
   pressStart.update();
}

//=======================================================================
//
void LevelScreens::shutdown()
{
   myTimer.restart();
   myTimer.stop();

   for(int i = 0; i < SIZEOFARRAY( myLevelPics ); i++)
   {
      myLevelPics[i].destroy();
   }

//   myLevelPicNames.resize(0);
   pressStart.destroy();
   myCurrentLevelPic.destroy();
}

//=======================================================================
//
DxGameSprite& LevelScreens::getCurrentLevelPic( )
{
   return myCurrentLevelPic;
}

//=======================================================================
//
void LevelScreens::setCurrentLevelPic( DxGameSprite &newLevelPic )
{
   myCurrentLevelPic = newLevelPic;
}

//=======================================================================
//
void LevelScreens::incCurrentLevelPic()
{
   if( myCurrentPicIndex < SIZEOFARRAY( myLevelPicNames ) )
   {
      myCurrentPicIndex += 1;
      myCurrentLevelPic = myLevelPics[ myCurrentPicIndex ]; 
   }   
}

//=======================================================================
//
void LevelScreens::decCurrentLevelPic()
{
   if( myCurrentPicIndex > 0 )
   {
      myCurrentPicIndex -= 1;
      myCurrentLevelPic = myLevelPics[ myCurrentPicIndex ]; 
   }   
}

//=======================================================================
//
void LevelScreens::draw( IDXSPRITE spriteObj )
{
   myCurrentLevelPic.draw( spriteObj );
   pressStart.draw( spriteObj );
}

//=======================================================================
//
void LevelScreens::loadWinScreen()
{
   myCurrentLevelPic = myWinLosePics[0];
}

//=======================================================================
//
void LevelScreens::loadLoseScreen()
{
   myCurrentLevelPic = myWinLosePics[1];
}

//=======================================================================
//
void LevelScreens::resetScreens()
{
   myLevelNum = 0;
   myCurrentLevelPic = myLevelPics[0];
}




