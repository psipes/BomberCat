
#pragma once

#define WIN32_EXTRA_LEAN

#if !defined( DIRECTINPUT_VERSION )
#define DIRECTINPUT_VERSION 0x0800
#endif 

#define _USE_MATH_DEFINES

#include <iostream>
#include <iomanip>
#include <windows.h>
#include <tchar.h>
#include <string>
#include <math.h>
#include <cmath>
#include "NVCUtils/tstring.h"


//----------------------------------------------------------------------------------------
// non-namespaced utility functions for standard or windows types

tostream& operator<< ( tostream& strm, const RECT& r );

bool operator== ( const RECT& lhs, const RECT& rhs );
bool operator!= ( const RECT& lhs, const RECT& rhs );



//----------------------------------------------------------------------------------------

namespace NVCUtils
{

/// <summary> 
/// class VFormatHelper is useful for handling the common variable argument list
/// construction which allows a format string and a variable number of arguments,
/// like printf( fmtStr, ... ).   It handles the internal va_list handling plus
/// allocates an internal buffer to format the string.
/// Usage:
///    RETTYPE foo ( const char* fmtStr, ... ) {
///       VFormatHelper vf( fmtStr, 128 ); // defaults to 1024 char buffer
///       // vf.bufPtr is the pointer to the formatted string
///       puts( vf.bufPtr );
///       }
/// </summary>
class VFormatHelper
{
public:
   VFormatHelper ( const TCHAR*& fmtStr, int bufSize = 512 )
   {
      va_start( ap, fmtStr );
      int bufSz = dfltBufSize;

      bufPtr = defaultBuffer;
      if ( bufSize > dfltBufSize )
      {
         bufPtr = new TCHAR[ bufSz ]; 
         bufSz = bufSize;
      }

      _vstprintf_s( bufPtr, bufSz, fmtStr, ap );
   }

   VFormatHelper ( const TCHAR** fmtStr, int bufSize = 512 )
   {
      va_start( ap, *fmtStr );
      int bufSz = dfltBufSize;

      bufPtr = defaultBuffer;
      if ( bufSize > dfltBufSize )
      {
         bufPtr = new TCHAR[ bufSz ]; 
         bufSz = bufSize;
      }

      _vstprintf_s( bufPtr, bufSz, *fmtStr, ap );
   }

   ~VFormatHelper ( )
   {
      if ( bufPtr != defaultBuffer )
         delete [] bufPtr;
      bufPtr = NULL;
      va_end(ap);
   }

   inline TCHAR* c_str ( void ) const { return bufPtr; }

private:
   static const int dfltBufSize = 512;
   va_list  ap;
   TCHAR*   bufPtr;
   TCHAR    defaultBuffer[ dfltBufSize ];
};


//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 


///<summary>
/// BREAKPOINT MACRO:  Hardcodes a processor single-step interrupt (INT 3) directly inline
/// which immediately triggers the debugger.  NOT enabled in release builds, for obvious reasons
///</summary>
#ifdef _DEBUG
   #define BREAKPOINT() \
      { \
         __asm  \
         {  \
            int 3  \
         }  \
      }
#else
   #define BREAKPOINT() \
      { \
         __asm  \
         {  \
            nop  \
         }  \
      }
#endif


// hard breakpoint if condition is false (assertion)
#define breakpt_assert(cond) \
   if (!(cond)) \
      BREAKPOINT()

// hard breakpoint if condition is true
#define breakpt_if(cond) \
   if ((cond)) \
      BREAKPOINT()

//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 
//---------------------------------------------------------------------------------------- 

/// <summary> 
/// class Utils is a container for *static* utility functions which are generally 
/// useful in any project.
///
/// By design, these routines are intended to be static, and if possible, inline.
/// 
/// Non-static methods must be approved before being added to Utils!
/// </summary>

class Utils
{
private:
   static int ourVerbosityLevel;      // = Utils::vMedium;

public:

