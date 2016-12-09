#pragma once

#if !defined(_DXKEYBOARD_H_)
#define _DXKEYBOARD_H_

#include "stdafx.h"
#include <windows.h>
#include "DxFramework/DxCommon.h"

class DxKeyboard
{
public:
   enum KeyState
   {      
      UP,
      DOWN
   };

   static bool keyPressed(int key);

	static bool keyDown(int key);
   static bool keyUp(int key);
   
   DxKeyboard () {}
   ~DxKeyboard () {}
   
   bool keyboardInit ( HWND hwnd ) { UNREFERENCED_PARAMETER(hwnd); ZeroMemory(ourKeyboardState, sizeof(ourKeyboardState)); return true; }
   bool keyboardUpdate () { return true; }


private:

   static KeyState ourKeyboardState[256];
};

#endif

