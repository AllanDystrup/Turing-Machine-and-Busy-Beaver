//+1==========================================================================//
//  MODULE                      BEAVER.CPP                                    //
//============================================================================//
//  FUNCTION    Implement CLASS BEAVER for simulation of a "Busy Beaver" TM
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Module BEAVER.HPP for definition of the class Beaver 
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//-1==========================================================================//

#include <iostream>			// DOS: <iostream.h>  
#include <string.h> 
#include <stdio.h>
#include <assert.h>

#include "Beaver.hpp" 
 

 
//+2 MODULE : BEAVER.CPP -------CLASS Beaver----------------------------------//
//   FUNCTION public            Beaver                          constructor   //
//-- SYNOPSIS ----------------------------------------------------------------//
                                        // SideEff: Instantiated Turing object
Beaver::Beaver                          // Function
(   unsigned    _uID,                   // Param 1: [in] BTM Unique identifier#
    char        *_rcProgram )           //       2: [in] BTM Program (trans seq.)
                                        //--------------------------------------
    : Turing                            // Inherit  Call base class constructor
    (  _uID,                            // Param 1: [in] TM Unique identifier# 
       (unsigned)strlen(_rcProgram)/10, //       2: [in] TM #Program States
       2u,                              //       3: [in] TM #TapeSymbols = 2
       8192u,                           //       4: [in] TM #Tape bytes  = 8K
       _rcProgram,                      //       5: [in] TM Program 
       (char *) 0,                      //       6: [in] TM Tape content = none
       '0' )                            //       7: [in] TM Tape filler  = zero
{                                       //--------------------------------------

//  DESCRIPTION
//  Create Beaver TM, ie. Turing Machine with :
//      Program: <_rcProgram>
//      Tape...: 4KB byte, using 2 symbols [0|1], initially all 0
//-2
    
    D(DLev1, std::cout<< "Creating" << "\n");    // Dump new state of BTM on stdout
    D(DLev1, std::cout<< *this << "\n" << std::flush);

} // END function Beaver::Beaver



//+2 MODULE : BEAVER.CPP -------CLASS Beaver----------------------------------//
//   FUNCTION public            vPReset                                       //
//-- SYNOPSIS ----------------------------------------------------------------//
void                                    // Return - Nothing
Beaver::vPReset                         // Function Reset this BTM
(   char    *_rcProgram )               // Param 1: [in] New program for BTM
{                                       //--------------------------------------
//  DESCRIPTION
//  Reset Beaver TM as follows :
//      Program: init.   to <rcProgram>, Pgm.Pointer reset to 1.st transition
//      Tape...: cleared to all zero,    TapePointer reset to middle-of-tape
//      History: cleared to all '~',     HistPointer reset to 1.st position
//-2
    unsigned    U;


    // The program
    uPStates = strlen(_rcProgram) / 10; // Calc. #states for BTM, cf. input pgm.  
    assert(bRANGE(uPStates, 1u, 9u));   // Check valid size of BTM : [1...9]
    U = uPStates * uTSymbols * uPCells; // Calc. length of TM program (#byte)
    assert(U == strlen(_rcProgram));    // Check correct program length!
    memcpy(rcProgram, _rcProgram, U);   // Initialize storage, cf. param1
    rcProgram[U] = '\0';                // Terminate storage (dump as string)
    pcProgram = rcProgram;              // Set "program pointer" on 1.transition

    // The tape
    memset(rcTape, '0', uTCells);       // Clear storage
    pcTape = rcTape + (uTCells / 2);    // Set "tape pointer" on middle of Tape 
    
    // The history
    ulTrans = 0ul;                      // Clear transition counter
    memset(rcTrans, '@', uHCells);      // Clear storage (to all '~')
    pcTrans = rcTrans;                  // Set "history pointer" on 1.position
        
    // Trace
    D(DLev4, std::cout<< "Reseting" << "\n");    // Dump new state of BTM on stdout
    D(DLev4, std::cout<< *this << "\n" << std::flush);
    
} // END function Beaver::vPReset


    

//+2 MODULE : BEAVER.CPP -------CLASS Beaver----------------------------------//
//   FUNCTION public            vPRun                                         //
//-- SYNOPSIS ----------------------------------------------------------------//
void                                    // Return - Nothing  
Beaver::vPRun                           // Function Run this BTM
(   unsigned        _uStep,             // Param 1: [in]  Bool T if Step Trace 
    unsigned long   _ulTrans,           //       2: [in]  Max Transitions to run
    char            *_rcResult )        //       3: [out] String for result 
{                                       //--------------------------------------
//  DESCRIPTION
//  Run Beaver TM
//     1: Repeat transitions by calling TM::ePTrans() until termination or failure;
//     2: Report result of the run (simulation) in return string <rcResult>
//-2
    char                cHitKey;
    enum Turing::ePExit ePE_Result;
    char                *pc;
    char                rcTrace[1025];   // >= uHCells!
    unsigned long       UL = 0;
    unsigned            U, V;


    // RUN the BTM until termination or failure (if program defined)
    if (*rcProgram != '\0')
        while ((ePE_Result = ePTrans()) == ePE_PTrans && ++UL <= _ulTrans) {
            if (_uStep) {
                std::cout << *this << "\n";  
                std::cin  >> &cHitKey;
            }
    }

    // FORMAT output string (including reversed transition trace) 
    for (   U = 0, V = uHCells-2, pc = pcTrans;
            U < V  &&  *(pc-1) != '@';
            U += 2,     pc = (pc == rcTrans+2 ? rcTrans + uHCells-2 : pc-2)) {
        rcTrace[U]   = *(pc-2);
        rcTrace[U+1] = *(pc-1);
        D(DLev4, rcTrace[U+2] = '\0');
        D(DLev4, std::cout << U << " " << rcTrace << std::endl);
    }
    rcTrace[U] = '\0';        
    (void) sprintf(_rcResult, "%d\t %u\t %lu\t %c%s\t %c%s\n",
                              ePE_Result, uTCount('1'), ulTrans,
                              '~', rcProgram,
                              '~', rcTrace);

} // END function Beaver::vPRun 



// END MODULE BEAVER.CPP                                                      //
//============================================================================//
