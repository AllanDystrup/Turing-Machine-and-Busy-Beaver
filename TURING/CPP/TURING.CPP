//+1==========================================================================//
//  MODULE                      TURING.CPP                                    //
//============================================================================//
//
//  FUNCTION    Implement CLASS TURING for simul. of a Universal Turing Machine
//
//  SYSTEM      IBM PC >= i386 cpu, MS DOS, Later ported to MS Windows terminal.
//
//  SEE ALSO    Module TURING.HPP for definition of the class 'turing'.
//
//  PROGRAMMER  Allan Dystrup (AD).
//
//  VERSION     0.1		Alpha/pre-release 11-06-1995, using C++ 2.0 (1989)
//						On PC/MS DOS using Microsoft Visual C++ (cl build)
//				0.2 	28-02-2026 port using C++ 23 (ISO/IEC 14882:2024)
//						On Windows 10 terminal with WSL/UBUNTU g++ build 
//
// REFERENCES  
// 	    		Turing, A.M. (1937). 
//					"On Computable Numbers, with an Application to the 
//					Entscheidungsproblem". Proceedings of the London 
//					Mathematical Society. 2. 42 (1): 230–65.
//
//				Rado, T: 
//					"On Non-Computable Functions",
//					 The Bell System Technical Journal,
//					(vOL.: 41, Issue: 3, pp.877 - 884, May 1962).
//
//  USAGE 		Console Test drivers (build cf Turing.Makefile):
//					Turing Machine testdriver:	./bin/TMTEST
//					Busy Beaver Application:	./bin/BBTEST
//      
//  DOC			Incorporated in the code comments. 
//
//	BUGS:
//		Design:	The main design principle has been comprehensibility and fast 
//             	execution, - NOT maximum reusability (eg. Class Turing could be
//             	decomposed into Class TMProgram, Class TMTape etc.
//  	Code:	This is an alpha/pre-release, and as such it has not been 
//				developed for robustness or strict code standard compliance.
//
//  COPYRIGHT   none!
//
//-1==========================================================================//
 

//----------------------------------------------------------------------------//
//                              INCLUDES                                      //
//----------------------------------------------------------------------------//
#include <iostream>		// <iostream.h> port from C++ v 2 (1995) to v 23 (2026)
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
     
#include "turing.hpp"



//+2 MODULE : TURING.CPP -----------------------------------------------------//
//   FUNCTION public            Turing                          constructor   //
//-- SYNOPSIS ----------------------------------------------------------------//
                                        // SideEff: Instantiated Turing object
Turing::Turing                          // Function Class Turing constructor
(   unsigned    _uID,                   // Param 1: [in] TM Unique identifier#
    unsigned    _uPStates,              //       2: [in] TM #Program States
    unsigned    _uTSymbols,             //       3: [in] TM #Tape Symbols
    unsigned    _uTCells,               //       4: [in] TM #Tape bytes
    char        *_rcProgram,            //       5: [in] TM Program (trans seq.)
    char        *_rcTape    =0,         //       6: [in] TM Tape content (input)
    char        _cSet       =0)         //       7: [in] TM Tape initial filler
{                                       //--------------------------------------
//  DESCRIPTION
//  Constructor for object of class Turing :
//     1: INITIALIZE :  TM primitives, program, tape and history
//    [2: DUMP.......:  TM state on stdout (if defined: DEBUG)]
//-2
    unsigned    U = 0;                      // Scratch variable

       
    // The primitives
    uID       = _uID;                   // Unique ID for this TM object
    uPStates  = _uPStates;              // 
    uTSymbols = _uTSymbols;             //
    uTCells   = _uTCells;               //
    uPCells   = 5;                      // Each transition definition is 5 byte 
    uHCells   = 1024;                   //  1K Cyclic buffer tracing state trans.
    ulPLoop   = 10000000L;            	//  ~10M Transitions considered LoopOmega!


    // The program  
    U = uPStates * uTSymbols * uPCells; // Calc. length of TM program (#byte) 
    assert(U == strlen(_rcProgram));    // Check correct program length!    
    rcProgram = new char[U+1];          // Allocate storage for the TM Program
    assert(rcProgram != 0);             // 
    memcpy(rcProgram, _rcProgram, U);   // Initialize pgm. storage, cf. param5
    rcProgram[U] = '\0';                // Terminate storage (for dump as string)
    pcProgram = rcProgram;              // Set "program pointer" on 1.transition

    
    // The tape
    rcTape = new char[_uTCells+1];      // Allocate storage for the TM Tape
    assert(rcTape != 0);                //
    memset(rcTape, _cSet, _uTCells);    // Clear storage, cf. param7
    rcTape[_uTCells] = '\0';            // Terminate storage (for dump as string)
    pcTape = rcTape + (_uTCells / 2);   // Set "tape pointer" on middle of Tape 
    if (_rcTape != 0 && (U = strlen(_rcTape)) > 0)
          memcpy(pcTape, _rcTape, U);     // Initialize Tape input, cf. param6
    
    
    // The history
    ulTrans = 0L;                       // Clear transition counter
    rcTrans = new char[uHCells+1];      // Allocate storage for the TM history
    assert(rcTrans != 0);               //
    memset(rcTrans, '@', uHCells);      // Clear storage (to all '*')
    rcTrans[uHCells] = '\0';            // Terminate storage (for dump as string)
    pcTrans = rcTrans;                  // Set "history pointer" on 1.position
    
        
    // Trace
    D(DLev1,std::cout << "Constructing" << "\n");// Dump initial state of TM on stdout
    D(DLev1,std::cout << *this << "\n" << std::flush);

} // END function Turing::Turing