   // Testing methods for testing Utils methods:
   static bool testHighResTiming();
   static bool testUtils();
   static void testWinMsgDebug();
   static bool testDebugMessages();

   
   //0xABABABAB : Used by Microsoft's HeapAlloc() to mark "no man's land" guard bytes after allocated heap memory
   //0xABADCAFE : A startup to this value to initialize all free memory to catch errant pointers
   //0xBAADF00D : Used by Microsoft's LocalAlloc(LMEM_FIXED) to mark uninitialised allocated heap memory
   //0xBADCAB1E : Error Code returned to the Microsoft eVC debugger when connection is severed to the debugger
   //0xBEEFCACE : Used by Microsoft .NET as a magic number in resource files
   //0xCCCCCCCC : Used by Microsoft's C++ debugging runtime library to mark uninitialised stack memory
   //0xCDCDCDCD : Used by Microsoft's C++ debugging runtime library to mark uninitialised heap memory
   //0xDEADDEAD : A Microsoft Windows STOP Error code used when the user manually initiates the crash.
   //0xFDFDFDFD : Used by Microsoft's C++ debugging heap to mark "no man's land" guard bytes before and after allocated heap memory
   //0xFEEEFEEE : Used by Microsoft's HeapFree() to mark freed heap memory
   //
   ///<summary>
   ///Test ptr is one of the pre-defined MSFT bad pointer values.  Note this DOES NOT 
   ///use the IsBadWritePtr().  The ONLY USE OF THIS IS TO ABORT THE PROGRAM IF BAD.
   ///</summary>
   static inline bool isReservedBadPtr ( void* ptr ) {
      switch( DWORD(ptr) )
      {
      case 0x00000000:
      case 0xABABABAB:
      case 0xABADCAFE:
      case 0xBAADF00D:
      case 0xBADCAB1E:
      case 0xBEEFCACE:
      case 0xCCCCCCCC:
      case 0xCDCDCDCD:
      case 0xDEADDEAD:
      case 0xFDFDFDFD:
      case 0xFEEEFEEE:
         return true;
         break;
      
      }
      return false;
   }


   ///<summary>Accessory function to test SUCCEEDED(hresult)</summary>
   static inline bool HR_SUCCEEDED ( DWORD hr ) { return SUCCEEDED(hr) ? true : false; }

   ///<summary>
   ///Accessory function to (at runtime) return size of an array reference
   ///Will not compile if used (incorrectly) with a pointer argument 
   ///NOTE: Deprecated: Use standard _countof() template
   ///</summary>
   template <typename T, size_t N>
   static inline size_t NUM_ELEMENTS( T (&array)[N] ) { return N; }



   ///<summary>
   ///Accessory function to safely release an IUnknown derived object
   ///and set the reference variable to NULL
   ///</summary>
   template <typename T>
   static inline void ifRelease( T*& p ) { if (p) p->Release(); p = NULL; }


   //
   // DEBUG OUTPUT METHODS
   //
   //enum Verbosity { vDebugMax = 0, vDebugMedium, vDebugMin, vHigh, vWarning, vMedium, vSevere, vFatal, vMinimum };
   enum Verbosity { DEBUG0, DEBUG1, DEBUG2, DEBUG3, MEDIUM, WARNING, SEVERE, FATAL, ALWAYS };

   ///<summary>Set global debugMsg verbosity level</summary>
   static void setVerbosity ( Verbosity vLevel ) { ourVerbosityLevel = vLevel; }


   ///<summary>Formats and returns a string ala printf</summary>
   static tstring formatString ( const TCHAR* formatStr, ... );

   ///<summary>Formats and prints debug message using OutputDebugString </summary>
   static bool debugMsg ( const TCHAR* formatStr, ... );
   static bool debugMsg ( Verbosity vLevel, const TCHAR* formatStr, ... );

   ///<summary>Formats and prints debug message using OutputDebugString, with one level of indirection.
   ///Use this in situations where debugMsg() is called from a function which itself takes a format string
   ///and variable parameters, i.e.,   void exFunc ( bool b, const TCHAR* fmtStr, ... ) { debugMsg(&amp;fmtStr); } </summary>
   static bool debugMsg ( const TCHAR** formatStr );
   static bool debugMsg ( Verbosity vLevel, const TCHAR** formatStr );

   ///<summary>Formats and prints debug message using OutputDebugString </summary>
   static bool debugMsgBox ( HWND hWnd, const TCHAR* formatStr, ... );

   ///<summary>Formats specified lastError (a Windows system error)
   /// and outputs to the debug monitor.</summary>
   static bool debugLastError ( DWORD lastError, const TCHAR* formatStr, ... );

