#pragma once

#if !defined(_CONTROLLER_H_)
#define _CONTROLLER_H_

#include "DxFramework/DxFramework.h"
#include "DxFramework/DxKeyboard.h"
#include "DxFramework/DxController.h"
#include "Bomberman/Bomb.h"
#include "Bomberman/Kitty.h"
#include "Bomberman/Level.h"
#include "Bomberman/GameUI.h"


   //========================================================================================
   //
class Controller
{

public:

   //========================================================================================
   //State for the controller
   enum AButtonState
   {      
      UP,
      DOWN
   };

   AButtonState buttonState;

//constructers
public:

   //========================================================================================
   //
   Controller();

   //========================================================================================
   //
   ~Controller();


public:

   //========================================================================================
   //Updates the controller
   void update( Kitty& myKitty, DxKeyboard& myKeyboard, DxController& myController, Level& myLevel, GameUI& myGameUI );

   //========================================================================================
   //Moves the character
   bool moveCharacter( Kitty& myKitty, DxKeyboard keyboard, DxController controller );

   //========================================================================================
   //Places bomb
   bool placeBomb( Kitty& kitty, Level& level, DxKeyboard keyboard, DxController controller );

   //========================================================================================
   //Handles the controls for level progression
   void levelControls( Level& level, DxKeyboard myKeyboard, Kitty& myKitty, GameUI& myGameUI );

   //========================================================================================
   //Returns true if a button is pressed
   bool buttonPressed( DxController controller );
};

#endif