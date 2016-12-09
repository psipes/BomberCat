
#include "stdafx.h"
#include <algorithm>
#include <Commctrl.h>

#include <crtdbg.h>

#include "NVCUtils/TString.h"
#include "NVCUtils/NVCUtils.h"
#include "WinApplFramework.h"

using namespace NVCUtils;

#pragma comment(lib,"Comctl32.lib")

// "Magic" value (taken from CRC-32-IEEE802.3 x 3) is used during window
// creation process as a final sanity check that the untyped (void*) lpCreateParams
// is indeed a real WinApplFramework (this*), using the following formula:
// (((this ^ gMagicVal) ^ gMagicVal) == this)
const unsigned gMagicVal = 0x04C11DB7; 

// Forward declarations
LRESULT CALLBACK startupWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK forwardingWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 


WinApplFramework::WinApplFramework (  )
{
   hPrevInstance  = NULL;
   hInstance      = NULL;
   hMainWnd       = NULL;

   lpCmdLine      = _T("");
   nCmdShow       = 0;

   szAppTitle     = _T("CTOR Title");
   szAppWndClass  = _T("CTOR_CLASS");

   myAppIsRunning = false;
}

//---------------------------------------------------------------------------------------- 

void WinApplFramework::loadResourceStrings ( void )
{
	// Load global strings
   TCHAR buffer [ MAX_LOADSTRING ];
   UINT id = winResrcIdTitleStr();
   if ( id != 0 )
   {
	   LoadString( hInstance, id, buffer, MAX_LOADSTRING );
      szAppTitle = buffer;
   }
   
   id = winResrcIdClassStr();
   if ( id != 0 )
   {
	   LoadString( hInstance, id, buffer, MAX_LOADSTRING );
      szAppWndClass = buffer;
   }
}

//---------------------------------------------------------------------------------------- 

// Enable CRT Heap (dynamic memory) debugging using vendor specific 
// functionality.  Note that this has NO EFFECT in RELEASE builds.
void WinApplFramework::enableCRTHeapDebugging ( bool maxImpact )
{
#ifdef _DEBUG
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG ) | _CRTDBG_LEAK_CHECK_DF;

   // _CRTDBG_CHECK_ALWAYS_DF can SERIOUSLY slow execution, but breakpoints immediately on heap corruption
   if ( maxImpact )
      flag |= _CRTDBG_CHECK_ALWAYS_DF;

	_CrtSetDbgFlag(flag);
   
   // NOTE: This allocation is INTENTIONALLY leaked.  It should show up in the output window at program 
   // termination.  Any allocation #'s LESS than this can be ignored: microsoft bugs in the CRT.
   // It shows up as a "data: <    > DE AD BE EF", i.e., "DEAD BEEF"
   int* iTestDebug = new int( 0xEFBEADDE );

   OutputDebugString( _T("\n\
******************************************************\n\
CRT DEBUG MODE ENABLED\n\
After program exits, review debug output window\n\
for leaks and other heap corruption information\n\
EXPECT ONE INTENTIONAL LEAK:\n\
    {nnn} normal block at 0xXXXXXXXX, 4 bytes long\n\
    Data: <    > DE AD BE EF\n\
Any allocation number < {nnn} can be ignored\n") );

   if ( maxImpact )
      OutputDebugString( _T(">>> MAX HEAP CHECKING ENABLED: EXPECT SLOW EXECUTION <<<\n") );

   OutputDebugString( _T("\
******************************************************\n\n"));
      
#endif
   
   // For Reference: These could be used to expand upon the debugging
   // Store current heap state
   //_CrtMemState  memState;
   //_CrtMemCheckpoint( &memState );

   // Dump allocations since given checkpoint
   // _CrtMemDumpAllObjectsSince( &memState );

   // _CrtDumpMemoryLeaks(); immediate memory dump report to output window

}

//---------------------------------------------------------------------------------------- 

void WinApplFramework::breakOnCRTAlloc ( int n )
{
#ifdef _DEBUG
   _CrtSetBreakAlloc( n ); 
#endif
}

//---------------------------------------------------------------------------------------- 
int WinApplFramework::winMain ( TCHAR*    appTitleStr,
                                HINSTANCE hInstance,
                                HINSTANCE hPrevInstance,
                                LPTSTR    lpCmdLine,
                                int       nCmdShow )
{
   szAppTitle = appTitleStr;
   szAppWndClass = appTitleStr;
   szAppWndClass += TEXT("_WinClass");
   std::replace( szAppWndClass.begin(), szAppWndClass.end(), _T(' '), _T('_') );  

   return winMain( hInstance, hPrevInstance, lpCmdLine, nCmdShow );
}

