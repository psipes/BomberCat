

#pragma once

#if !defined(_DXMOUSE_H_)
#define _DXMOUSE_H_

#include "stdafx.h"

#include "Utilities/Point.h"
#include "DxFramework/DxCommon.h"
#include <dinput.h>
//#include <xinput.h>


class DxMouse
{
public:
	// Constructors
	DxMouse();
	~DxMouse();
 
	// Public Functions
	bool mouseInit(HWND hwnd, LPDIRECT3DDEVICE9 d3ddev);
	void mouseUpdate();
	void mouseShutdown();
	int mouseX();
	int mouseY();
	int mouseButton(int button);

private:
	//Private member variable
    Point                   myPosition;
	LPDIRECTINPUTDEVICE8	myMouse;
	LPDIRECTINPUT8			myInput;
	DIMOUSESTATE			myMouseState;
	HWND					myHwnd;
};

#endif