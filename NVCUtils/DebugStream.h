#pragma once

#include <iostream>
#include <strstream>
#include "NVCUtils/tiostream.h"
#include "NVCUtils/tstring.h"



namespace NVCUtils {


   class DebugStreambuf : public tstreambuf
   {
   public:
      DebugStreambuf( void );
      ~DebugStreambuf ( void );
      virtual int overflow ( int ch );
      virtual int sync ( void );
      virtual std::streamsize xsputn ( const TCHAR* s, std::streamsize n );

   protected:
      void Flush();
      inline int out_waiting()  { return  pptr() ? (int)(pptr() - pbase()) : 0; }

      unsigned m_BufferSize;
      TCHAR* m_pBuffer;
   };



   class DebugStream : public tostream
   {
   public:
      DebugStream( void );
   protected:
      DebugStreambuf m_buf;
   };


   tostream& DebugFlush( tostream& os );

   extern DebugStream  dout;

};