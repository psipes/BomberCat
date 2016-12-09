
//------------------------------------------------------------------------------------------
// tiostream.h
//    Expanded TCHAR-based type (and a few variable) definitions.
//    TCHAR provide Unicode/MultiByte transparency for (many) C-language and Win32 items.
//    tiostream extends this to (many) C++ std:: namespace items, including the
//    majority of std::iostream and std::string
//
// Revision History
//
//    28FEB2015      SAS      Original version
//   
//------------------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "tchar.h"


// wide-string representations of C/C++ defines:
#define __TFUNCTION__   TEXT(__FUNCTION__)
#define __TFILE__       TEXT(__FILE__)


// Removal of using namespace std; leads to some problems
// compiling items - this keeps backwards compatibility
using std::endl;


typedef std::basic_ios<TCHAR, std::char_traits<TCHAR>>                  TIOS;            // ios
typedef std::basic_streambuf<TCHAR, std::char_traits<TCHAR>>            TSTREAMBUF;      // streambuf
typedef std::basic_istream<TCHAR, std::char_traits<TCHAR>>              TISTREAM;        // istream
typedef std::basic_ostream<TCHAR, std::char_traits<TCHAR>>              TOSTREAM;        // ostream
typedef std::basic_iostream<TCHAR, std::char_traits<TCHAR>>             TIOSTREAM;       // iostream
typedef std::basic_filebuf<TCHAR, std::char_traits<TCHAR>>              TFILEBUF;        // filebuf
typedef std::basic_ifstream<TCHAR, std::char_traits<TCHAR>>             TIFSTREAM;       // ifstream;
typedef std::basic_ofstream<TCHAR, std::char_traits<TCHAR>>             TOFSTREAM;       // ofstream;
typedef std::basic_fstream<TCHAR, std::char_traits<TCHAR>>              TFSTREAM;        // fstream;

typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,        \
                          std::allocator<TCHAR>>	                     TSTRING;         // string
typedef std::basic_stringbuf<TCHAR, std::char_traits<TCHAR>,     \
                             std::allocator<TCHAR>>                     TSTRINGBUF;      // stringbuf
typedef std::basic_istringstream<TCHAR, std::char_traits<TCHAR>, \
                                 std::allocator<TCHAR>>                 TISTRINGSTREAM;  // istringstream
typedef std::basic_ostringstream<TCHAR, std::char_traits<TCHAR>, \
                                 std::allocator<TCHAR>>                 TOSTRINGSTREAM;  // ostringstream
typedef std::basic_stringstream<TCHAR, std::char_traits<TCHAR>,  \
                                std::allocator<TCHAR>>                  TSTRINGSTREAM;   // stringstream

typedef TIOS            tios;          
typedef TSTREAMBUF      tstreambuf;    
typedef TISTREAM        tistream;      
typedef TOSTREAM        tostream;      
typedef TIOSTREAM       tiostream;     
typedef TFILEBUF        tfilebuf;      
typedef TIFSTREAM       tifstream;     
typedef TOFSTREAM       tofstream;     
typedef TFSTREAM        tfstream;      
typedef TSTRING         tstring;       
typedef TSTRINGBUF      tstringbuf;    
typedef TISTRINGSTREAM  tistringstream;
typedef TOSTRINGSTREAM  tostringstream;
typedef TSTRINGSTREAM   tstringstream; 

// Extern defines for std input/output streams
extern tistream&     tcin;
extern tostream&     tcout;
