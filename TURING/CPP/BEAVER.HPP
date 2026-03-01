//+1==========================================================================//
//  MODULE                      BEAVER.HPP                                    //
//============================================================================//
//  FUNCTION    Def.CLASS BEAVER for simulation of Beaver Turing Machines (BTM);
//
//              The purpose of the simulations is to find n-state "Busy" BTM's; 
//              An n-state "Beasy Beaver" Turing Machine: BBTM(n) is defined by:
//              - n+1 Program states: [(0)|1|...|n], where 0=HALT
//              - 2   Tape symbols..: [0|1]
//              - 2+1 Moves.........: [(H)|L|R], where L=LEFT, R=RIGHT, H=HALT
//              - takes as input an unlimited Tape filled with 0's
//              - writes at least as many 1's on the Tape as any other n-BTM
//              - eventually halts !
//
//              Below are listed BBTM(n) for n=1-4 plus a conjectured BBTM(5) :
//                  +---------------+------------------------+
//                  |    Input      |           Output       |
//              +---+-------+-------|-------+--------+-------+-----+---------+
//              | n | State | Symbol| State | Symbol | Move  |#1's | #BTM(n) |
//              +---+---------------+------------------------+-----+---------+
//                  +---------------+------------------------+
//               1  |   1       0   |  (0)      1       H    | 001         
//                  |___1_______1___|__(0)______1_______H____|            64
//
//                * +---------------+------------------------+
//               2  |   1       0   |   2       1       L    | 004       
//                  |___1_______1___|___2_______1_______R____|         20.736
//                  |   2       0   |   1       1       R    |
//                  |___2_______1___|___0_______1_______H____|
//
//                  +---------------+------------------------+
//               3  |   1       0   |   2       1       R    | 006      
//                  |___1_______1___|___3_______1_______L____|     17.777.216
//                  |   2       0   |   1       1       L    |
//                  |___2_______1___|___2_______1_______R____|
//                  |   3       0   |   2       1       L    |
//                  |___3_______1___|__(0)______1______(H)___|
//
//                  +---------------+------------------------+
//               4  |   1       0   |                          013     
//                  |___1_______1___|                            25.600.000.000
//                  |   2       0   |           
//                  |___2_______1___|                         
//                  |   3       0   |             
//                  |___3_______1___|                         
//                  |   4       0   |             
//                  |___4_______1___|________________________|
//                          
//                  +---------------+------------------------+ 
//               5? |   1       0   |   2       1       R    | 501 
//                  |___1_______1___|___3_______0_______L____| 63.403.380.965.374
//                  |   2       0   |   3       1       R    |
//                  |___2_______1___|___4_______1_______R____|
//                  |   3       0   |   1       1       L    |
//                  |___3_______1___|___2_______0_______R____|
//                  |   4       0   |   5       0       R    |
//                  |___4_______1___|__(0)______1______(H)___|
//                  |   5       0   |   3       1       L    |
//                  |___5_______1___|___1_______1_______R____|
//
//               *) Located by this programs random probing w. genetic mutation 
//
//              There are two obvious ways of attacking the BBTM(n) problem :
//              [1] Find an analytical method of identifying BBTM(n) 
//              [2] Conduct an exhaustive search of all BTM(n)'s
//
//              Both these approaches however have major flaws :
//              [1] Let BBTM(n) = max #1's producible by a BTM(n), Then :
//                  BBTM(n)= 1,4,6,13,501? for n=[1,2,3,4,5] as listed above.
//                  BBTM(n) is however NONCOMPUTABLE, since it is undecidable
//                  how long to wait for a BTM(n) to halt ("Halting Problem").
//              [2] Let n= #Program-states, a= #Tape-symbols(2), m= #Moves(2); 
//                  We can then calculate the #different n-BTM as follows :
//                      #transitions in BTM Program/Graph : T = (n*a) = 2n
//                      #unique graphs....: (n+1) ** T = (n+1) ** (n*2)
//                      each decorated by.: (a**T) * (m**T) Output/Move comb.
//                                          = 2**T * 2**T = 2**2T = 2 ** 4n 
//                  which gives the total # different n-state BTM's. :
//                      ((n+1)**2n) * 2**4n   =   ((n+1) * 4) ** 2n 
//                  #BTM(n) for n=[1,2,3,4,5] are listed in the diagram above.
//                  The combinatorial complexity of BTM(n) graphs increases 
//                  exponentially as a function of n, which makes an exhaustive
//                  search INFEASABLE. (and furthermore the Halting Problem
//                  also crops up in this context)
//
//              So by both theoretical (graph analysis) & practical (simulation)
//              methods the BBTM(n) problem is in general UNSOLVABLE. Why then
//              waste your time on the problen? The BBTM(n) problem is a simple
//              canonical representation of a basic problem in Computer Science,
//              namely the computational undecidability of the complexity class
//              NP (NonPolynomial systems). Many problems in NP are of great 
//              theoretical and practical interest, ranging from simple areas
//              as graph analysis (TSP) to artificial intelligence.
//
//              I believe that one way to deal with NP-problems is by imposing 
//              a conceptional hierachy on the problem solving method by using :
//              A: a basic "dumb" algorithm to crank away on problem instances
//              B: higher level "oracles" for :
//                 - deciding the feasability of specific problem instances
//                   based on heuristics of program structure, ...
//                 - evaluate the simulation with respect to anomalies such 
//                   as nontermination, resource exhaustion ...
//              C: a top level control logic to :
//                 - "spawn" instances of A, using program mutation (in the
//                   sence of genetic programming) related to some specific
//                   fitness criteria, - eg. #1's produced by BTM(n)
//                 - for each A, call on B to supervise the problem solving.
//
//              The BBTM(n) problem seems ideal to try out the validity of this
//              concept.
//              
//
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Module TURING.CPP for implementation of the class turing methods
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       "
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//-1==========================================================================//
#if !defined BEAVER_HPP
#define BEAVER_HPP

#include "Turing.hpp"



//+2 MODULE BEAVER.HPP =======================================================//
//   CLASS                          Beaver                                    //
//== SYNOPSIS ================================================================//
//   This class defines a specialized TM for simulation of "Busy Beaver"   
//-2
class Beaver : public Turing {

public :
    // Class constructor & destructor
            Beaver  ( unsigned uID, char *rcProgram);
            ~Beaver () {};
    
    // Reset & run Beaver TM Program
    void    vPReset ( char *rcProgram);  
    void    vPRun   ( unsigned uStep, unsigned long ulTrans, char *rcResult);

}; // END CLASS Beaver



#endif // BEAVER_HPP
// END MODULE BEAVER.HPP                                                      //
//============================================================================//
