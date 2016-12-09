#if !defined(_POINT_H_)
#define _POINT_H_
#include <windows.h>
class Point : public POINT
{
public:
   Point ( );
   Point ( LONG x, LONG y );
   ~Point ( );

   void set ( LONG x, LONG y );

private:
   POINT myPoint;
};

#endif //_POINT_H_