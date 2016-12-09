
#pragma once

#if !defined(__EVENTDELEGATE_H__)
#define __EVENTDELEGATE_H__

#include "NVCUtils/srutil/delegate/delegate.hpp"

/*
   EventDelegate.h includes the necessary srutil header file(s) to provide
   a fairly robust delegate data type, similar to the C# or other dynamic
   language mechanisms used to implement event callbacks to any method,
   function, or static method.  
   
   This is an improvement on the simpler callback to virtual interface
   that is typically used to implement event notifications, which suffers
   from name collisions and an inability to specify more than one 
   event handler function - since only ONE override function can be written.

   The srutil classes are courtesy of, and copyright by Sergey Ryazanov,
   with minor modifications performed locally.

   USAGE:  
      A delegate is a TYPE representing a named function with a programmer defined prototype.

      Thus, we define a delegate type, then make variables of that type
      which store such a named function.  This differs from simple function
      pointers, which are a type representing a function prototype, but 
      not tied to a specific named function.

      STEP 1:  
         typedef the prototype using one of 10 named templates: srutil::Delegate0 through Delegate10.
         The number specifies the number of function parameters in the prototype.
         The template parameters specify the RETURN TYPE and PARAMETER TYPES for the prototype.
         The component which defines the typedef depends on usage - but is usually the
         component which is the SOURCE or SENDER of an event, as it knows what parameters
         are going to be passed to the event handling function represented by the delegate.
         Examples:
            typedef srutil::Delegate2<int, Foo*, int > FooEventDelegate;         // 2 params: (Foo*,int)
            typedef srutil::Delegate1<void*, std::string& > NamedEventDelegate;  // 1 param: (str::string&)

      STEP 1a:
         Each delegate type has a public embedded class type, EventSource, which implements
         multiple delegate handling, if required.  This type should also be typedef, but it
         is not a requirement.  
            Examples:
               typedef FooEventDelegate::EventSource     FooEventSource;
               typedef NamedEventDelegate::EventSource   NamedEventSource;
         
         The purposes of these is to allow declaration of a single source object which 
         can dispatch to multiple delegates of the containing delegate class type.

         The EventSource type defines three additional methods to allow manipulation of
         the multiple delegates associated with the source:
            
               DELEGATE::EventSource::operator+= ( DELEGATE d )   -- Add d to set of sinks listening on event
               DELEGATE::EventSource::operator-= ( DELEGATE d )   -- Remove d from set of sinks (**see note**)

         **NOTE**
            There are restrictions here. 
            1. The delegate return type *should* be void, but this isn't enforced.  However, the
               multi-delegate EventSource ignores the return from called delegates.
            2. The DELEGATE type's uniqueness is per-function - thus, two DELEGATE objects which refer
               to the same method/function *ARE INDISTINGUISHABLE*.
               Thus, adding/removing identical DELEGATEs *IS NOT PREDICTABLE*         

      STEP 2:
         The SOURCE typically declares a variable of the event type, used to store the 
         SINK or RECEIVER function.  Typically this has public method(s) to set the 
         delegate which are called by the SINK which wants to handle the event type.

            FooEventDelegate  fooEvDelegate; // see typedef above

      STEP 3:
         Implement a one or more functions, methods, const methods, or static methods to SINK events.  
         Example (all 4 forms implemented):
            //static method:
            int SomeClass::dummyFooEventStaticMethod ( Foo* p, int val ) { return 0; }
            //function 
            int dummyFooEventFunction ( Foo* p, int val ) { return 0; }
            //class method:
            int SomeClass::dummyFooEventMethod ( Foo* p, int val ) { return 0; }         
            //const class method:
            int SomeClass::dummyFooEventConstMethod ( Foo* p, int val ) const { return 0; }         


      STEP 4:
         The declared delegate  (fooEvDelegate) object must be initialized using one of three 
         special template functions:
            from_method       -  used for initialization to a non-static class method
            from_function     -  used for initialization to a global function or static method.
            from_const_method - used for initialization to a const class method.
         These functions MAKE the delegate objects, specifying actual named functions/methods and
         any required context (i.e., the Class name and instance for methods, actual function name, etc.).
         Notice the from_method forms specify a (this) function parameter. The parameter specifies the instance 
         for which the method will be called (i.e, it IS the this ptr *in* the event method).  It can be *any*
         valid instance pointer, but is often simply the this pointer, since usually the SINK or RECEIVER
         calls the from_ function(s). 
         Example: 
            // In initialization code somewhere, use specific template functions for the 4 forms allowed
            FooEventDelegate d1  = FooEventDelegate::from_function<&SomeClass::dummyFooEventStaticMethod>();
            FooEventDelegate d2  = FooEventDelegate::from_function<&dummyFooEventFunction>();
            FooEventDelegate d3  = FooEventDelegate::from_method<SomeClass,&SomeClass::dummyFooEventMethod>(this);
            FooEventDelegate d4  = FooEventDelegate::from_const_method<SomeClass,&SomeClass::dummyFooEventConstMethod>(this);

         The created delegate is then passed into the SOURCE component, which stores it in its delegate 
         object (fooEvDelegate).  This is usually via some public method implemented in the SOURCE class, something
         like "SourceClass::registerForEvents( FooEventDelegate evHandler )".  
         Example:
            sourceObject.registerForEvents( d3 );     // or without a temporary variable...
            sourceObject.registerForEvents( FooEventDelegate::from_method<SomeClass,&SomeClass::dummyFooEventMethod>(this) );

      STEP 5:
         To SOURCE an event, the SOURCE uses its stored delegate object as a normal function:
         Example:
            fooEvDelegate( NULL, 100 );      // calls the function stored 'in' fooEvDelegate
                                             // assuming above register function stores evHandler to fooEvDelegate...
                                             // the 'event' calls SomeClass::dummyFooEventMethod( NULL, 100 ), 
                                             // and ignores the returned int.

      NOTE:
         Delegate objects have VALUE semantics.  Pass them as value parameters and make copies of the
         value's as needed. 
*/

#endif


