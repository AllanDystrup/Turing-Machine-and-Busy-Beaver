//+1==========================================================================//
//  MODULE                      GENERAL.CPP                                   //
//============================================================================//
//  FUNCTION    Implementation of general functions 
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

#include <iostream>			// <iostream.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "general.hpp"



//+3 MODULE : GENERAL.CPP ----------------------------------------------------//
//   DATA                       DLevel                             Debugging  //
//----------------------------------------------------------------------------//
// Set DEBUG level
const static unsigned  DLAll   = DLev1 | DLev2 | DLev3 | DLev4;
const static unsigned  DLNrm   = DLev1 | DLev2 | DLev3;
unsigned               DLevel  = DLNrm;




//+3 MODULE : GENERAL.CPP ----------------------------------------------------//
//   FUNCTION public            uC2U                               Conversion //
//-- SYNOPSIS ----------------------------------------------------------------//
unsigned uC2U
(   char C )
{
    char    S[2];

    S[0] = C; S[1] = '\0';
    return (unsigned) atoi(S);
}


// END MODULE GENERAL.CPP                                                     //
//============================================================================//
