//+1==========================================================================//
//  MODULE                      TURING.HPP                                    //
//============================================================================//
//  FUNCTION    Define CLASS TURING for simulation of a Universal Turing Machine
//
//              The structure of the UTM is defined as follows (by example): 
//              ----------------------------------------------------------------
//              iPStates  = 5   :   Turing machine states .... [(0)|1|2|3|4|5]
//              iTSymbols = 2   :   Tape I/O symbol alfabet... [0|1]       
//              iPCells = 5byte:    Turing machine transition definition :
//                                  [InState,InSymbol,OutState,OutSymbol,Move]
//
//              rcProgram           Sequence of TM transition definitions
//                                  describing the current TM program
//              pcProgram           Pointer to current TM transition def.
//                                  +----------------------------------------+
//                                  |Program ~ Seq. of Transition Definitions|
//                                  +----------------------------------------+
//                                  |    Input      |           Output       |
//                                  |State  Symbol  |   State   Symbol  Move*|
//                                  +---------------+------------------------+
//              rcProgram----->pos 0|   1       0   |   2       1       2    |
//                                  |___1_______1___|___3_______0_______1____|
//                                  |   2       0   |   3       1       2    |
//                                  |___2_______1___|___4_______1_______2____|
//                                  |   3       0   |   1       1       1    |
//              pcProgram---------->|___3_______1___|___2_______0_______2____|
//                                  |   4       0   |   5       0       2    |
//                                  |___4_______1___|__(0)______1______(0)___|
//                                  |   5       0   |   3       1       1    |
//                                  |   5       1   |   1       1       2    |
//                                  +---------------+------------------------+
//                                                                      ^
//                                                                      | *) 0=Halt
//                                                                      |    1=Left
//                                                                      |    2=Right
//                                                                      |
//                                  rcProgram + (iPStates * iTSymbols * iPCells)-1
//
//
//              rcTape              Sequence of cells with I/O symbols
//                                  describing the current TM tape
//              pcTape              Pointer to the current TM tape position
//                          +-+-+-+--/\/------------+-+----------------/\/--+-+
//              rcTape----->|0|1|1|  ...            |0|     ...             |1|
//                          +-+-+-+--/\/------------+-+----------------/\/--+-+
//                           ^                       ^                       ^
//                           |                       |                       |
//                      pos -iTCells/2            pcTape                 +iTCells/2
//              ----------------------------------------------------------------
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Module TURING.CPP for implementation of the class turing methods
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       "
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//  ----------------------------------------------------------------------------
//  DESIGN      The main design principle has been comprehensibility and fast 
//              execution, - NOT maximum reusability (eg. Class Turing could be
//              decomposed into Class TMProgram, Class TMTape etc.)
//
//-1==========================================================================//
#if !defined TURING_HPP
#define TURING_HPP

#include "General.hpp"



//+2 MODULE TURING.HPP =======================================================//
//   CLASS                          Turing                                    //
//== SYNOPSIS ================================================================//
//   This class defines a generic Universal Turing Machine (UTM) for simulation
//-2
class Turing {

#ifndef DEBUG
protected :
#else
public :
#endif
    // The primitives 
    unsigned uID;           // Unique identification of this TM
    unsigned uPStates;      // Number of machine states
    unsigned uTSymbols;     // Number of tape symbols
    unsigned uPCells;       // Number of bytes in one transition
    unsigned uTCells;       // Number of bytes on tape 
    unsigned uHCells;       // Number of bytes in Trans history
    unsigned long ulPLoop;  // Number of Trans for LoopOmega cond.
        
    // The program
    char    *rcProgram;     // Range of bytes defining state transition function
    char    *pcProgram;     // Pointer to current program state transition definition
    
    // The tape
    char    *rcTape;        // Range of bytes defining the tape
    char    *pcTape;        // Pointer to current tape position
        
    // The transitions (memory)
    unsigned long ulTrans;  // Number of state transitions 
    char    *rcTrans;       // Trace of state transitions (cyclic buffer)
    char    *pcTrans;       // Pointer to current trans. position


public :
    // TM Constructor and Destructor
             Turing (unsigned uID, 
                     unsigned uPStates,   unsigned uSybmols, unsigned uCells,
                     char     *rcProgram, char     *rcTape,  char     cSet);
    	    ~Turing();
    
    // TM Program transition 
    enum     ePExit { ePE_PTrans=0, ePE_PHalt, ePE_PLoop, ePE_TUflow, ePE_TOflow };
    enum     ePExit ePTrans();

    // TM State accessors (Tape content, TM dump)
    unsigned uTCount(char cGet);
    friend   std::ostream& operator<<(std::ostream&, const Turing&);
    
};
// END CLASS Turing



#endif // TURING_HPP
// END MODULE TURING.HPP                                                      //
//============================================================================//

                    
                    
    
    
