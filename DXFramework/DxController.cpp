
//=======================================================================
// Nathan Contreras
//   Controller wrapper for XINPUT
//=======================================================================

#include <stdafx.h>
#include <xinput.h>
#include "DxFramework/DxCommon.h"
#include "DXFramework/DxController.h"

#pragma comment(lib,"xinput.lib")

//=======================================================================
DxController::DxController ( )
{
   myDev = NULL;
}

//=======================================================================
DxController::~DxController ( )
{
}

//=======================================================================
bool DxController::init (IDXDEVICE device, int contNum)
{	
   myDev = device; 
   myDev->AddRef();
   myControllerNum = contNum;
   myStickDZ = 10000;
   myLeftVib = 65535;
   myRightVib = 65535;
   myStateIsGood = false;

   // init state
   DWORD result = XInputGetState( myControllerNum, &myState );
   if ( result == ERROR_SUCCESS )
   {
      XINPUT_CAPABILITIES caps;
      ZeroMemory(&caps, sizeof(caps));

      result = XInputGetCapabilities(contNum, XINPUT_FLAG_GAMEPAD, &caps);      
      myStateIsGood = ( caps.Type == ERROR_SUCCESS ); // XINPUT_DEVTYPE_GAMEPAD is default 0x01,
                                                      // caps.Type returns ERROR_SUCCESS
                                                      // myStateIsGood would always return false
                                                      // checking to see if the type is successful.
   }

   return myStateIsGood;
}

//=======================================================================
bool DxController::vibrate ()
{
   XINPUT_VIBRATION vibration;
   ZeroMemory( &vibration, sizeof(vibration) );
   vibration.wLeftMotorSpeed = myLeftVib;
   vibration.wRightMotorSpeed = myRightVib; 
   XInputSetState( myControllerNum, &vibration );
   return true;
}

//=======================================================================
bool DxController::update ()
{
   XINPUT_STATE state;
   ZeroMemory( &state, sizeof(state) );

   //get the state of the controller
   // ERROR_SUCCESS.
   // ERROR_DEVICE_NOT_CONNECTED.
   DWORD result = XInputGetState( myControllerNum, &state );
   if ( result == ERROR_SUCCESS)
   {
      myState = state;
      myStateIsGood = true;
   }
   return myStateIsGood;
}

//=======================================================================
void DxController::shutdown ( )
{
   IfRelease(&myDev);
}

//=======================================================================
void DxController::setStickDZ ( int deadZone )
{
   myStickDZ = deadZone;
}

//=======================================================================
void DxController::setVibration ( int vib )
{
   myLeftVib = vib;
   myRightVib = vib;
}

//=======================================================================
DxController::DxButton DxController::getButtonDown ()
{
   if ( myStateIsGood )
   { 
      // controller is connected 
      if (myState.Gamepad.bLeftTrigger)
      {
         return B_LT;
      }

      else if (myState.Gamepad.bRightTrigger)
      {
         return B_RT;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
      {
         return B_DPU;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
      {
         return B_DPD;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
      {
         return B_DPL;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
      {
         return B_DPR;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_START)
      {
         return B_START;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB)
      {
         return B_LTS;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB)
      {
         return B_RTS;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER)
      {
         return B_RS;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
      {
         return B_LS;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_A)
      {
         return B_A;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_B)
      {
         return B_B;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_X)
      {
         return B_X;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_Y)
      {
         return B_Y;
      }

      else if (myState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
      {
         return B_BACK;
      }
      
      else
      {
         return B_NOBTNDWN;
      }
   }

   return B_ERROR;
}

//=======================================================================
DxController::DxStick DxController::getLeftStickDir ()
{
   if ( myStateIsGood )
   {
      if ( myState.Gamepad.sThumbLX < -myStickDZ )
      {
         return SL_LEFT;
      }
      else if ( myState.Gamepad.sThumbLX > myStickDZ )
      {
         return SL_RIGHT;
      }
      else if ( myState.Gamepad.sThumbLY < -myStickDZ )
      {
         return SL_DOWN;
      }
      else if ( myState.Gamepad.sThumbLY > myStickDZ )
      {
         return SL_UP;
      }
      else
      {
         return STICK_CENTER;
      }
   }
   return STICK_ERROR;
}

//=======================================================================
DxController::DxStick DxController::getRightStickDir ()
{
   if ( myStateIsGood )
   {
      if ( myState.Gamepad.sThumbRX < -myStickDZ )
      {
         return SR_LEFT;
      }
      else if ( myState.Gamepad.sThumbRX > myStickDZ )
      {
         return SR_RIGHT;
      }
      else if ( myState.Gamepad.sThumbRY < -myStickDZ )
      {
         return SR_DOWN;
      }
      else if ( myState.Gamepad.sThumbRY > myStickDZ )
      {
         return SR_UP;
      }
      else
      {
         return STICK_CENTER;
      }
   }
   return STICK_ERROR;
}

//=======================================================================
bool DxController::leftTriggerDown()
{
   return ( myStateIsGood && ( myState.Gamepad.bLeftTrigger ) );
}

//=======================================================================
bool DxController::rightTriggerDown()
{
   return ( myStateIsGood && ( myState.Gamepad.bRightTrigger ) );
}

//=======================================================================
bool DxController::leftShoulderDown()
{
   return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER ) );
}

//=======================================================================
bool DxController::rightShoulderDown()
{
   return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER ) );
}

//=======================================================================
bool DxController::leftThumbStickInDown()
{
   return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_THUMB ) );
}