//+2 MODULE : TURING.CPP -----------------------------------------------------//
//   FUNCTION public            ~Turing                          destructor   //
//-- SYNOPSIS ----------------------------------------------------------------//
                                        // SideEff: Terminate Turing object
Turing::~Turing                         // Function Class Turing destructor
()                                      // Param  - None
{                                       //--------------------------------------
//   DESCRIPTION
//   Destructor for object of class Turing :
//     1: TERMINATE..:  TM, releasing resources for program, tape and history
//    [2: DUMP.......:  TM state on stdout (if defined: DEBUG)]
//-2
    

    // Trace
    D(DLev1,std::cout << "Destructing" << "\n");
    D(DLev1,std::cout << *this << std::flush);
    // Free dynalloc'ed resources (new'ed in constructor)

    delete [] rcProgram;
    delete [] rcTape;
    delete [] rcTrans;
    
} // END function Turing::~Turing



//+2 MODULE : TURING.CPP -----------------------------------------------------//
//   FUNCTION public            ePTrans                                       //
//-- SYNOPSIS ----------------------------------------------------------------//
enum Turing::ePExit                     // Return   Exit-state of transition
Turing::ePTrans                         // Function Perform TM state transition
()                                      // Param  - None
{                                       //------------------------------------//
//   DESCRIPTION
//   Perform ONE transition : (CurrentState,Input) -> (NewState, Output, Move)
//-2
    char        cPSta = *pcProgram;     // Get: Current TM Program state 
    char        cTSym = *pcTape;        // Get: Current TM Tape input symbol
    unsigned    U;                      // 
    const char  *pcEnd = rcProgram + uPStates * uTSymbols * uPCells;
     
    
    // 1: FIND TRANSITION from current PgmState:cPSta on current TapeSymbol:cTSym 
    while ( (cPSta == *pcProgram) && (cTSym != *(pcProgram+1)) && pcProgram < pcEnd ) {
            pcProgram = pcProgram + uPCells;
    }
    assert(cPSta == *pcProgram && cTSym == *(pcProgram+1)); 
    
    // 2: UPDATE HISTORY with (cPSta,cTSym), cf. current trans:pcProgram->
    ulTrans++;                                  // Check for loop omega    
    if (ulTrans > ulPLoop) return ePE_PLoop;    
    *pcTrans++ = *pcProgram;                    // Record Trans. Pgm.State
    *pcTrans++ = *(pcProgram+1);                //    "   Trans. TapeSymbol
    pcTrans = (pcTrans >= rcTrans + uHCells - 2 ? rcTrans : pcTrans); // Wrap


    // 3.1: ADVANCE TAPE, cf transition pcTrans-> (OutSym, Move)
    *pcTape = *(pcProgram + 3);                 // Write output symbol to Tape
    switch (*(pcProgram + 4)) {                 // Move Tape R/W position
        case '2' :  // RIGHT
                    pcTape += 1;                
                    if (pcTape - rcTape >= (int) uTCells) return ePE_TOflow;
                    break;
        case '1' :  // LEFT
                    pcTape -= 1;
                    if (pcTape - rcTape <= 0) return ePE_TUflow; 
                    break;
        default  :  assert(eBFalse);
                    break;
    }


    // 3.2: HALT or ADVANCE PROGRAM, cf transition pcTrans->(OutSta)
    if ( *(pcProgram + 2) == '0')
        return ePE_PHalt;
    else {
        U = uC2U(*(pcProgram+2));        
        pcProgram = rcProgram + ((U-1) * uTSymbols * uPCells); 
        assert((pcProgram - rcProgram) >= 0 && U <= uPStates);
        return ePE_PTrans;
    }        

} // END function Turing::iPTrans



