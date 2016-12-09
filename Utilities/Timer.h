#if !defined(_TIMER_H_)
#define _TIMER_H_
#include "Utilities/NonCopyable.h"
#include <windows.h>


class Timer //: public NonCopyable
{
public:
   static DWORD systemTick ( ) { return GetTickCount(); }
   static unsigned long long systemTick64 ( ) { return GetTickCount64(); }

   Timer ( );
   ~Timer ( );
   bool start ( );
   unsigned long long stop ( );
   unsigned long long restart ( );
   unsigned long long elapsedTime ( );    
   bool isRunning ( ) { return myTimerIsRunning; }

private:
   bool               myTimerIsRunning;
   unsigned long long myStartingTime;
};

#endif //_TIMER_H_