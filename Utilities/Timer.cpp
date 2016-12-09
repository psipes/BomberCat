#include "Utilities/Timer.h"

//=======================================================================
Timer::Timer ( )
:myTimerIsRunning(false), myStartingTime(0)
{
}

//=======================================================================
Timer::~Timer ( )
{
}

//=======================================================================
bool Timer::start ( )
{
   if ( !myTimerIsRunning )
   {
      myTimerIsRunning = true;
      myStartingTime = GetTickCount64( );
   }
   return myTimerIsRunning;
}

//=======================================================================
unsigned long long Timer::stop ( )
{
   if ( myTimerIsRunning )
   {
      myTimerIsRunning = false;
      return GetTickCount64( ) - myStartingTime;
   }
   return 0;
}

//=======================================================================
unsigned long long Timer::restart ( )
{
   if ( myTimerIsRunning )
   {
      unsigned long long delta = GetTickCount64( ) - myStartingTime;
      myStartingTime = GetTickCount64 ( );
      return delta;
   }
   return 0;
}

//=======================================================================
unsigned long long Timer::elapsedTime ( )
{
   if ( myTimerIsRunning )
   {
      return GetTickCount64 ( ) - myStartingTime;
   }
   return 0;
}