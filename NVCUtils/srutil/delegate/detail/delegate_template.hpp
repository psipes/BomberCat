/*
	(c) Sergey Ryazanov (http://home.onego.ru/~ryazanov)

	Template file. May be included many times with different predefined macros.

   SAS: 
      NOTICE LACK OF RE-INCLUSION PROTECTION: THIS IS INTENTIONAL, PER ABOVE!
*/

#include <list>
#include <iostream>

#if SRUTIL_DELEGATE_PARAM_COUNT > 0
   #define SRUTIL_SEPARATOR_COMMA ,
   #define SRUTIL_SEPARATOR_COLON :
#else
   #define SRUTIL_SEPARATOR_COMMA
   #define SRUTIL_SEPARATOR_COLON
#endif

// see BOOST_JOIN for explanation
// SAS: These macros do token pasting to generate a symbol, like "delegate2" from "delegate" and "2",
//      preprocessor expansion.  The Extra macro - I believe - ensures the macro arguments X & Y
//      are expanded properly BEFORE being pasted together.
#define SRUTIL_DELEGATE_JOIN_MACRO( X, Y) SRUTIL_DELEGATE_DO_JOIN( X, Y )
#define SRUTIL_DELEGATE_DO_JOIN( X, Y ) SRUTIL_DELEGATE_DO_JOIN2(X,Y)
#define SRUTIL_DELEGATE_DO_JOIN2( X, Y ) X##Y

namespace srutil
{
   #define SRUTIL_DELEGATE_CLASS_NAME           SRUTIL_DELEGATE_JOIN_MACRO(Delegate,SRUTIL_DELEGATE_PARAM_COUNT)
   #define SRUTIL_DELEGATE_INVOKER_CLASS_NAME   SRUTIL_DELEGATE_JOIN_MACRO(Delegate_invoker,SRUTIL_DELEGATE_PARAM_COUNT)
	
   template <typename R SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_TEMPLATE_PARAMS>
	   class SRUTIL_DELEGATE_INVOKER_CLASS_NAME;

	template <typename R SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_TEMPLATE_PARAMS>
	   class SRUTIL_DELEGATE_CLASS_NAME
	{
	public:
		typedef R return_type;
		typedef SRUTIL_DELEGATE_INVOKER_CLASS_NAME<R SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_TEMPLATE_ARGS> invoker_type;

      SRUTIL_DELEGATE_CLASS_NAME() : object_ptr(0), stub_ptr(0) {
         // Nothing here...
         // std::cout << __FUNCTION__ "::ctor " << (void*) this << std::endl;
      }

		template <return_type (*TMethod)(SRUTIL_DELEGATE_TEMPLATE_ARGS)>
		static SRUTIL_DELEGATE_CLASS_NAME from_function()
		{
			return from_stub(0, &function_stub<TMethod>);
		}

		template <class T, return_type (T::*TMethod)(SRUTIL_DELEGATE_TEMPLATE_ARGS)>
		static SRUTIL_DELEGATE_CLASS_NAME from_method(T* object_ptr)
		{
			return from_stub(object_ptr, &method_stub<T, TMethod>);
		}

		template <class T, return_type (T::*TMethod)(SRUTIL_DELEGATE_TEMPLATE_ARGS) const>
		static SRUTIL_DELEGATE_CLASS_NAME from_const_method(T const* object_ptr)
		{
			return from_stub(const_cast<T*>(object_ptr), &const_method_stub<T, TMethod>);
		}

		return_type operator()(SRUTIL_DELEGATE_PARAMS) const
		{
			return (*stub_ptr)(object_ptr SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_ARGS);
		}

		operator bool () const
		{
			return stub_ptr != 0;
		}

		bool operator!() const
		{
			return !(operator bool());
		}

      //SAS: Problem with this delegate concept is it doesn't support proper equality.
      //     This equality operator doesn't identify a specific registration *instance*,
      //     but a specific registration of a specific FUNCTION or METHOD/INSTANCE.  
      //     Thus, two delegates which refer to the same FUNCTION are equal.
      //     For non-static methods, instance AND method identify a delegate uniquely, so
      //       equality is true if same method AND same instance.
      bool operator== ( const SRUTIL_DELEGATE_CLASS_NAME& other ) const
      {
         return (this->stub_ptr == other.stub_ptr) && (this->object_ptr == other.object_ptr);
      }