   ///<summary>Calls GetLastError(), format system error message.</summary>
   static bool debugLastError ( const TCHAR* formatStr, ... );

   ///<summary>Calls GetLastError(), format system error message.</summary>
   static tstring formatSysError ( DWORD sysErr );

   /// <summary> winMsgToString() takes a windows message structure and forms a printable
   /// string in the form "WM_?????: hwnd=0xXXXX (wParam,lParam)", with user
   /// definable prefix and suffix strings.
   /// </summary>
   static tstring winMsgToString( const MSG* pMsg, 
                                  const TCHAR* prefix = _T(""), 
                                  const TCHAR* suffix = _T("\n") );

   /// <summary>winErrorToString() takes a windows error define (see GetLastError()) and 
   /// converts it to string form - ie., ERROR_SUCCESS converts to "ERROR_SUCCESS".
   /// NOTE: Unsorted and HUGE until re-implemented!
   /// </summary>
   static tstring Utils::winErrorToString ( DWORD err );


   ///<summary> debugWinMsg() provides the simplistic case where a windows message
   /// is immediately printed to the Debug windows. </summary>
   static void debugWinMsg ( const MSG* pMsg );


   ///<summary> Utils::tcharToStr() is for TCHAR-to-char conversion, as needed for API's which
   /// depend on char* or string, and have not wchar_t or wstring versions.
   ///</summary>
   static std::string Utils::tcharToStr ( const TCHAR* tPtr );


   ///<summary> 
   /// CallTracer is used to declare local block variables that automatically log debugMsg's
   /// for entry and exit.  Usage:
   ///      CallTracer t(_T(__FUNCTION__) );  // logs "ENTR: class::function" and EXIT:same
   ///</summary>
   class CallTracer {
   public:
      CallTracer ( const TCHAR* msg = _T("???") ) : pMsg(msg) {
         debugMsg( _T("ENTR: %s\n"), pMsg );
      }
      ~CallTracer ( ) {
         debugMsg( _T("EXIT: %s\n"), pMsg );
      }
      const TCHAR* pMsg;
   };


   //
   // High-Resolution Timer methods
   //
   static unsigned      ourHRTResolution;     // HRT in *nano-seconds*.
   static LARGE_INTEGER ourHRTFrequency;
   static LARGE_INTEGER ourHRTStart;
   static LARGE_INTEGER ourHRTDeltaStart;

   ///<summary> 
   /// highResClockInit() must be called before highRes methods can be called.
   /// Additional calls to highResClockInit() have no effect
   /// Note: static member variables ourHRTFrequency and ourHRTStart are public,
   /// but should not be modified except via the Init routine.
   ///</summary>
   ///<returns>highResClockInit returns FALSE if HighRes timing is unavailable</returns>
   static bool highResClockInit();
   
   ///<summary> 
   /// highResClock() returns the current time in milliseconds, since CPU was
   /// powered on.  Rollover is ~200yrs, so it shouldn't be an issue for games :-)
   /// Note: (WINAPI) calling convention gives prototype compatibility with GetTickCount.
   /// Results are indeterminate if Init has not been called or HRTiming is unavilable.
   ///</summary>
   static DWORD WINAPI highResClock ();
   
   ///<summary> 
   /// highResClockDelta() returns time in milliseconds since highResClockDelta()
   /// was last called.   A *NON-THREAD-SAFE* static internal variable keeps track
   /// of the "starting" point, and is reset only when
   ///      1) <param>resetMinLimit</param> == -1 (default case, per call delta mode)
   ///      2) <param>resetMinLimit</param>  &lt;= returnValue (limit timing mode)
   /// Results are indeterminate if Init has not been called or HRTiming is unavilable.
   ///</summary>
   static DWORD WINAPI highResClockDelta ( int resetMinLimit = -1 );
   

   //
   // Keyboard Methods
   //

   ///<summary>Checks key status using Windows::GetAsyncKeyState(), true if down</summary>
   static inline bool keyIsDown ( unsigned vk_code, unsigned vk_chordKey = -1 ) {
      bool bKeyPress = ((GetAsyncKeyState(vk_code) & 0x8000) ? true : false );
      bool bChordPress = (vk_chordKey == -1) ? true : !!(GetAsyncKeyState(vk_chordKey) & 0x8000);
      return( bKeyPress && bChordPress );
   }
   
