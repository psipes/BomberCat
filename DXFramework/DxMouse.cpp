#include "stdafx.h"
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxMouse.h"

DxMouse::DxMouse()
{
   ZeroMemory( &myMouseState, sizeof(myMouseState) );
   myMouse = NULL;
   myInput = NULL;
}

DxMouse::~DxMouse()
{

}


bool DxMouse::mouseInit(HWND hwnd, LPDIRECT3DDEVICE9 d3ddev)
{
   myHwnd = hwnd;
   //initialize DirectInput object
   DirectInput8Create( GetModuleHandle(NULL), 
                       DIRECTINPUT_VERSION, 
                       IID_IDirectInput8,
                       (void**)&myInput,
                       NULL);

   //initialize the mouse
   myInput->CreateDevice(GUID_SysMouse, &myMouse, NULL);
   myMouse->SetDataFormat(&c_dfDIMouse);
   myMouse->SetCooperativeLevel(hwnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
   myMouse->Acquire();
   d3ddev->ShowCursor(false);
   GetCursorPos( &myPosition );

   return true;
}

void DxMouse::mouseUpdate()
{
   //update mouse
   myMouse->Poll();
   if (!SUCCEEDED(myMouse->GetDeviceState(sizeof(myMouseState),&myMouseState)))
   {
      //mouse device lose, try to re-acquire
      myMouse->Acquire();
   }
   
   Point clientPos;
   ClientToScreen( myHwnd, &clientPos );
   GetCursorPos( &myPosition );
   
   myPosition.x = myPosition.x - clientPos.x;
   myPosition.y = myPosition.y - clientPos.y;
}

void DxMouse::mouseShutdown()
{
   if (myMouse) 
   {
      myMouse->Unacquire();
      myMouse->Release();
      myMouse = NULL;
   }
}


int DxMouse::mouseX()
{
   return myPosition.x;
}

int DxMouse::mouseY()
{
   return myPosition.y;
}

int DxMouse::mouseButton(int button)
{
   return myMouseState.rgbButtons[button] & 0x80;
}