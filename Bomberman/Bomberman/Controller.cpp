#include "stdafx.h"
#include "Bomberman/Controller.h"

//=======================================================================
//Controller constructor
Controller::Controller ( )
{
   int keyCount = 0;

}
//=======================================================================
//Controller deconstructor
Controller::~Controller ( )
{

}
//=======================================================================
//Update controller, which moves character, places bomb, or switches levels based on Keyboard or Xbox controller input
void Controller::update( Kitty& myKitty, DxKeyboard& myKeyboard, DxController& myController, Level& myLevel, GameUI& myGameUI )
{
   moveCharacter( myKitty, myKeyboard, myController );
   placeBomb( myKitty, myLevel, myKeyboard, myController );
   levelControls( myLevel, myKeyboard, myKitty, myGameUI );
}
//=======================================================================
//Moves character based on keyboard or Xbox controller input
bool Controller::moveCharacter( Kitty& myKitty, DxKeyboard myKeyboard, DxController controller )
{
   if( !myKitty.getDeathState() )
   {
      int keyCount = 0;
      if(myKeyboard.keyPressed(VK_DOWN) || controller.leftStickDown() || controller.dPadDownDown() )
      {
         myKitty.goDown();
         //goDown();
         keyCount++;
      }
      else if(myKeyboard.keyPressed(VK_LEFT) || controller.leftStickLeft() || controller.dPadLeftDown() )
      {
         keyCount++;
         myKitty.goLeft();
      }
      else if(myKeyboard.keyPressed(VK_RIGHT) || controller.leftStickRight() || controller.dPadRightDown() )
      {
         keyCount++;
         myKitty.goRight();
      }
      else if(myKeyboard.keyPressed(VK_UP) || controller.leftStickUp() || controller.dPadUpDown() )
      {
         keyCount++;
         myKitty.goUp();
      }
      // Stop all myKitty motion first, then check keyboard
      if( keyCount == 0 )
         myKitty.goStop();

   }
   return true;
}

//=======================================================================
//Places bomb based on keyboard or Xbox controller input
bool Controller::placeBomb( Kitty& kitty, Level& level, DxKeyboard myKeyboard, DxController controller )
{
   if( !kitty.getDeathState() )
   {
      if( myKeyboard.keyDown(VK_SPACE) )
      {
         level.placeBombs( kitty );

         return true;
      }
      if( buttonPressed( controller ) )
      {
            level.placeBombs( kitty );
            return true;
      }
   }
   return false;

}

//=======================================================================
//Switchs to next level based on keyboard input
void Controller::levelControls( Level& level, DxKeyboard myKeyboard, Kitty& myKitty, GameUI& myGameUI )
{
   if ( myKeyboard.keyDown( VK_NEXT ) )
   {
      level.nextLevel( myKitty );
      //level.setEnemyPositions();
      myGameUI.resetTimer();
   }
}

//=======================================================================
//Allows one bomb to be placed due to continous input from DxController A button press
bool Controller::buttonPressed( DxController controller )
{
   if ( controller.buttonADown() )
   {
      if ( buttonState != DOWN )
      {
         buttonState = DOWN;
         return true;
      }
   }
   else if ( buttonState == DOWN ) 
   {
      buttonState = UP;
   }

   return false;

}