   ///<summary>Checks key status using Windows::GetAsyncKeyState(), true if 
   /// pressed since LAST call to keyPressed() </summary>
   static inline bool keyIsPressed ( unsigned vk_code, unsigned vk_chordKey = -1 ) { 
      bool bChordPress = (vk_chordKey == -1) ? true : !!(GetAsyncKeyState(vk_chordKey) & 0x8000);
      bool bKeyPress = bChordPress && ((GetAsyncKeyState(vk_code) & 1) ? true : false );
      return( bKeyPress );
   }
   

   // Math helper functions

   ///<summary>swap(i,j) swaps the contents of two variables</summary>
   static inline void swap ( int& i, int& j ) {
      register int t = i;
      i = j;
      j = t;
   }

   ///<summary>degToRads() converts degrees to radians.</summary>
   static inline double degToRads ( double degrees ) {
      return ( degrees * (M_PI/180.0) );
   }

   // C99 defines several macros for handling floating point numbers without 
   // causing a FPU operation, which *may* give improved performance. 

   ///<summary>Extract sign bit from floating point w/o fp operation </summary>
   static inline int signbitf ( float f ) {
      return( int( int(f) & 0x80000000 ) );
   }

   ///<summary>Change sign of FP 'val' to sign of signval.</summary>
   static inline double copysignf( float val, float signval) {
      return( (int(val) & 0x7FFFFFFF) | signbitf(signval) );
   }

   ///<summary>Do two FP values have same sign?</summary>
   static inline bool samesignf( float f1, float f2 ) {
      return( signbitf(f1) == signbitf(f2) );
   }



   //
   // Random number methods
   //

   ///<summary>rand() overload to return [min..max], inclusive</summary>
   static inline int rand ( int min, int max )
   {
      if ( min > max ) swap( min, max );
      return( min + ( ((max-min)==0) ? 0 : ( ::rand() % (max-min)) + 1) );
   }

   ///<summary>rand() overload to return [0..n-1], inclusive</summary>
   static inline int rand ( unsigned n )
   {
      return( ::rand() % n);
   }


   ///<summary>double floating-point overload for rand(), returns [0..1.0] range.</summary>
   static inline double Utils::drand ( void )
   {
      return( (double(::rand()) / RAND_MAX ) );
   }

   ///<summary>double overload for rand(), returns [0..max];</summary>
   static inline double Utils::drand ( double max )
   {
      return( drand() * max );
   }

   ///<summary>double overload for rand(), returns [min..max] </summary>
   static inline double Utils::drand ( double min, double max )
   {
      double rdiff = drand(fabs(max-min));
      return ((min<max) ? (min+rdiff) : (max+rdiff));
   }


   // RECTANGLE Functions
   static inline RECT makeRect ( int x, int y, int w, int h ) {RECT r = { x, y, x+w, y+h }; return r; }

   // RECTANGLE Functions
   static inline RECT makeRectLTRB ( int l, int t, int r, int b ) { RECT rect = { l, t, r, b }; return rect; }

   // RECTANGLE Functions
   static inline RECT superRect ( const RECT& r1, const RECT& r2 )
   {
      RECT sRect = Utils::makeRectLTRB( min( r1.left, r2.left ), 
                                        min( r1.top, r2.top ), 
                                        max( r1.right, r2.right), 
                                        max( r1.bottom, r2.bottom ) );
      return sRect;
   }



   // Add two rectangles, explicitly
   static inline RECT addRect ( const RECT& lhs, const RECT& rhs ) {
      RECT r(lhs);
      r.left   += rhs.left;
      r.top    += rhs.top;
      r.right  += rhs.right;
      r.bottom += rhs.bottom;
      return r;
   }

   static inline COLORREF randomRGB ( int rMin = 0, int gMin = 0, int bMin = 0 )
   {
      int r = rand( rMin, 255 );
      int g = rand( gMin, 255 );
      int b = rand( bMin, 255 );
      return RGB( r, g, b );
   }



private:
   Utils ( const Utils& other );
   Utils& operator= ( const Utils& rhs );
};


};  // End namespace