//+2 MODULE : TURING.CPP -----------------------------------------------------//
//   FUNCTION public            uTCount                                       //
//-- SYNOPSIS ----------------------------------------------------------------//
unsigned                                // Return
Turing::uTCount                         // Function
(   char    cGet)                       // Param 1: [in]
{
//  DESCRIPTION
//
//-2
    unsigned    U       = 0;
    const char  *pcEnd  = rcTape + uTCells;
    
    for (char *pc = rcTape; pc < pcEnd; pc++)
        { if (*pc == cGet)  U++; }
    
    return U;
    
} // END function Turing::iTCount   




//+2 MODULE : TURING.CPP -----------------------------------------------------//
//   FUNCTION public            operator <<                 operator overload //
//-- SYNOPSIS ----------------------------------------------------------------//
std::ostream&                           // Return
operator<<                              // Function
(   std::ostream&   os,            		// Param 1:
    const Turing&   TM)                 //       2:
{                                       //------------------------------------//
//  DESCRIPTION
//
//-2
    unsigned        uI, uJ, uK;
    char            *pcP, *pcT, *pcH, *pcX;
    const unsigned  uLineLen = 80;
     char            rcLineBuf[uLineLen+1];

    
    // The TM ID
    os << "TM ID="   << TM.uID      << "\n";


    // The program
    os  << "\nPROGRAM... (InState, InSymbol, OutState, OutSymbol, Move)\n"; 
    os << "   #States.: " << TM.uPStates  << ",   [(0)|1|...|" << TM.uPStates << "]\n"; 
    os << "   #Symbols: " << TM.uTSymbols << ",   [0|1]\n";
     uJ = (TM.uPStates * TM.uTSymbols);                                 // Total   #Trans
     uK = (unsigned)(TM.pcProgram - TM.rcProgram) / TM.uPCells ;   // Current Trans#
     rcLineBuf[TM.uPCells] = '\0';
     for (uI = 0, pcP = TM.rcProgram; uI < uJ; uI++, pcP += TM.uPCells) {
          os << ((uI == uK) ? "pc>" : "   ");
        strncpy(rcLineBuf, pcP, TM.uPCells);
        os << rcLineBuf << "\n";
    }       


    // The tape
    os << "\nTAPE...:\n"; 
    os << "   #Cells:" << TM.uTCells << std::endl;
    rcLineBuf[uLineLen] = '\0';
    for (pcT = TM.rcTape; pcT < (TM.rcTape + TM.uTCells) ; pcT += uLineLen) { 
        if (pcT <= TM.pcTape && TM.pcTape <= pcT + uLineLen) {
            os <<  "pcT";
            for (pcX = pcT; pcX <= TM.pcTape; pcX++) { os << ">"; }
            os << (TM.pcTape - (TM.rcTape + TM.uTCells / 2)) << std::endl;
            
            D(DLev2, strncpy(rcLineBuf, pcT, uLineLen));
            D(DLev2, os << "   " << rcLineBuf << "\n");
        }
        D(DLev2, strncpy(rcLineBuf, pcT, uLineLen));
        D(DLev2, os << "   " << rcLineBuf << "\n");
    }       


    // The transitions
    assert (TM.uHCells > uLineLen);
    os << "\nTRANS...:\n";
    os << "   #Moves:" << TM.ulTrans << std::endl;
    rcLineBuf[uLineLen+1] = '\0';
    for (pcH = TM.rcTrans; pcH < (TM.rcTrans + TM.uHCells) ; pcH += uLineLen) { 
        if (pcH <= TM.pcTrans && TM.pcTrans <= pcH + uLineLen) {
            os <<  "pcH";
            for (pcX = pcH; pcX <= TM.pcTrans; pcX++) { os << ">"; }
                os << (TM.pcTrans - TM.rcTrans) << std::endl;
        }
          strncpy(rcLineBuf, pcH, uLineLen);
        os << "   " << rcLineBuf << std::endl;
    }       
    os << "\n\n" << std::flush;
    
        
    return os;
    
} // END function Turing::operator<<
    


// END MODULE TURING.CPP                                                      //    
//============================================================================//
    
    
    
    
    
    
    
    
    
    
    
     