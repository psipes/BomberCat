
#include <iostream>
#include <iomanip>
#include <sstream>
#define WIN32_EXTRA_LEAN
#define DIRECTINPUT_VERSION 0x0800
#include <windows.h>
#include <tchar.h>
#include "NVCUtils\NVCUtils.h"
#include "NVCUtils\tstring.h"
#include "NVCUtils\tiostream.h"
#include "NVCUtils\debugstream.h"

// Include C++ source code that defines the Windows message & error tables.
// These HUGE headers should NOT be included in ANY other file.
#include "NVCUtils\WinMsgTable.h"
#include "NVCUtils\WinErrorTable.h"

using namespace std;

//----------------------------------------------------------------------------------------
//-------  Non-Namespaced functions and data structures ----------------------------------
//----------------------------------------------------------------------------------------

tostream& operator<< ( tostream& strm, const RECT& r )
{
   return strm << TCHAR('[') << r.left << TCHAR(',') << r.top << TCHAR(',') << r.right << TCHAR(',') << r.bottom << TCHAR(']');
}


bool operator== ( const RECT& lhs, const RECT& rhs )
{
   bool b = lhs.top == rhs.top;
   b &= (lhs.left == rhs.left);
   b &= (lhs.bottom == rhs.bottom);
   b &= (lhs.right == rhs.right);
   return b;
}  


