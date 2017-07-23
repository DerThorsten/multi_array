#pragma once

#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <limits>
#include <cmath>




/** \def MULTI_ARRAY_CHECK_OP(a,op,b,message)
    \brief macro for runtime checks
    
    \warning The check is done 
        <B> even in Release mode </B> 
        (therefore if NDEBUG <B>is</B> defined)

    \param a : first argument (like a number )
    \param op : operator (== )
    \param b : second argument (like a number )
    \param message : error message (as "my error")

    <b>Usage:</b>
    \code
        int a = 1;
        MULTI_ARRAY_CHECK_OP(a, ==, 1, "this should never fail")
        MULTI_ARRAY_CHECK_OP(a, >=, 2, "this should fail")
    \endcode
*/
#define MULTI_ARRAY_CHECK_OP(a,op,b,message) \
    if(!  static_cast<bool>( a op b )   ) { \
       std::stringstream s; \
       s << "multi_array: Error: "<< message <<"\n";\
       s << "multi_array: check :  " << #a <<#op <<#b<< "  failed:\n"; \
       s << #a " = "<<a<<"\n"; \
       s << #b " = "<<b<<"\n"; \
       s << "in file " << __FILE__ << ", line " << __LINE__ << "\n"; \
       throw std::runtime_error(s.str()); \
    }

/** \def MULTI_ARRAY_CHECK(expresscd bl ion,message)
    \brief macro for runtime checks
    
    \warning The check is done 
        <B> even in Release mode </B> 
        (therefore if NDEBUG <B>is</B> defined)

    \param expression : expression which can evaluate to bool
    \param message : error message (as "my error")

    <b>Usage:</b>
    \code
        int a = 1;
        MULTI_ARRAY_CHECK_OP(a==1, "this should never fail")
        MULTI_ARRAY_CHECK_OP(a>=2, "this should fail")
    \endcode
*/
#define MULTI_ARRAY_CHECK(expression, message) if(!(expression)) { \
   std::stringstream s; \
   s << message <<"\n";\
   s << "multi_array: assertion " << #expression \
   << " failed in file " << __FILE__ \
   << ", line " << __LINE__ << std::endl; \
   throw std::runtime_error(s.str()); \
 }





#define MULTI_ARRAY_TEST(expression) MULTI_ARRAY_CHECK(expression,"")

#define MULTI_ARRAY_TEST_OP(a,op,b) MULTI_ARRAY_CHECK_OP(a,op,b,"")

#define MULTI_ARRAY_CHECK_EQ_TOL(a,b,tol) \
    if( std::abs(a-b) > tol){ \
        std::stringstream s; \
        s<<"multi_array: assertion "; \
        s<<"\""; \
        s<<" | "<< #a <<" - "<<#b<<"| < " #tol <<"\" "; \
        s<<"  failed with:\n"; \
        s<<#a<<" = "<<a<<"\n";\
        s<<#b<<" = "<<b<<"\n";\
        s<<#tol<<" = "<<tol<<"\n";\
        throw std::runtime_error(s.str()); \
    }

#define MULTI_ARRAY_TEST_EQ_TOL(a,b,tol) MULTI_ARRAY_CHECK_EQ_TOL(a,b,tol)

#define MULTI_ARRAY_CHECK_NUMBER(number) \
   { \
   std::stringstream s; \
   s << "multi_array: assertion failed in file " << __FILE__ \
   << ", line " << __LINE__ << std::endl; \
    if(std::isnan(number))\
        throw std::runtime_error(s.str()+" number is nan"); \
    if(std::isinf(number))\
        throw std::runtime_error(s.str()+"number is inf");\
    }

#ifdef NDEBUG
    #ifdef MULTI_ARRAY_DEBUG 
        #define MULTI_ARRAY_DO_DEBUG
    #endif
#else
    #ifdef MULTI_ARRAY_DEBUG 
        #define MULTI_ARRAY_DO_DEBUG
    #endif
#endif


/** \def MULTI_ARRAY_ASSERT_OP(a,op,b,message)
    \brief macro for runtime checks
    
    \warning The check is <B>only</B> done in
        in Debug mode (therefore if NDEBUG is <B>not</B> defined)

    \param a : first argument (like a number )
    \param op : operator (== )
    \param b : second argument (like a number )
    \param message : error message (as "my error")

    <b>Usage:</b>
    \code
        int a = 1;
        MULTI_ARRAY_ASSERT_OP(a, ==, 1) // will not fail here
        MULTI_ARRAY_ASSERT_OP(a, >=, 2) // will fail here
    \endcode
*/
#ifdef NDEBUG
   #ifndef MULTI_ARRAY_DEBUG 
      #define MULTI_ARRAY_ASSERT_OP(a,op,b) { }
   #else
      #define MULTI_ARRAY_ASSERT_OP(a,op,b) \
      if(!  static_cast<bool>( a op b )   ) { \
         std::stringstream s; \
         s << "multi_array: assertion :  " << #a <<#op <<#b<< "  failed:\n"; \
         s << #a " = "<<a<<"\n"; \
         s << #b " = "<<b<<"\n"; \
         s << "in file " << __FILE__ << ", line " << __LINE__ << "\n"; \
         throw std::runtime_error(s.str()); \
      }
   #endif
#else
   #define MULTI_ARRAY_ASSERT_OP(a,op,b) \
   if(!  static_cast<bool>( a op b )   ) { \
      std::stringstream s; \
      s << "multi_array: assertion :  " << #a <<#op <<#b<< "  failed:\n"; \
      s << #a " = "<<a<<"\n"; \
      s << #b " = "<<b<<"\n"; \
      s << "in file " << __FILE__ << ", line " << __LINE__ << "\n"; \
      throw std::runtime_error(s.str()); \
   }
#endif

/** \def MULTI_ARRAY_ASSERT(expression,message)
    \brief macro for runtime checks
    
    \warning The check is <B>only</B> done in
        in Debug mode (therefore if NDEBUG is <B>not</B> defined)

    \param expression : expression which can evaluate to bool

    <b>Usage:</b>
    \code
        int a = 1;
        MULTI_ARRAY_ASSERT(a == 1) // will not fail here 
        MULTI_ARRAY_ASSERT(a >= 2) // will fail here
    \endcode
*/
#ifdef NDEBUG
   #ifndef MULTI_ARRAY_DEBUG
      #define MULTI_ARRAY_ASSERT(expression) {}
   #else
      #define MULTI_ARRAY_ASSERT(expression) if(!(expression)) { \
         std::stringstream s; \
         s << "multi_array: assertion " << #expression \
         << " failed in file " << __FILE__ \
         << ", line " << __LINE__ << std::endl; \
         throw std::runtime_error(s.str()); \
      }
   #endif
#else
      #define MULTI_ARRAY_ASSERT(expression) if(!(expression)) { \
         std::stringstream s; \
         s << "multi_array: assertion " << #expression \
         << " failed in file " << __FILE__ \
         << ", line " << __LINE__ << std::endl; \
         throw std::runtime_error(s.str()); \
      }
#endif





