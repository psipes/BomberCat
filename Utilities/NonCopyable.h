#if !defined(_NONCOPYABLE_H_)
#define _NONCOPYABLE_H_
// An inheritable class that removes the copy and assigment functions from the derived classes.
class NonCopyable
{
public:
   NonCopyable ( ) { }
   virtual ~NonCopyable ( ) { };
private:
   NonCopyable ( const NonCopyable& other );
   NonCopyable& operator= ( const NonCopyable& other );
};

#endif //_NONCOPYABLE_H_