bool operator!= ( const RECT& lhs, const RECT& rhs )
{
   return !(lhs==rhs);
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

//static:
int NVCUtils::Utils::ourVerbosityLevel = NVCUtils::Utils::MEDIUM;


namespace NVCUtils {

   unsigned      Utils::ourHRTResolution;
   LARGE_INTEGER Utils::ourHRTFrequency;
   LARGE_INTEGER Utils::ourHRTStart;
   LARGE_INTEGER Utils::ourHRTDeltaStart;

   //---------------------------------------------------------------------------------------- 
   //---------------------------------------------------------------------------------------- 
   //---------------------------------------------------------------------------------------- 

   void Utils::testWinMsgDebug ( )
   {
      MSG msg;

      for ( const MESSAGE_STRING_MAP* pMapMsg = allMessages; 
         pMapMsg->lpszMsg != NULL; 
         pMapMsg++ )
      {
         msg.message = pMapMsg->nMsg;
         msg.hwnd = (HWND) 0x12345678;
         msg.lParam = 1;
         msg.wParam = 2;
         Utils::debugWinMsg( &msg );
      }
   }



   static void testRandInt ( int iterations, bool fMinMax, int min, int max, int expectedSum )
   {
      int   minAllowed, maxAllowed;
      int   passedMinMax = true;
      int   sum = 0;
      stringstream ss;


      if ( fMinMax )
      {
         // Test Utils::rand(min,max)
         minAllowed = min;
         maxAllowed = max;

         if ( minAllowed > maxAllowed )
            Utils::swap( minAllowed, maxAllowed );

         ss << "Utils::rand( " << min << ',' << max << " )";

         for ( int ix = 0; ix < iterations; ix++ )
         {
            int ri = Utils::rand( min, max );
            sum += ri;
            if ( ri < minAllowed || ri > maxAllowed )
               passedMinMax = false;
         }
      }
      else
      {
         // Test Utils::rand(max);
         minAllowed = 0;
         maxAllowed = max-1;

         ss << "Utils::rand( " << max << " )";

         for ( int ix = 0; ix < iterations; ix++ )
         {
            int ri = Utils::rand( max );
            sum += ri;
            if ( ri < minAllowed || ri > maxAllowed )
               passedMinMax = false;
         }
      }

      int flags = cout.flags();
      cout << hex << showbase << left;

      if ( (sum == expectedSum) )
         cout << setw(30) << ss.str() << " chksum:pass " << setw(10) << sum  << " min/max:" << (passedMinMax ? "pass" : "FAIL" ) << endl;
      else
         cout << setw(30) << ss.str() << " chksum:FAIL " << setw(10) << sum  << " min/max:" << (passedMinMax ? "pass" : "FAIL" ) << endl;

      cout.flags( flags );

      return;
   }

   bool Utils::testUtils()
   {

      Utils::debugMsg( _T("Utils Test\n") );
      
      Utils::keyIsDown( VK_ESCAPE );
      Utils::keyIsPressed( VK_ESCAPE );


      // Preset RNG to specific sequence
      ::srand( 0xCAFEBABE );


      // testRandInt ( int iterations, bool fMinMax, int min, int max, int expectedSum )
      const int nIters = 10000;


      cout << "Integer Random Number Generator tests\n\n";
      // Test random generated ints.  Sums are computed by 1st run, then hard coded for later checks
      testRandInt( nIters, false, 0, 100,           0x7878a       );           // 2nd param = false uses Utils::rand(max)
      testRandInt( nIters, false, 0, 1397,          0x6acf34      );       
      testRandInt( nIters, true, 10, 20,            0x25e24       );           // 2nd param = true uses Utils::rand(min,max) form
      testRandInt( nIters, true, 10, 10,            0x186a0       );
      testRandInt( nIters, true, 20, 10,            0x25e01       );
      testRandInt( nIters, true, -10, 10,           0x120c        );
      testRandInt( nIters, true, -10, -1,           0xffff3d77    );
      testRandInt( nIters, true, -9771, -10329,     0xfa029def    );
      testRandInt( nIters, true, 7399, 31017,       0xa40c957     );
      testRandInt( nIters, true, 0, 1000,           0x4b30d2      );
      testRandInt( nIters, true, 1, 1000,           0x4ba423      );   
      testRandInt( nIters, true, -16383, 16383,     0xffe7d481    );
      testRandInt( nIters, true, -16384, 16384,     0x1b8131      );
      testRandInt( nIters, true, -16385, 16385,     0xfffa92fc    );
      testRandInt( nIters, true, -24576, 24576,     0xfb2020b2    );
      cout << endl;


      Utils::drand();
      Utils::drand(10.0);
      Utils::drand(0,10);
      Utils::drand(0,0);
      Utils::drand(-10.0,10.0);
      Utils::drand(-10.0,-1.0);


      return true;
   }

   bool Utils::testHighResTiming()
   {
      if ( !Utils::highResClockInit() )
      {
         Utils::debugMsg( _T("Utils::testHighResTiming() - Failed highResClockInit()\n") );
         return false;
      }

      // TEST NORMAL highResClock() usage
      Utils::debugMsg( _T("\nHRTiming Test - start/end using highResClock()\n") );
      for (int ms=1000, i = 2; ms > 0 ; i++)
      {
         unsigned hrStart = Utils::highResClock();
         ::Sleep( ms );
         unsigned hrEnd   = Utils::highResClock();
         Utils::debugMsg( _T("Timing Test: Sleep(%d) ==> (start-end)=%d, since power-on\n"), ms, hrEnd-hrStart, hrEnd );

         ms /= i;
      }

      // TEST HR Delta mode, auto-reset
      Utils::debugMsg( _T("\nHRTiming Test - Auto Reset Mode\n") );
      for (int ms=1000, i = 2; ms > 0 ; i++)
      {
         Utils::highResClockDelta();
         ::Sleep( ms );
         unsigned hrDelta = Utils::highResClockDelta();
         Utils::debugMsg( _T("Timing Test: Sleep(%d) ==> HRT_Delta()=%d\n"), ms, hrDelta );

         ms /= i;
      }


      // TEST Delta mode, interval reset mode w/ and w/o Sleep
      Utils::debugMsg( _T("\nHRTiming Test - Interval Reset Mode\n") );
      int resetLimit = 60;
      const int arrayLen = 25;
      int      ms[ arrayLen ];
      unsigned results[ arrayLen ];

      for (int i = 0; i < arrayLen; i++)  // pre-gen ms[] w/arrayLen random #s 15-25ms
         ms[i] = (15 + Utils::rand(11));


      // TEST Delta Mode w/interval reset, with Sleep
      Utils::debugMsg( _T("\nHRTiming Test - Interval Reset Mode w/Sleep\n") );
      Utils::highResClockDelta();   // Force Reset
      for ( int i = 0; i < arrayLen; i++ )
      {
         ::Sleep( ms[i] );
         results[i] = Utils::highResClockDelta( resetLimit );
      }
      for (int i = 0; i < 25; i++)
         Utils::debugMsg( _T("Timing Test: Sleep(%02.2d) ==> hrClockDelta(%02.2d)=%-3d\n"), ms[i], resetLimit, results[i] );


      // TEST Delta Mode w/interval reset, without Sleep
      Utils::debugMsg( _T("\nHRTiming Test - Interval Reset Mode w/o Sleep\n") );
      resetLimit = 5;
      Utils::highResClockDelta();   // Force Reset
      for ( int i = 0; i < arrayLen; i++ )
      {
         int delta;
         results[i] = 0;
         ms[i] = rand(5, 25 );
         while ( (delta = Utils::highResClockDelta(ms[i])) <= ms[i] )
         {
            results[i] += 1;
         }

      }
      for (int i = 0; i < 25; i++)
         Utils::debugMsg( _T("Timing Test: hrClockDelta(%02.2d) took %d iterations\n"), ms[i], results[i] );





      // TEST Delta Mode w/AutoReset, without Sleep
      Utils::debugMsg( _T("\nHRTiming Test - AutoReset Mode w/o Sleep\n") );
      Utils::highResClockDelta();   // Force Reset
      for ( int i = 0; i < arrayLen; i++ )
      {
         int delta;
         int busyWaitLen = rand( 15000, 75000 );
         while ( (delta = Utils::highResClockDelta()) == 0 )
         {
            for (int i = 0; i < rand(busyWaitLen,10*busyWaitLen); i++)
            {
               //DUMMY TIMING LOOP
            }
            busyWaitLen /= 2;
         }

         results[i] = delta;
      }
      for (int i = 0; i < 25; i++)
         Utils::debugMsg( _T("Timing Test: hrClockDelta(auto)=%-3d\n"), results[i] );



      return true;

   }


   bool Utils::testDebugMessages()
   {
      tstring str;

      str = Utils::formatString( _T("String Format Test [%d] [%s]\n"), 123, _T("456") );
      Utils::debugMsg( str.c_str() );

      Utils::debugMsg( _T("This is a debug message [%d] [%s]\n"), 123, _T("456") );

      Utils::debugMsg( _T("Test ::GetLastError() - expect SUCCESS, INVALID_FUNCTION, FILE_NOT_FOUND, INVALID_PASSWORD, DNS_WINS_ERROR\n") );
      DWORD errs[] = { ERROR_SUCCESS, ERROR_INVALID_FUNCTION, ERROR_FILE_NOT_FOUND, ERROR_INVALID_PASSWORD, DNS_ERROR_NEED_WINS_SERVERS, -1 };
      for ( int i = 0; errs[i] != -1; i++ )
      {
         Utils::debugLastError( errs[i], _T("dle(err) test %d [Err=%d]\n"), i, errs[i] );
         ::SetLastError( errs[i] );
         Utils::debugLastError( errs[i], _T("dle()    test %d [Err=%d]\n"), i, errs[i] );

         tstring str( Utils::formatSysError( errs[i] ) );
         Utils::debugMsg( _T("FormatSysError(%d) == %s\n"), errs[i], str.c_str() );
      }


      MSG msg;
      msg.hwnd = 0;
      msg.wParam = 0x00010002;
      msg.lParam = 0x00030004;
      msg.time = 0;
      msg.pt.x = msg.pt.y = 100;
      msg.message = WM_TIMER;

      str = Utils::winMsgToString( &msg, _T("Prefix: "), _T(" Suffix\n") ); 
      Utils::debugMsg( str.c_str() );

      Utils::debugWinMsg( &msg );


      Utils::debugMsg( _T("%s\n"), Utils::winErrorToString( ERROR_SUCCESS ).c_str() );
      Utils::debugMsg( _T("%s\n"), Utils::winErrorToString( ERROR_INSTALL_SERVICE_SAFEBOOT ).c_str() );
      Utils::debugMsg( _T("%s\n"), Utils::winErrorToString( ERROR_NDIS_INDICATION_REQUIRED ).c_str() );
      Utils::debugMsg( _T("%s\n"), Utils::winErrorToString( ERROR_JOURNAL_DELETE_IN_PROGRESS ).c_str() );

      // Test DebugStream:
      tstringstream  tss;
      tss << _T("ERROR_SUCCESS string is [") << Utils::winErrorToString( ERROR_SUCCESS ) << _T("]\n");
      dout << "dout: " << tss.str() << endl;
      tss << _T("ERROR_INSTALL_SERVICE_SAFEBOOT string is [") << Utils::winErrorToString( ERROR_INSTALL_SERVICE_SAFEBOOT ) << _T("]\n");
      dout << "dout: " << tss.str() << endl;
      tss << _T("ERROR_NDIS_INDICATION_REQUIRED string is [") << Utils::winErrorToString( ERROR_NDIS_INDICATION_REQUIRED ) << _T("]\n");
      dout << "dout: " << tss.str() << endl;
      tss << _T("ERROR_JOURNAL_DELETE_IN_PROGRESS string is [") << Utils::winErrorToString( ERROR_JOURNAL_DELETE_IN_PROGRESS ) << _T("]\n");
      dout << "dout: " << tss.str() << endl;


      return true;
   }


   //---------------------------------------------------------------------------------------- 
   //---------------------------------------------------------------------------------------- 
   //---------------------------------------------------------------------------------------- 

   bool Utils::debugMsg ( const TCHAR* formatStr, ... )
   {
      VFormatHelper vf( formatStr );
      OutputDebugString( vf.c_str() );
      return true;
   }

   bool Utils::debugMsg ( Verbosity vLevel, const TCHAR* formatStr, ... )
   {
      if ( vLevel >= ourVerbosityLevel )
      {
         VFormatHelper vf( formatStr );
         OutputDebugString( vf.c_str() );
      }
      return true;
   }

   bool Utils::debugMsg ( const TCHAR** formatStr )
   {
      VFormatHelper vf( formatStr );
      OutputDebugString( vf.c_str() );
      return true;
   }

   bool Utils::debugMsg ( Verbosity vLevel, const TCHAR** formatStr )
   {
      if ( vLevel >= ourVerbosityLevel )
      {
         VFormatHelper vf( formatStr );
         OutputDebugString( vf.c_str() );
      }
      return true;
   }

   bool Utils::debugMsgBox ( HWND hWnd, const TCHAR* formatStr, ... )
   {
      VFormatHelper vf( formatStr );   
      int mbReturn = ::MessageBox( hWnd, vf.c_str(), _T("Debug"), MB_OKCANCEL );
      return( mbReturn == IDOK );
   }

   tstring Utils::formatString ( const TCHAR* formatStr, ... )
   {
      VFormatHelper vf( formatStr );
      tstring ts( vf.c_str() );
      return ts;

      //return vf.c_str();
   }

   bool Utils::debugLastError ( DWORD lastError, const TCHAR* formatStr, ... )
   {
      VFormatHelper vf( formatStr );
      tstring sysErr( formatSysError( lastError ) );

      Utils::debugMsg( vf.c_str() );
      Utils::debugMsg( _T("ERROR 0x%X: %s"), lastError, sysErr.c_str());

      return true;
   }

   bool Utils::debugLastError ( const TCHAR* formatStr, ... )
   {
      DWORD gle = ::GetLastError();
      VFormatHelper vf( formatStr );
      tstring sysErr( formatSysError( gle ) );

      Utils::debugMsg( vf.c_str() );
      Utils::debugMsg( _T("ERROR 0x%X: %s"), gle, sysErr.c_str());

      return true;
   }

   tstring Utils::formatSysError ( DWORD sysErr )
   {
      DWORD langId = MAKELANGID( LANG_NEUTRAL, SUBLANG_NEUTRAL );
      TCHAR tcBuf[1024];
      DWORD rv;
      tstring rStr;

      rv = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, sysErr, langId, tcBuf, 1024, NULL);
      
      // FormatMessage fails for some error codes (like HRESULTs incorrectly passed here or returned as system errors)
      if ( rv == 0 )
      {
         rv = FormatMessage( FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY, _T("Error Code 0x%1!02X!"), -1, langId, tcBuf, 1024, (va_list*) &sysErr );
      }

      
      if ( rv > 0 )
      {
         rStr = tcBuf;
      }
      else
      {
         rStr = _T("Utils::formatSysError() fail");
      }

      return rStr;
   }



   //---------------------------------------------------------------------------------------- 

   tstring Utils::winMsgToString ( const MSG* pMsg, 
                                   const TCHAR* prefix, 
                                   const TCHAR* suffix )
   {
      if ( pMsg == NULL )
         return( _T("Null pMsg") );

      if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE ||
         pMsg->message == WM_NCHITTEST || pMsg->message == WM_SETCURSOR ||
         pMsg->message == WM_CTLCOLORBTN ||
         pMsg->message == WM_CTLCOLORDLG ||
         pMsg->message == WM_CTLCOLOREDIT ||
         pMsg->message == WM_CTLCOLORLISTBOX ||
         pMsg->message == WM_CTLCOLORMSGBOX ||
         pMsg->message == WM_CTLCOLORSCROLLBAR ||
         pMsg->message == WM_CTLCOLORSTATIC ||
         pMsg->message == WM_ENTERIDLE || 
         pMsg->message == WM_CANCELMODE ||
         pMsg->message == 0x0118)    // WM_SYSTIMER (caret blink)
      {
         // don't report very frequently sent messages
         return( _T("") );
      }

      LPCTSTR lpszMsgName = NULL;
      TCHAR szBuf[80];

      // find message name
      if (pMsg->message >= 0xC000)
      {
         // Window message registered with 'RegisterWindowMessage' (actually a USER atom)
         if (::GetClipboardFormatName(pMsg->message, szBuf, _countof(szBuf)))
            lpszMsgName = szBuf;
      }
      else if (pMsg->message >= WM_APP)
      {
         // User message
         _stprintf_s( szBuf, _countof(szBuf), _T("WM_APP+0x%04X"), pMsg->message - WM_APP);
         lpszMsgName = szBuf;
      }
      else if (pMsg->message >= WM_USER)
      {
         // User message
         _stprintf_s(szBuf, _countof(szBuf), _T("WM_USER+0x%04X"), pMsg->message - WM_USER);
         lpszMsgName = szBuf;
      }
      else
      {
         // a system windows message
         for ( const MESSAGE_STRING_MAP* pMapMsg = allMessages; pMapMsg->lpszMsg != NULL; pMapMsg++ )
         {
            if (pMapMsg->nMsg == pMsg->message)
            {
               lpszMsgName = pMapMsg->lpszMsg;
               break;
            }
         }

         if (lpszMsgName == NULL)
         {
            _stprintf_s( szBuf, _countof(szBuf), _T("Unknown (0x%4X)"), pMsg->message );
         }
      }


      TCHAR  szRetBuf[255];

      if (lpszMsgName)
      {
         _stprintf_s( szRetBuf, _countof(szRetBuf), _T("%s%s: hwnd=0x%08X (0x%08X, 0x%08X)%s"),
                      prefix, lpszMsgName, pMsg->hwnd, pMsg->wParam, pMsg->lParam, suffix);
      }
      else 
      {
         _stprintf_s( szRetBuf, _countof(szRetBuf), _T("%s%s: hwnd=0x%08X (0x%08X, 0x%08X)%s"),
                      prefix, lpszMsgName, pMsg->hwnd, pMsg->wParam, pMsg->lParam, suffix);
      }

      return( szRetBuf );
   }

   //---------------------------------------------------------------------------------------- 

   void Utils::debugWinMsg ( const MSG* pMsg )
   {
      OutputDebugString( Utils::winMsgToString( pMsg ).c_str() );
   }


   //---------------------------------------------------------------------------------------- 

   tstring Utils::winErrorToString ( DWORD err )
   {
      LPCTSTR lpszErrName = NULL;

      // a system error message
      for ( const ERROR_STRING_MAP* pMapMsg = allWinErrors; 
            pMapMsg->lpszMsg != NULL; 
            pMapMsg++ )
      {
         if ( pMapMsg->err == err )
         {
            lpszErrName = pMapMsg->lpszMsg;
            break;
         }
      }

      if ( lpszErrName == NULL )
         lpszErrName = _T("Err not found");

      return tstring( lpszErrName );
   }

   //---------------------------------------------------------------------------------------- 
   //---------------------------------------------------------------------------------------- 
   //---------------------------------------------------------------------------------------- 

   DWORD WINAPI Utils::highResClock ()
   {
      LARGE_INTEGER li;
      ::QueryPerformanceCounter( &li );

      LONGLONG diff = ( li.QuadPart - ourHRTStart.QuadPart );
      DWORD mSecs = DWORD(diff / ourHRTFrequency.QuadPart);
      return mSecs;
   }

   DWORD WINAPI Utils::highResClockDelta ( int resetMinLimit )
   {
      LARGE_INTEGER li;
      ::QueryPerformanceCounter( &li );

      LONGLONG diff = (li.QuadPart - ourHRTDeltaStart.QuadPart);
      DWORD mSecs = DWORD(diff / ourHRTFrequency.QuadPart);

      if ( resetMinLimit <= -1 || (mSecs >= unsigned(resetMinLimit)) )
         ourHRTDeltaStart.QuadPart = li.QuadPart;

      return mSecs;
   }

   bool Utils::highResClockInit ()
   {
      if ( ::QueryPerformanceFrequency( &ourHRTFrequency ) ) 
      {
         Utils::debugMsg( _T("QueryPerformanceFrequency: %lld tics/sec\n"), ourHRTFrequency.QuadPart );

         ::QueryPerformanceCounter( &ourHRTStart );
         ourHRTDeltaStart = ourHRTStart;
         
         ourHRTResolution = unsigned( 1000000000LL / ourHRTFrequency.QuadPart );     // resolution of HRT in nano-seconds.

         ourHRTFrequency.QuadPart /= 1000LL;   //pre-Scale frequency to give MS.

         return true;
      }

      return false;
   }




   // Specifically return basic_char string instead of tstring for 
   // api's which require char*'s
   std::string Utils::tcharToStr ( const TCHAR* tPtr )
   {
      // Handy for converting TCHAR to std::string (char)
      // If the conversion fails, an empty string is returned.
      std::string str;

#ifdef _UNICODE
      // calculate the size of the char string required
      // Note: If wcstombs encounters a wide character it cannot convert
      //       to a multibyte character, it returns –1.


      //int len = 1 + wcstombs(0, t, 0);
      size_t len;
      
      wcstombs_s( &len, NULL, 0, tPtr, 0 );  // computes total length in bytes, including null terminator
      if (0 == len) 
         return str;
   
      char localBuffer[512];
      char* charBuffer = (len > 512) ? new char[len] : localBuffer;
      charBuffer[0] = '\0';
      wcstombs_s( NULL, charBuffer, len, tPtr, len );  // computes total length in bytes, including null terminator

      str = charBuffer;
      
      if ( charBuffer != localBuffer )
         delete [] charBuffer;

#else
      str = tPtr;
#endif

      return str;
   }



}; // END NAMESPACE