#pragma once

#if !defined(_GAMEINTERFACE_H_)
#define _GAMEINTERFACE_H_

#include "DxFramework/DxController.h"
#include "DxFramework/DxKeyboard.h"
#include "Bomberman/GameUI.h"

class Game;

class GameInterface
{
public:

   //========================================================================================
   //State of the game
   enum State { NONE, INIT, RUN, RESET, SHUTDOWN };

   //========================================================================================
   //Creates the game interface
   GameInterface ( GameUI& gameUI, DxKeyboard& keyboard, DxController& controller ) 
      :myGameUI(gameUI), myKeyboard(keyboard), myController(controller)
   {  
      myState = INIT; 
   }

   //========================================================================================
   //
   virtual ~GameInterface () {}

   //========================================================================================
   //Initializes the interface
   virtual bool init ( Game* window ) = 0;

   //========================================================================================
   //Runs the interface
   virtual void run ( Game* window ) = 0;

   //========================================================================================
   //Resets the interface
   virtual void reset ( Game* window ) = 0;

   //========================================================================================
   //Shutdowns the interface
   virtual void shutdown ( Game* window ) = 0;

   //========================================================================================
   //Return the state of the interface
   State state () { return myState; }

   //========================================================================================
   //Returns the passed in state of the interface
   State state ( State state ) { return (myState = state); }

protected:
   State          myState;
   GameUI&        myGameUI;
   DxKeyboard&    myKeyboard;
   DxController&  myController;
};


#endif //_GAMEINTERFACE_H_