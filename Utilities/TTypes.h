#if !defined(_TTYPE_H_)
#define _TTYPE_H_
#include <windows.h>
#include <string>
#include <fstream>
#include <sstream>

#if defined(UNICODE)// Is UNICODE
#define TSPRINTF_S swprintf_s
#define VTSPRINTF_S vswprintf_s
typedef std::wstring tstring;
typedef std::wfstream tfstream;
typedef std::wstringstream tstringstream;
#else // NOT UNICODE
#define TSPRINTF_S sprintf_s
#define VTSPRINTF_S vsprintf_s
typedef std::string tstring;
typedef std::fstream tfstream;
typedef std::ifstream tifstream;
typedef std::ofstream tofstream;
typedef std::stringstream tstringstream;
#endif // UNICODE

#define XOR(A,B) ((!!A)^(!!B))

#endif //_TTYPE_H_