int WinApplFramework::winMain ( HINSTANCE hInstance,
                                HINSTANCE hPrevInstance,
                                LPTSTR    lpCmdLine,
                                int       nCmdShow )
{
   this->hInstance = hInstance;
   this->hPrevInstance = hPrevInstance;
   this->lpCmdLine = lpCmdLine;
   this->nCmdShow = nCmdShow;

   // Load Optional Libraries (non destructively)
   HMODULE hm = LoadLibrary( TEXT("RichEd20.dll") );
   if ( hm == NULL )
      hm = LoadLibrary( TEXT("Msftedit.dll") );

   if ( hm != NULL )
   {
      INITCOMMONCONTROLSEX ccEx;
      ccEx.dwSize = sizeof(ccEx);
      ccEx.dwICC = ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_USEREX_CLASSES | ICC_UPDOWN_CLASS;
      InitCommonControlsEx( &ccEx );
   }


   myAppIsRunning = true;

   loadResourceStrings();


   // ------------------ Windows Application Initialization --------------
   if ( !(winPreCreateWindow() && winRegisterClass() && winInitInstance( nCmdShow )) )
	{
      // Error message:  Pre-Game Windows initialization failed
      Utils::debugMsg( _T("Exiting application: preCreateWindow/registerClass/initInstance failure\n") );
		return false;
	}

   ShowWindow( hWnd(), nCmdShow);
   UpdateWindow( hWnd() );

	HACCEL hAccelTable;
	hAccelTable = LoadAccelerators( hInstance, MAKEINTRESOURCE( winResrcIdClassStr() ) );

   if ( !winPostCreateWindow() )
   {
      Utils::debugMsg( _T("Exiting application: winPostCreateWindow FAIL\n") );
      return false;
   }


   // main message loop
   static int msgCount = 0;
   MSG message = {0};
   DWORD ltc = 0;
   
   try 
   {	        
      while ( myAppIsRunning && message.message != WM_QUIT )
      {
         while ( PeekMessage( &message, NULL, 0, 0, PM_REMOVE ) && (message.message != WM_QUIT) ) 
         {
            //Utils::debugWinMsg ( &message );
            msgCount++;
            TranslateMessage(&message);
            DispatchMessage(&message);
         }

         winAppIdle();

      }
   }
   catch (...)
   {
      Utils::debugMsg( _T("FATAL ERROR: EXCEPTION caught in WinApplFramework::winMain\n" ) );
   }
   
      
   Utils::debugMsg( _T("Exiting Main Message loop\n") );

   // Informational Message - exiting application 
   winAppExit();

   FreeLibrary( hm );

   return message.wParam;
}


//---------------------------------------------------------------------------------------- 
// Change the main window title to the specified string
//
void WinApplFramework::winSetTitle ( tstring title )
{
   ::SetWindowText( hWnd(), title.c_str() );
}

//---------------------------------------------------------------------------------------- 
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
bool WinApplFramework::winRegisterClass ( void )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			   = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	   = startupWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= sizeof( this );
	wcex.hInstance		   = hInst();
	wcex.lpszClassName	= winClassName(); 
	wcex.lpszMenuName	   = winClassMenu();
	wcex.hCursor		   = winClassCursor();
	wcex.hbrBackground	= winClassBackgroundBrush();
	wcex.hIcon			   = winClassIcon();
	wcex.hIconSm		   = winClassSmallIcon();

	return RegisterClassEx( &wcex ) != 0;
}

//---------------------------------------------------------------------------------------- 

HBRUSH WinApplFramework::winClassBackgroundBrush ( void )
{
   return (HBRUSH)(COLOR_WINDOW+1);
}

//---------------------------------------------------------------------------------------- 

HCURSOR WinApplFramework::winClassCursor ( void )
{
   return LoadCursor(NULL, IDC_ARROW);
}

//---------------------------------------------------------------------------------------- 

HICON WinApplFramework::winClassIcon ( void )
{
   if ( winResrcIdIcon() == 0 )
      return NULL;

   return LoadIcon( hInst(), MAKEINTRESOURCE( winResrcIdIcon()  ));
}

//---------------------------------------------------------------------------------------- 

HICON WinApplFramework::winClassSmallIcon ( void )
{
   if ( winResrcIdSmallIcon() == 0 )
      return NULL;

   return LoadIcon( hInst(), MAKEINTRESOURCE(winResrcIdSmallIcon()));
}

