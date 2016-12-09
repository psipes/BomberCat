
#pragma once
#if !defined(_BOMBERMAN_H_)
#define _BOMBERMAN_H_

#include "Resource.h"

#include <map>

#include "DxFramework/DxGameSprite.h"
#include "DxFramework/DxKeyboard.h"
#include "DxFramework/DxController.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxSound.h"
#include "DxFramework/DxSoundIdentifier.h"
#include "DxFramework/DxText.h"
#include "Bomberman/GameUI.h"
#include "Bomberman/Level.h"
#include "Bomberman/Controller.h"
#include "Bomberman/GameInterface.h"


 

//========================================================================================
//
class Game : public DxFramework
{
public:

   //========================================================================================
   //
   Game ();

   //========================================================================================
   //
   ~Game ();

   //========================================================================================
   //Loads the game interface which controls the games state
   void loadInterface ( tstring name, GameInterface::State state );


protected:

   //========================================================================================
   //Runs the high score screen
   void highScoreGameRun ();

   //========================================================================================
   //Initializes the game
   bool gameInit ();

   //========================================================================================
   //Runs/Updates the game
   void gameRun ();

   //========================================================================================
   //Closes the game
   void gameExit ();

   //========================================================================================
   //Windows virtual interface
   //Removes window frame
   LONG removeWindowStyle ( ) { return WS_THICKFRAME | WS_MAXIMIZEBOX; }

   //========================================================================================
   //Returns the small icon
   UINT winResrcIdSmallIcon ( void ) { return IDI_SMALL; } 

   //========================================================================================
   //Returns default width of 800
   int winScreenWidth() const { return 800; }
   

   //========================================================================================
   //Returns default height of 608
   int winScreenHeight() const { return 608; }

   //========================================================================================
   //Todo
   BOOL enableAutoDepthStencil ( ) {  return true;  }

   //========================================================================================
   //Todo
   D3DFORMAT autoDepthStencilFormat ( )  {  return D3DFMT_D16;  }

//Member variables
private:
   std::map<tstring, GameInterface*> myInterfaces;

   tstring           myGameTitle;
   tstring           myCurrentInterfaceName;

   D3DCOLOR          bgColor;

   DxKeyboard        myKeyboard;
   DxController      myController;
   DxSound*          mySoundInterface;

   GameUI            myGameUI;

};

#endif //_BOMBERMAN_H_.