      std::string to_string ( ) 
      {
         std::stringstream ss;
         ss << "[this=" << (void*) this << "; stub="
            << (void*) stub_ptr << "; obj="
            << (void*) object_ptr << "]";
         return ss.str();
      }

		
		typedef return_type (SRUTIL_DELEGATE_CALLTYPE *stub_type)(void* object_ptr SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_PARAMS);

	private:

		void* object_ptr;
		stub_type stub_ptr;


		static SRUTIL_DELEGATE_CLASS_NAME from_stub(void* object_ptr, stub_type stub_ptr)
		{
			SRUTIL_DELEGATE_CLASS_NAME d;
			d.object_ptr = object_ptr;
			d.stub_ptr = stub_ptr;
			return d;
		}

		template <return_type (*TMethod)(SRUTIL_DELEGATE_TEMPLATE_ARGS)>
		static return_type SRUTIL_DELEGATE_CALLTYPE function_stub(void* SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_PARAMS)
		{
			return (TMethod)(SRUTIL_DELEGATE_ARGS);
		}

		template <class T, return_type (T::*TMethod)(SRUTIL_DELEGATE_TEMPLATE_ARGS)>
		static return_type SRUTIL_DELEGATE_CALLTYPE method_stub(void* object_ptr SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_PARAMS)
		{
			T* p = static_cast<T*>(object_ptr);
			return (p->*TMethod)(SRUTIL_DELEGATE_ARGS);
		}

		template <class T, return_type (T::*TMethod)(SRUTIL_DELEGATE_TEMPLATE_ARGS) const>
		static return_type SRUTIL_DELEGATE_CALLTYPE const_method_stub(void* object_ptr SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_PARAMS)
		{
			T const* p = static_cast<T*>(object_ptr);
			return (p->*TMethod)(SRUTIL_DELEGATE_ARGS);
		}



   public:
      //SAS: ---------------- EMBEDDED CLASS FOR MULTI-DELEGATE NOTIFICATIONS -----------------------------------
      class EventSource 
      {
      public:

         //SAS Problem here is that multi-event model can't return a single return value...
         void operator()(SRUTIL_DELEGATE_PARAMS) 
		   {
            std::list<SRUTIL_DELEGATE_CLASS_NAME>::iterator iter;
            for ( iter = delegateList.begin(); iter != delegateList.end(); iter++ )
            {
               if ( *iter == true )
               {
                  // R ret = 
                  (*iter)(SRUTIL_DELEGATE_ARGS);
               }
            }
		   }

         void clear ( )
         {
            delegateList.clear();
         }

         void operator+= ( SRUTIL_DELEGATE_CLASS_NAME t ) 
         {
            delegateList.push_back( t );
         }

         void operator-= ( SRUTIL_DELEGATE_CLASS_NAME t ) 
         {
            std::list<SRUTIL_DELEGATE_CLASS_NAME>::iterator iter;
            for ( iter = delegateList.begin(); iter != delegateList.end(); iter++ )
            {
               if ( *iter == t )
               {
                  delegateList.erase( iter );
                  return;
               }
            }
         }

      private:
         std::list<SRUTIL_DELEGATE_CLASS_NAME> delegateList;
      };


	};


   // Helper class for invoking the delegate - SAS???
	template <typename R SRUTIL_SEPARATOR_COMMA SRUTIL_DELEGATE_TEMPLATE_PARAMS>
	   class SRUTIL_DELEGATE_INVOKER_CLASS_NAME
	{
		SRUTIL_DELEGATE_INVOKER_DATA

	public:
		SRUTIL_DELEGATE_INVOKER_CLASS_NAME(SRUTIL_DELEGATE_PARAMS) SRUTIL_SEPARATOR_COLON  SRUTIL_DELEGATE_INVOKER_INITIALIZATION_LIST
		{
		}

		template <class TDelegate>
		R operator()(TDelegate d) const
		{
			return d(SRUTIL_DELEGATE_ARGS);
		}
	};

}

#undef SRUTIL_DELEGATE_CLASS_NAME
#undef SRUTIL_SEPARATOR_COMMA
#undef SRUTIL_SEPARATOR_COLON
#undef SRUTIL_DELEGATE_JOIN_MACRO
#undef SRUTIL_DELEGATE_DO_JOIN
#undef SRUTIL_DELEGATE_DO_JOIN2
