/*
   DEAD CODE - Perhaps to be resurrected later :-)
*/


//virtual int onCreate ( CREATESTRUCT& cs ); //WM_CREATE
//virtual int onPaint ( HDC hdc, PAINTSTRUCT& ps ); //WM_PAINT
//virtual int onMove ( int xPos, int yPos );  //WM_MOVE
//virtual int onSize ( int resizeType, int clientWidth, int clientHeight ); //WM_SIZE
//virtual int onActivate ( int activationType,  bool isMinimized, HWND hWnd );  // WM_ACTIVATE
//virtual int onSetFocus ( HWND hPrevFocusWnd ); //WM_SETFOCUS
//virtual int onKillFocus ( HWND hNextFocusWnd ); //WM_KILLFOCUS
//virtual int onEnable ( bool isEnabled ); //WM_ENABLE
//
//virtual int onClose ( void ); //WM_CLOSE
//virtual int onQuit ( int exitCode ); //WM_QUIT
//virtual int onShowWindow ( bool isShow, int swReason ); // WM_SHOWWINDOW
//virtual int onChar ( int ch, UINT keyRepeatCount, unsigned scanCode, unsigned keyBits );
//virtual int onTimer ( int timerId, void* lParam );


int WinAppl::onCreate ( CREATESTRUCT& cs )
{
   // Create should return 0 to continue window creation.
   return 0;
}

int WinAppl::onPaint ( HDC hdc, PAINTSTRUCT& ps )
{
   return -1;
}

int WinAppl::onMove ( int xPos, int yPos )
{
   return -1;
}

int WinAppl::onSize ( int resizeType, int clientWidth, int clientHeight )
{
   return -1;
}

int WinAppl::onActivate ( int activationType,  bool isMinimized, HWND hWnd )
{
   return -1;
}

int WinAppl::onSetFocus ( HWND hPrevFocusWnd )
{
   return -1;
}

int WinAppl::onKillFocus ( HWND hNextFocusWnd )
{
   return -1;
}

int WinAppl::onEnable ( bool isEnabled )
{
   return -1;
}

int WinAppl::onClose ( void )
{
   return -1;
}

int WinAppl::onQuit ( int exitCode )
{
   return -1;
}

int WinAppl::onShowWindow ( bool isShow, int swReason )
{
   return -1;
}

int WinAppl::onChar ( int ch, UINT keyRepeatCount, unsigned scanCode, unsigned keyBits )
{
   return -1;
}

int WinAppl::onTimer ( int timerId, void* lParam )
{
   return -1;
}

