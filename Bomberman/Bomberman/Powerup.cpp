#include "stdafx.h"

#include "Utilities/TTypes.h"
#include "Powerup.h"

//========================================================================================
tstring Powerup::toString ( Powerup::Power power )
{
   switch ( power )
   {
   case POWER_LIFE:
      return "P1UP";
   case POWER_PASSTHRU:
      return "PBOMBPASS";
   case POWER_MULTIBOMB:
      return "PBOMBUP";
   case POWER_FIREDOWN:
      return "PFIREDOWN";
   case POWER_FIREUP:
      return "PFIREUP";
   case POWER_POOP:
      return "PPOOP";
   case POWER_SPEED:
      return "PSPEED";
   case POWER_TIME:
      return "PTIME";
   case POWER_COOKIE:
      return "COOKIE";
   case POWER_TOYMOUSE:
      return "TOYMOUSE";
   case POWER_YARN:
      return "YARN";
   }
   return "ERROR";
}

//========================================================================================
bool Powerup::create ( Power power, float x, float y )
{
   tstring powerName = toString( power );
   
   if ( power != Power::POWER_NONE && DxGameSprite::create( powerName, 10 ) )
   {
      myPower = power;
      setPosition( x, y );
      return true;
   }
   return false;
}

//========================================================================================
