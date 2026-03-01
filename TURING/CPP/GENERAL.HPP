//+1==========================================================================//
//  MODULE                      GENERAL.HPP                                   //
//============================================================================//
//  FUNCTION    Declaration of general enum's & macro's and 
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO     
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       "
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//-1==========================================================================//
#if !defined GENERAL_HPP
#define GENERAL_HPP


#include <assert.h> 
#include <stdio.h>          // FILE
#include <memory.h>         // memset()
#include <string.h>         // strncpy(), strchr()



//----------------------------------------------------------------------------//
//                              ENVIRONMENT                                   //
//----------------------------------------------------------------------------//
// ---OPERATING SYSTEM
//#define OSDOS
#define OSWIN
// ---PROGRAM COMPILER
#define BORLAND			// also GNU C++
//#define MICROSOFT
#define GNU

#if defined(OSDOS)
#define DOS(x)  (x)
#else
#define DOS(x)  /*ignore*/
#endif



//----------------------------------------------------------------------------//
//                              CONVERSION                                    //
//----------------------------------------------------------------------------//
unsigned uC2U(char C);



//----------------------------------------------------------------------------//
//                              BOOLEAN                                       //
//----------------------------------------------------------------------------//
enum eBool  { eBFalse=0, eBTrue };



//----------------------------------------------------------------------------//
//                              RANGE                                         //
//----------------------------------------------------------------------------//
#if defined (MICROSOFT)     // MSC++ 1.5 no support for templates!
    #define bRANGE(a,b,c)   ( ((b) <= (a)) && ((a) <= (c)) )
#elif defined (BORLAND)     // BC++ 4.0 support for templates
    template <class Type>
    enum eBool bRANGE(Type a, Type b, Type c) {
        return ((b) <= (a) && (a) <= (c) ? eBTrue : eBFalse);
    }
#endif

#define uROUND(f)       (unsigned) (fmod(f,1) < 0.5 ? (f) : ((f)+1.0) )


//----------------------------------------------------------------------------//
//                              DEBUG                                         //
//----------------------------------------------------------------------------//
//#define DEBUG
//#undef DEBUG

const unsigned DLev0 = 0x0;
const unsigned DLev1 = 0x1;
const unsigned DLev2 = 0x2;
const unsigned DLev3 = 0x4;
const unsigned DLev4 = 0x8;

extern unsigned DLevel;      // Initialized in general.cpp


#ifdef   DEBUG
#define  D(Level,Action)    { if ((Level) & (DLevel)) (Action) ; }
#else
#define  D(Level,Action)    /*ignore*/
#endif




#endif // GENERAL_HPP
// END MODULE GENERAL.HPP                                                     //
//============================================================================//