//=======================================================================
bool DxController::rightThumbStickInDown()
{
   return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB ) );
}

//=======================================================================
bool DxController::dPadUpDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP ) );
}

//=======================================================================
bool DxController::dPadDownDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN ) );
}

//=======================================================================
bool DxController::dPadLeftDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT ) );
}

//=======================================================================
bool DxController::dPadRightDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT ) );
}

//=======================================================================
bool DxController::buttonStartDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_START ) );
}

//=======================================================================
bool DxController::buttonBackDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_BACK ) );
}

//=======================================================================
bool DxController::buttonADown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_A ) );
}

//=======================================================================
bool DxController::buttonBDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_B ) );
}

//=======================================================================
bool DxController::buttonXDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_X ) );
}

//=======================================================================
bool DxController::buttonYDown()
{
    return ( myStateIsGood && ( myState.Gamepad.wButtons & XINPUT_GAMEPAD_Y ) );
}


//=======================================================================
bool DxController::leftStickUp()
{
   return ( myStateIsGood && myState.Gamepad.sThumbLY > myStickDZ );
}

//=======================================================================
bool DxController::leftStickDown()
{
   return ( myStateIsGood && myState.Gamepad.sThumbLY < -myStickDZ );
}

//=======================================================================
bool DxController::leftStickLeft()
{
   return ( myStateIsGood && myState.Gamepad.sThumbLX < -myStickDZ );
}

//=======================================================================
bool DxController::leftStickRight()
{
   return ( myStateIsGood && myState.Gamepad.sThumbLX > myStickDZ );
}

//=======================================================================
bool DxController::leftStickCentered()
{
   return ( myStateIsGood && ( myState.Gamepad.sThumbLX <= 0 ) && ( myState.Gamepad.sThumbLY <= 0 ) );
}


//=======================================================================
bool DxController::rightStickUp()
{
   return ( myStateIsGood && myState.Gamepad.sThumbRY > myStickDZ );
}

//=======================================================================
bool DxController::rightStickDown()
{
   return ( myStateIsGood && myState.Gamepad.sThumbRY < -myStickDZ );
}

//=======================================================================
bool DxController::rightStickLeft()
{
   return ( myStateIsGood && myState.Gamepad.sThumbRX < -myStickDZ );
}

//=======================================================================
bool DxController::rightStickRight()
{
   return ( myStateIsGood && myState.Gamepad.sThumbRX > myStickDZ );
}

//=======================================================================
bool DxController::rightStickCentered()
{
   return ( myStateIsGood && ( myState.Gamepad.sThumbRX <= 0 ) && ( myState.Gamepad.sThumbRY <= 0 ) );
}
