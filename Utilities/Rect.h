#if !defined(_RECT_H_)
#define _RECT_H_
#include <windows.h>

class Rect : public RECT
{
public:
   Rect ( );
   Rect ( const RECT& other )
   {
      left = other.left;
      top = other.top;
      right = other.right;
      bottom = other.bottom;
   }
   Rect ( LONG left, LONG top, LONG right, LONG bottom );
   Rect ( POINT topleft, LONG width, LONG height );
   ~Rect ( );

   Rect& operator= ( const RECT& other )
   {
      left = other.left;
      top = other.top;
      right = other.right;
      bottom = other.bottom;
      return *this;
   }

   bool operator== ( const Rect& other ) {
      return (top == other.top) && (left == other.left) && (bottom == other.bottom) && (right == other.right);
   }

   bool operator!= ( const Rect& other ) {
      return !(operator==( other ));
   }



   LONG area ( ) const;

   //todo GET RID OF THESE (for now)
   bool collidesWith ( const RECT& other ) const;
   bool collidesWith ( const RECT& other, Rect& intersect ) const;
   bool contains ( const POINT& point ) const;
   bool contains ( LONG x, LONG y ) const;

   void normalize ( );

   //todo: and make it readable like this first one!!!!
   inline LONG x ( ) const { return left; }

   inline LONG y ( ) const { return top; }

   inline LONG width ( ) const { return (right - left); }

   inline LONG height ( ) const { return (bottom - top); }

   void set ( LONG left, LONG top, LONG right, LONG bottom );
   void set ( POINT topleft, LONG width, LONG height );
   LONG x ( LONG value, bool relative = false );
   LONG y ( LONG value, bool relative = false );
   LONG width ( LONG value );
   LONG height ( LONG value );
};
#endif //_RECT_H_