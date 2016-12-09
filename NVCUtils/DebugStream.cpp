
#include <cassert>
#include <tchar.h>

#include <windows.h>

#include "DebugStream.h"

namespace NVCUtils {

// Single debugging stream ala "cout"
DebugStream  dout;

#define DEBUG_BUFFER_SIZE (1024)


//----------------------------------------------------------------------------------------

DebugStream::DebugStream( void )
: m_buf(), tostream((tstreambuf*) &m_buf )  
{
   assert( this );
}


//----------------------------------------------------------------------------------------



DebugStreambuf::DebugStreambuf ( void ) : tstreambuf()
{
   assert( this );
   m_BufferSize = DEBUG_BUFFER_SIZE;            // store the buffer size
   m_pBuffer = new TCHAR[ m_BufferSize + 1 ];    // one extra for NULL terminator
   assert(m_pBuffer );                          // paranoia check

   setp( m_pBuffer, m_pBuffer + m_BufferSize ); // init put pointers
}

DebugStreambuf::~DebugStreambuf ( void )
{
   delete [] m_pBuffer;
}

std::streamsize DebugStreambuf::xsputn (const TCHAR* s, std::streamsize n)
{
   int nPut = 0;
   for (int i = 0; i < n; i++)
   {
      if ( tstreambuf::sputc( s[i] ) == EOF )
         break;

      nPut++;
      if ( s[i] == '\n' )
         Flush();
   }
   return nPut;
}

int DebugStreambuf::overflow( int ch )
{
   assert( this );
   int nChars = out_waiting();
   if ( nChars ) {
      Flush();                               // flush data in buffer
      return sputc( ch );                    // then put the char
   }
   return ch;
}

int DebugStreambuf::sync()
{
   assert( this );
   int nChars = out_waiting();
   if ( nChars )
      Flush();

   return tstreambuf::sync();
}

void DebugStreambuf::Flush()
{
   assert( this );

   TCHAR* p = pptr() - 1;      // get pointer to last character
   *pptr() = '\0';                       // NULL terminate

   ::OutputDebugString( m_pBuffer );

   setp( m_pBuffer, m_pBuffer + m_BufferSize ); // reset buffer pointers
}


tostream& DebugFlush( tostream& os )
{
   return os << "\r\n" << std::flush;
}

};