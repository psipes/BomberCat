
//=======================================================================
// Nathan Contreras
//   Controller wrapper for XINPUT
//=======================================================================

#pragma once

#if !defined( _DXCONTROLLER_H_ )
#define _DXCONTROLLER_H_

#include <xinput.h>
#include "DxCommon.h"
#include "DxTypes.h"

class DxController
{
   
//=====================================================================================
// Public methods
public:
   
   //-----------------------------------------------------------------------
   // enumeration of buttons
   //-----------------------------------------------------------------------
   enum DxButton;

   //-----------------------------------------------------------------------
   // enumeration of stick positions
   //-----------------------------------------------------------------------
   enum DxStick;


   //-----------------------------------------------------------------------
   // ctor
   //-----------------------------------------------------------------------
   DxController ();

   //-----------------------------------------------------------------------
   // dtor
   //-----------------------------------------------------------------------
   ~DxController ();

   //-----------------------------------------------------------------------
   // Initializes XInput and connected controllers
   // Param device The d3d device
   // Param contNum The number of this controller
   //-----------------------------------------------------------------------
	bool init ( IDXDEVICE device, int contNum = 0 );

   //-----------------------------------------------------------------------
   // Vibrate the controller
   //-----------------------------------------------------------------------
	bool vibrate ();

   //-----------------------------------------------------------------------
   // Checks the state of the controller
   //-----------------------------------------------------------------------
   bool update ();

   //-----------------------------------------------------------------------
   // Prepares this controller for destruction
   //-----------------------------------------------------------------------
	void shutdown ();

   //-----------------------------------------------------------------------
   // Sets the Dead zone of control sticks
   // param deadZone The desired deadZone of the control sticks
   //-----------------------------------------------------------------------
   void setStickDZ ( int deadZone );

   //-----------------------------------------------------------------------
   // Sets the vibration of the controller 
   // param vib The desired vibration of the controller
   //-----------------------------------------------------------------------
   void setVibration ( int vib );   

   //-----------------------------------------------------------------------
   // Get the first button to be pressed, returning B_NOBTNDOWN if none 
   // pressed. Returning an int representing a button which are all defined
   // within DxButton enumeration.
   //-----------------------------------------------------------------------
   DxButton getButtonDown ();
                                 
   //-----------------------------------------------------------------------
   // Get the direction the left stick is facing, in the form of a int value
   // from DxStick enumeration.  Useful for detecting if the stick is 
   // centered but still outside of the dead zone
   //-----------------------------------------------------------------------
   DxStick getLeftStickDir ();
                               
   //-----------------------------------------------------------------------
   // Get the direction the right stick is facing, in the form of a int 
   // value from DxStick enumeration. Useful for detecting if the stick is 
   // centered but still outside of the dead zone
   //-----------------------------------------------------------------------
   DxStick getRightStickDir ();
                                

   //-----------------------------------------------------------------------
   // Button held state getters.
   // Returns true if the buttons are being held.
   // Returns false if the buttons are not being held.
   //-----------------------------------------------------------------------
   bool leftTriggerDown ();        // is the left trigger being held?
   bool rightTriggerDown ();       // is the right trigger being held?
   bool leftShoulderDown ();       // is the left shoulder being held?
   bool rightShoulderDown ();      // is the right shoulder being held?
   bool leftThumbStickInDown ();   // is the left thumb stick being held?
   bool rightThumbStickInDown ();  // is the right thumb stick being held?
   bool dPadUpDown ();             // is the dpad being held up? 
   bool dPadDownDown ();           // is the dpad being held down?
   bool dPadLeftDown ();           // is the dpad being held left?
   bool dPadRightDown ();          // is the dpad being held right?
   bool buttonStartDown ();        // is the start button being held?
   bool buttonBackDown ();         // is the back button being held?
   bool buttonADown ();            // is the A button being held?
   bool buttonBDown ();            // is the B button being held?
   bool buttonXDown ();            // is the X button being held?
   bool buttonYDown ();            // is the Y button being held?

   //-----------------------------------------------------------------------
   // Stick direction state getters.
   // Returns true  If the stick is being moved into the desired position.
   // Returns false If the stick is not being moved into the desired position.
   //-----------------------------------------------------------------------
   bool leftStickUp ();            // is the left stick being moved up?
   bool leftStickDown ();          // is the left stick being moved down?
   bool leftStickLeft ();          // is the left stick being moved left?
   bool leftStickRight ();         // is the left stick being moved right?
   bool leftStickCentered ();      // is the left stick not being moved?

   bool rightStickUp ();           // is the right stick being moved up?
   bool rightStickDown ();         // is the right stick being moved down?
   bool rightStickLeft ();         // is the right stick being moved left?
   bool rightStickRight ();        // is the right stick being moved right?
   bool rightStickCentered ();     // is the right stick not being moved? 

//=====================================================================================
// Public member data
public:
	int myControllerNum;  // This controller's number

//=====================================================================================
// Private member data
private:
	XINPUT_STATE   myState;
   bool           myStateIsGood;
	IDXDEVICE      myDev;           // D3D Device
   int            myStickDZ;       // Dead zone of sticks
	int            myLeftVib;       // Vibration left
	int            myRightVib;      // Vibration right

//=====================================================================================
// Public Enumerated types
public:

   //-----------------------------------------------------------------------
   // Enumeration representing buttons on a controller.
   //-----------------------------------------------------------------------
	enum DxButton
	{
      B_ERROR = -1,    // Button Error
      B_NOBTNDWN = 0,  // No button pressed
		B_LT,      // Left Trigger 
		B_RT,      // Right Trigger 
		B_LS,      // Left Shoulder
		B_RS,      // Right Shoulder
		B_LTS,     // Left Thumb Stick 
		B_RTS,     // Right Thumb Stick 
		B_DPU,     // DPAD Up
		B_DPD,     // DPAD Down
		B_DPL,     // DPAD Left
		B_DPR,     // DPAD Right
		B_START,   // Start
		B_BACK,    // Back
		B_A,       // A
		B_B,       // B
		B_X,       // X
		B_Y        // Y
	};

   //-----------------------------------------------------------------------
   // Enumeration representing control stick states on a controller.
   //-----------------------------------------------------------------------
	enum DxStick
	{
      STICK_ERROR = -1,   // Stick left error
      STICK_CENTER = 0,   // Stick left is centered

		SL_UP,     // Stick left is up
		SL_DOWN,   // Stick left is down
		SL_LEFT,   // Stick left is left
		SL_RIGHT,  // Stick left is right
      
      SR_UP,     // Stick left is up
      SR_DOWN,   // Stick left is down
      SR_LEFT,   // Stick left is left
      SR_RIGHT   // Stick left is right
	};
};

#endif