//---------------------------------------------------------------------------------------- 

LPCTSTR WinApplFramework::winClassMenu ( void )
{
   if ( winResrcIdMenu() == 0 )
      return NULL;

   return (LPCTSTR) MAKEINTRESOURCE(winResrcIdMenu());
}

//---------------------------------------------------------------------------------------- 

LPCTSTR WinApplFramework::winClassName ( void )
{
   if ( szAppWndClass.length() == 0 )
   {
      Utils::debugMsgBox( NULL, _T("*** UNEXPECTED: Empty WndClass name, using placeholder\n") 
                                _T("WinApplFramework::winClassName() or winResrcIdClassStr() needs to be overridden\n") );
         

      return _T("WinApplFramework_DEFAULT_CLASS");
   }
   return szAppWndClass.c_str();
}//---------------------------------------------------------------------------------------- 

LPCTSTR WinApplFramework::winAppTitleStr ( void )
{
   if ( szAppTitle.length() == 0 )
   {
      Utils::debugMsgBox( NULL, _T("*** UNEXPECTED: Window Title empty, using placeholder\n") 
                                _T("WinApplFramework::winAppTitleStr() or winResrcIdTitleStr() needs to be overridden\n") );
      return _T("WinApplFramework Default Title");
   }
   return szAppTitle.c_str();
}

//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 


bool WinApplFramework::winInitInstance( int nCmdShow )
{
   // See gMagicVal comment at top of file regarding this initialization.
   // Value is tested *DURING* CreateWindow and causes failure if not present!
   hMainWnd = HWND( DWORD(this) ^ gMagicVal );
   hMainWnd = CreateWindow( winClassName(), winAppTitleStr(), 
                            WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, 0, 
                            CW_USEDEFAULT, CW_USEDEFAULT, // some room for edges
                            NULL, NULL, hInst(), 
                            (void*) this );

   winResizeClient();

   if ( !hMainWnd )
   {
      return FALSE;
   }

   ShowWindow( hMainWnd, nCmdShow );
   UpdateWindow( hMainWnd );

   return TRUE;
}

//---------------------------------------------------------------------------------------- 
//
void WinApplFramework::winResizeClient ( void )
{
   int nWidth = winScreenWidth();
   int nHeight = winScreenHeight();

   RECT rcClient, rcWindow;
   POINT ptDiff;
   GetClientRect( hWnd(), &rcClient);
   GetWindowRect( hWnd(), &rcWindow);
   ptDiff.x = (rcWindow.right - rcWindow.left) - rcClient.right;
   ptDiff.y = (rcWindow.bottom - rcWindow.top) - rcClient.bottom;
   MoveWindow( hWnd(),rcWindow.left, rcWindow.top, nWidth + ptDiff.x-1, nHeight + ptDiff.y-1, TRUE);
}

//---------------------------------------------------------------------------------------- 

LRESULT WinApplFramework::memberWndProc(HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
   int result = -1;

   if ( message == WM_COMMAND )
   {
      if ( onCommand( hWindow, wParam, lParam ) == -1 )
         return DefWindowProc(hWindow, message, wParam, lParam);
   }
   else if ( WM_USER <= message && message <= (WM_APP-1) )
   {
      if ( onUserMsg( hWindow, message, wParam, lParam ) == -1 )
         return DefWindowProc(hWindow, message, wParam, lParam);
   }
   else if ( WM_APP <= message && message <= 0xBFFF )
   {
      if ( onAppMsg( hWindow, message, wParam, lParam ) == -1 )
         return DefWindowProc(hWindow, message, wParam, lParam);
   }
   else if ( 0xC000 <= message && message <= 0xFFFF ) 
   {
      if ( onRegMsg( hWindow, message, wParam, lParam ) == -1 )
         return DefWindowProc(hWindow, message, wParam, lParam);
   }
   
	if ( (WM_SYSKEYDOWN == message) && (wParam == VK_F10) )
      return 0;   // Disables Windows Accelerator that causes pause!

   switch ( message )
   {
   case WM_DESTROY:
      result = onDestroy();
      myAppIsRunning = false;
      break;
   default:
      result = onMessage( hWindow, message, wParam, lParam );
      break;
   }


   if ( result == -1 )
      return DefWindowProc(hWindow, message, wParam, lParam);

	return 0;
}


//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//  -------------- Default Implementations of virtual methods -------------------------

int WinApplFramework::onUserMsg( HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam )
{
   return -1;
}

