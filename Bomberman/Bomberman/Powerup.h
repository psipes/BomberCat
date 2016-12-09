 #pragma once

#if !defined(_POWERUP_H_)
#define _POWERUP_H_

#include "Utilities/TTypes.h"
#include "DxFramework/DxGameSprite.h"

//========================================================================================
// Powerup class represents a powerup in-game.
class Powerup : public DxGameSprite
{
public:
   //========================================================================================
   // Power enumeration representing the various available power-ups
   enum Power { POWER_NONE, POWER_LIFE, POWER_PASSTHRU, POWER_MULTIBOMB, POWER_FIREDOWN, POWER_FIREUP, POWER_POOP, POWER_SPEED, POWER_TIME, POWER_COOKIE, POWER_TOYMOUSE, POWER_YARN };

   //========================================================================================
   // Converts the passed Power to a tString representation.
   static tstring toString ( Power power );

   //========================================================================================
   // Creates a power-up of desired type Power at desired location x/y.
   bool create ( Power power, float x, float y );

   //========================================================================================
   // Returns this power-up.
   Power power () { return myPower; }

private:
   Power myPower;
};

#endif //_POWERUP_H_
