/*
	(c) Sergey Ryazanov (http://home.onego.ru/~ryazanov)

	Fast delegate compatible with C++ Standard.

   Minor mods from orginal - SAS
      Remove a lot of #ifdef's used to conditionally compiler for "prefered" syntax, which
      depends on variadic templates and other C++11 or Boost features.
      As is, the "less pretty" - but fully portable to GCC, VC, etc. - syntax is used.
      This is an example of the two syntaxes:

            
*/
#ifndef SRUTIL_DELEGATE_INCLUDED
#define SRUTIL_DELEGATE_INCLUDED

namespace srutil
{
}

#ifdef _MSC_VER
   #define SRUTIL_DELEGATE_CALLTYPE __fastcall
#else
   #define SRUTIL_DELEGATE_CALLTYPE
#endif

// Including delegate_list.hpp brings in the MANY templates used to handle
// variable numbers of parameters in a delegate prototype.
#include "detail/delegate_list.hpp"


#undef SRUTIL_DELEGATE_CALLTYPE

#endif//SRUTIL_DELEGATE_INCLUDED