int WinApplFramework::onAppMsg ( HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam )
{
   return -1;
}

int WinApplFramework::onRegMsg ( HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam )
{
   return -1;
}

int WinApplFramework::onMessage( HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam )
{
   return -1;
}

int WinApplFramework::onCommand ( HWND hWindow, WPARAM wParam, LPARAM lParam )
{
   if ( lParam == 0 )
   {
      // Menu & Accelerator messages
         // Menu:    HIWORD(wParam) == 0, LOWORD(wParam) == Menu Identifier (IDM_*)
         // Accel:   HIWORD(wParam) == 1, LOWORD(wParam) == Accelerator menu id (IDM_*)
      return onMenuCommand( hWindow, LOWORD(wParam), (HIWORD(wParam) == 1) );
   }
   else 
   {
      // Control Notification: LOWORD(wParam) == control ID; HIWORD(wParam) == Ctrl-defined notify code; 
      //                       lParam == Handle of child control window
      return onCtrlCommand( hWindow, LOWORD(wParam), HIWORD(wParam), HWND(lParam) );
   }

   return -1;
}

int WinApplFramework::onMenuCommand( HWND hwnd, DWORD menuId, bool isAccel )
{
   return -1;
}
int WinApplFramework::onCtrlCommand( HWND hwnd, DWORD ctrlId, DWORD ctrlNotifyCode, HWND hCtrl )
{
   return -1;
}


int WinApplFramework::onDestroy ( void )
{
   PostQuitMessage( 0 );
   return 0;
}


//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 


//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
static
LRESULT CALLBACK startupWndProc ( HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
   if ( message == WM_CREATE )
   {
      // Sanity checks to verify lpCreateParams is a (WinApplFramework*)
      //    1. lParam must pass the Utils::isReservedBadPtr() tests
      //    2. lpCreateParams must pass the Utils::isReservedBadPtr() tests
      //    3. lpCreateParams - ASSUMED (this*) - is DEREFERENCED to call
      //       hWnd(), which MUST be initialized as (this ^ gMagicVal).
      //       NOTE THIS STEP CAN CRASH IF ASSUMPTION IS INCORRECT, even
      //       after passing checks #1 and #2.
      //
      // NOTE: Use of c++ style reinterpret_cast and static_cast shows their
      //       expected usage *but at this low level IS INDISTINGUISHABLE from
      //       the equivalient c-style cases (type*)

      CREATESTRUCT* pcs;
      pcs = reinterpret_cast<CREATESTRUCT*>(lParam); // (CREATESTRUCT*)lParam;
      
      // Sanity Checks #1 and #2
      if ( Utils::isReservedBadPtr(pcs) || Utils::isReservedBadPtr(pcs->lpCreateParams) )
      {
         MessageBox( hWindow, _T("WM_CREATE structure pointers are bad"), _T("FATAL FRAMEWORK ERROR"), MB_OK );
         assert(false);
      }

      WinApplFramework* pWinAppl = static_cast<WinApplFramework*>( pcs->lpCreateParams ); // (WinApplFramework*) pcs->lpCreateParams;   // the (this*)
      assert(pWinAppl != NULL );
      HWND hwnd =  ((WinApplFramework*) pcs->lpCreateParams)->hWnd();
      
      // Sanity Check #3 - ( (hwnd=(this ^ gMagicVal)) ^ gMagicVal == pWinAppl)
      if ( pWinAppl != (void*)(DWORD(hwnd) ^ gMagicVal) )
      {
         MessageBox( hWindow, _T("startupWndProc: HWND sanity check FAILED"), _T("FATAL FRAMEWORK ERROR"), MB_OK );
         assert(false);
      }
      else
      {
         NVCUtils::Utils::debugMsg( _T("startupWndProc: (this*) validation SUCCEEDED\n") );
      }

      ::SetWindowLongPtr( hWindow, GWLP_USERDATA, (LONG) pcs->lpCreateParams );
      ::SetWindowLongPtr( hWindow, GWLP_WNDPROC, (LONG) forwardingWndProc );
      return pWinAppl->memberWndProc( hWindow, message, wParam, lParam );
   }

   return DefWindowProc(hWindow, message, wParam, lParam);
}



static
LRESULT CALLBACK forwardingWndProc ( HWND hWindow, UINT message, WPARAM wParam, LPARAM lParam)
{
   WinApplFramework* pWinAppl = (WinApplFramework*)GetWindowLongPtr( hWindow, GWLP_USERDATA );
   return pWinAppl->memberWndProc( hWindow, message, wParam, lParam );
}


