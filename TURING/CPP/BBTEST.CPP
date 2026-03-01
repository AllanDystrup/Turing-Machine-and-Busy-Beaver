//+1==========================================================================//
//  MODULE                      BBTEST.CPP                                    //
//============================================================================//
//  FUNCTION    Find BUSY BEAVERS (using Beaver TM based on Universal TM)
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Modules TURING.HPP/CPP for def./implementation of Class Turing
//              Modules BEAVER.HPP/CPP for def./implementation of Class Beaver
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       BBTEST
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//-1==========================================================================//



//----------------------------------------------------------------------------//
//                              INCLUDES                                      //
//----------------------------------------------------------------------------//
#include <iostream>				// <iostream.h>
#include <stdlib.h>             // exit()
#include <string.h>
#include <assert.h>
#include <ctype.h>              // BC4 toupper()

#include "Turing.hpp"           // Basic UTM: Universal Turing Machine
#include "Beaver.hpp"           // Beaver TM: Turing Machine for BusyBeaver Problem
#include "SList.hpp"            // Generic Sorted List ADT
#include "Random.hpp"           // Pseudo Random Number generator

#ifdef   OSDOS
#include <signal.h>             // DOS: Signal
static void         vSigCatch   (int iSigNum);
#else	//POSIX
#include <csignal>				// POSIX Signal
static void			vSigCatch	(int iSigNum );
#endif


//----------------------------------------------------------------------------//
//                              PROTOTYPES                                    //
//----------------------------------------------------------------------------//
enum    eBPType  { eBP_Ok=0, eBP_Split, eBP_Blind, eBP_Trap, eBP_Omega };

void             vBPCreate   (char *_rcProgram, unsigned _uPStates);
void             vBTMutate   (char *_pcP,       unsigned _uPStates);
void             vBBMutate   (char *_rcProgram, unsigned _uBBNum,  unsigned _uPStates, unsigned _uBMate);

enum    eBPType  eBPChkStruc (char *_rcProgram, char *_rcResult);
enum    eBPType  eBPChkTrans (char *_rcResult);

void             vBPRun      (Beaver &_BTM,     char *_rcProgram,  char *_rcResult, unsigned  _uStep);


//----------------------------------------------------------------------------//
//                              GLOBALS                                       //
//----------------------------------------------------------------------------//
                                    // History list for BB, - accessible by signal()
const unsigned cuLSlots = 20;       // #Slots in SList
const unsigned cuLElems = 1200;     // #Cells in one slot element
SList   BSL(cuLSlots, cuLElems);    // SortedList Object

const unsigned cuLPrgm = 101;       // Length of program string
CRandom RND;                        // PseudoRandom Object




//#ifdef OSDOS //===============================================================//
static void
vSigCatch
(   int iSigNum )
{
     char cAnswer = 'X';

     signal(iSigNum, SIG_IGN);		// ignore signal and continue 
     
     std::cerr << "\nINTERRUPT:\tSignal " << iSigNum << " received...\n";
     std::cerr << "Save&Continue | Continue | Exit&Save | Abort [S|C|X|A] =>";
     std::cin.get(cAnswer);

     switch (toupper(cAnswer)) {

          case 'S' :  std::cout << "OK, SAVE & CONTINUE\n";
                      BSL.vFlush();
                      std::cout << BSL << std::endl;
                      break;

          case 'X' :  std::cout << "OK, SAVE & EXIT\n";
                      BSL.vFlush();
                      std::cout << BSL << std::endl;
                      std::cout.flush();
                      exit(1);
                      break;

          case 'A' :  std::cout << "OK, ABORT WITHOUT SAVING\n";
                      std::cout.flush();
                      abort();
                      break;

          default  :
          case 'C' :  std::cout << "OK, CONTINUE WITHOUT SAVING\n";
                      break;
     }
     
     signal(iSigNum, vSigCatch);	// reset sig handler
}
//#endif//OSDOS ================================================================//





//+2 MODULE : BBTEST.CPP -----------------------------------------------------//
//   FUNCTION public            (v)main                          testdriver   //
//-- SYNOPSIS ----------------------------------------------------------------//
int                                        //
main                                        //
(   void )                                  //
{                                           //--------------------------------//
    char        rcProgram[cuLPrgm];         // For trans. definitions [0...99\0]
    char        rcResult[cuLElems+20];      // For result: cat(current, max)
    unsigned    U           =  0u;          //
    unsigned    V           =  0u;          //
    unsigned    uStep       =  0u;          //
    unsigned    uPStates    =  0u;          //
    char        rcPStates[] = "0123456789"; //
    char        rcLFile[]   = "BBEAVERx.TXT";


    // INITIALIZE ------------------------------------------------------------//
     // 1.0A: SETUP signal handler for MSDOS executable
    DOS(assert(signal(SIGINT,  vSigCatch) != SIG_ERR));
    DOS(assert(signal(SIGTERM, vSigCatch) != SIG_ERR));
    //  1.0B SETUP signal handler for POSIX executable (LINUX C++)
    std::signal(SIGINT, vSigCatch);
    
    RND.vRandomize();                       // Seed pseudo random number generator


    // 1.1: CREATE the "mother Beaver TM", with room for 10 states! (don't run it)
    memset(rcProgram, '~', 100);
    rcProgram[100] = '\0';              // [0---99\0]
    Beaver BTM(U, rcProgram);
    std::cout << "Please enter #States for BTM(n) ->";
    std::cin  >> uPStates;

    // 1.2: RESTORE stack of BB-candidates from file (cf. uPStates)
    assert(bRANGE(uPStates, 1u, 9u));
    rcLFile[7] = rcPStates[uPStates];
    BSL.vInit(rcLFile);

    
    // 2: RUN ----------------------------------------------------------------//
    // 2: LOOP : Reset, Execute & Report "child beavers", using the mother BTM
    
    // 2.1: Initially generate a population
    for (U = 0u; U < 1000u; U++) {
        vBPCreate(rcProgram, uPStates);
        vBPRun(BTM, rcProgram, rcResult, uStep);
    }
    
    // 2.2: Eternally evolve the population
    for (;;) {
        
        // 221: Loop through existing Beavers in BB-list: Mutate & sex ("Stir the pot")
        for (U = 0u; U < cuLSlots; U++) {
        
            // 0=FIX, 1=RND 2=SEX mutation
            for ( V = 0u; V < 3; V++) {
                vBBMutate(rcProgram, U, uPStates, V); 
                vBPRun(BTM, rcProgram, rcResult, uStep);
            }
        }

        // 222: Make room and mix new blood ("Add spice")
        BSL.vDelMin(5);                         // Eliminate loosers ("lebensraum")
        for (U = 0u; U < 1000u; U++) {          // Enter new generation
            vBPCreate(rcProgram, uPStates);
            vBPRun(BTM, rcProgram, rcResult, uStep);
        }             
     }

} // END function (v)main




#ifdef OSDOS //===============================================================//
static void
vSigCatch
(   int iSigNum )
{
     char cAnswer = 'X';

     signal(iSigNum, SIG_IGN);
     
     cerr << "\nINTERRUPT:\tSignal " << iSigNum << "received...\n";
     cerr << "Save&Continue | Continue | Exit&Save | Abort [S|C|X|A] =>";
     cin.get(cAnswer);

     switch (toupper(cAnswer)) {

          case 'S' :  std::cout << "OK, SAVE & CONTINUE\n";
                      BSL.vFlush();
                      std::cout << BSL << std::endl;
                      break;

          case 'X' :  cout << "OK, SAVE & EXIT\n";
                      BSL.vFlush();
                      std::out << BSL << std::endl;
                      std::cout.flush();
                      exit(1);
                      break;

          case 'A' :  std::cout << "OK, ABORT WITHOUT SAVING\n";
                      std::cout.flush();
                      abort();
                      break;

          default  :
          case 'C' :  std::std::cout << "OK, CONTINUE WITHOUT SAVING\n";
                      break;
     }
     
     signal(iSigNum, vSigCatch);
}
#endif//OSDOS ================================================================//




//+2 MODULE : BBTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vBPCreate                                     //
//-- SYNOPSIS ----------------------------------------------------------------//
void                                        // Return
vBPCreate                                   // Function
(   char        *_rcProgram,                // Param 1: [out]
    unsigned     _uPStates )                //       2: [in]
{                                           //--------------------------------//
//  DESCRIPTION
//  Create a (pseudo)random - but syntactically valid - BTM(_uPStates) program
//  Return the program in string <_rcProgram>
//-2
    char             cPState  = '0';    
    const unsigned   uPCells  = 5;                      // #Cells pr. Program Trans. 
    const unsigned   uPSCells = 2 * uPCells;            // #Cells pr. Program State
    const unsigned   uPTCells = _uPStates * uPSCells;   // #Cells pr. Program (total)
    char             *pcP, *pcPEnd;


    // CHECK Preconditions
     assert(bRANGE(_uPStates,1u,10u));


    // GENERATE the program
    for ( pcP=_rcProgram, pcPEnd=_rcProgram + uPTCells, cPState= '1';
          pcP < pcPEnd;
          pcP += uPSCells, cPState += 1 )   {
          
            // IN State & Symbols
            *pcP = *(pcP + uPCells) = cPState;      // Current State
            *(pcP + 1)          = '0';              // 1.st Symbol
            *(pcP + uPCells+1)  = '1';              // 2.nd Symbol
  
            // OUT Transition
            vBTMutate(pcP, _uPStates);
            vBTMutate(pcP + uPCells, _uPStates);
        }
    *pcP = '\0';
    
} // END function vBPCreate                         



//+2 MODULE : BBTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vBTMutate                                     //
//-- SYNOPSIS ----------------------------------------------------------------//
void                                            // Return
vBTMutate                                       // Function
(   char         *_pcT,                         // Param 1[in/out]: Pgm Trans.
     unsigned     _uPStates )                    //       2[in]....:
{                                               //----------------------------//
//  DESCRIPTION
//  Mutate ONE BTM transition : OUT-State, OUT-Symbol and Tape-MOVE
//-2
     // OUT  State, Symbol & Move
     *(_pcT + 2)  = RND.cRand((char*)"0123456789", 1+_uPStates);   // GoTo  State
     *(_pcT + 3)  = RND.cRand((char*)"01", 2);                     // Write Symbol
     *(_pcT + 4)  = RND.cRand((char*)"12", 2);                     // Move  L(1)/R(2)
     
} // END function vBTMutate



//+2 MODULE : BBTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vBBMutate                                     //
//-- SYNOPSIS ----------------------------------------------------------------//
void                                            // Return..:
vBBMutate                                       // Function:
(    char        *_rcProgram,                   // Param 1[out]: Return Mut.BB
     unsigned    _uBBNum,                       //      2[in]..: BB to mutate
     unsigned    _uPStates,                     //      3[in]..: #States in BB
     unsigned    _uMType )                      //      4[in]..: Mutation Type
{                                               //----------------------------//
//   DESCRIPTION
//   Mutate WHOLE BTM program: BB[uBBNum] if defined, & return it in <_rcProgram>
//-2
     const unsigned  uPCells = 5;                       // #Cells pr. Program Trans. 
     const unsigned  uPLeng  = _uPStates * 2*uPCells;   // "Cells in whole program
     char            *pcP;
     char            *pcP1;
     char            *pcP2;
     char            *pcT = 0;
     char            rcResult[300] = "";
     unsigned        uBMate  = 0;
     unsigned        uLocus  = 0;
     unsigned        uLength = 0;
     unsigned        u       = 0;
     unsigned        v       = 0;
          

     // 1: CHECK valid BTM program, - If undefined, generate whole new BTM program!
     if (    ! bRANGE(_uBBNum, 0u, BSL.uLSlots-1)
            || BSL.rpcList[_uBBNum] == 0
            || (pcP = strchr(BSL.rpcList[_uBBNum], '~')) == 0
            || *(++pcP) == '\0' ) {
          vBPCreate(_rcProgram, _uPStates);
          return;
     }


     // 2: COPY BTM program pcBB[_uBBNum] to parameter <_rcProgram>
     strncpy(_rcProgram, pcP, uPLeng);
     _rcProgram[uPLeng] = '\0';
   

     // 3: MUTATE the BTM program <_rcProgram> until valid, - cf. vBPChkStruc rules
     D(DLev3, strcpy(rcResult, _rcProgram));
     D(DLev3, std::cout << "Position............: 0...5....1....5....2....5....3....5....4....5....5" << std::endl);
     D(DLev3, std::cout << "Mutating............: " << _rcProgram << std::endl);
     do {
     
        switch (_uMType) {
        
            case 0: // FIX: Perform point mutation in last transition locus
                    pcT = strchr(pcP+1,'~') + 1;            // Point to last trans.
                    uLocus = (uC2U(*pcT)-1) * (2*uPCells);  // Pgm/genome locus corresp.to last trans.
                    pcP1 = _rcProgram + uLocus;
                    vBTMutate(pcP1, _uPStates);
                    D(DLev3, std::cout << "Fixd mutation at " << uLocus << " : ");
                    break;
                    
            case 1: // RND: Perform random point mutation
                    uLocus = RND.uRand(0,_uPStates-1) * (2*uPCells);
                    pcP1 = _rcProgram + uLocus;
                    vBTMutate(pcP1, _uPStates);
                    D(DLev3, std::cout << "Rand mutation at " << uLocus << " : ");
                    break; 

            case 2: // SEX: Perform 3x random CrossOver mutation with random BB mate (~ Have SEX!)
                    for (v=0; v < 3; v++) {
                        uBMate = RND.uRand(0,BSL.uLLast); 
                        uLocus = RND.uRand(0,_uPStates*(2*uPCells)-1);
                        uLength = RND.uRand(0, uPLeng-1 - uLocus);  // Rest of pgm Xover from uLocus 
                        pcP1 = _rcProgram + uLocus;
                        u = (_uBBNum + uBMate) % (BSL.uLLast+1);    // Wrap in BB list
                        assert ( (pcP2 = strchr(BSL.rpcList[u], '~')) != 0 );
                        strncpy(pcP1, ++pcP2 + uLocus, uLength);
                        D(DLev3, std::cout << std::endl << "Sex " << _uBBNum << " X " << u << " at " << uLocus << "," << uLength << "   ");
                    }                        
                    break;
        }                    
        D(DLev3, strcpy(rcResult, _rcProgram));
        D(DLev3, std::cout << rcResult << std::endl);
        
     } while (eBPChkStruc(_rcProgram, rcResult) != eBP_Ok);
     // Return with *valid* beaver in _rcProgram

} // END function vBBMutate

    

//+2 MODULE : BBTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vBPChkStruc                                   //
//-- SYNOPSIS ----------------------------------------------------------------//
enum eBPType
eBPChkStruc
(   char    *_rcProgram,
    char    *_rcResult )
//  DESCRIPTION
//  Detect "structural/static anomalies" before simulating the BTM
//-2
{
    unsigned    uPStates    = strlen(_rcProgram) / 10;
    unsigned    uPState     = 0;
    unsigned    uPCells     = 5;
    char        cPState     = 0;
    char        *pc         = NULL;
    char        *pc1        = NULL;
    char        *pcEnd      = _rcProgram + uPStates * 2*uPCells;
    int         iFound      = 0;

    
    // Preconditions
    assert(*_rcProgram != '\0');
    assert(bRANGE(uPStates, 1u, 9u));


    // -------------------------------- NO-HALT ------------------------------//
    // Reject NO-HALT graph
    for ( iFound = 0,  pc1 = _rcProgram+2;
          pc1 < pcEnd; pc1 += uPCells ) {
         iFound = iFound || *pc1 == '0'; 
    }            
    if (! iFound) {
        (void) sprintf(_rcResult, "%s(-)\t\t %s\n", "LOOP:NOHALT", _rcProgram);
        return eBP_Split;
    }

    // -------------------------------- 1ST-STATE RECURSION --------------------//
    // Reject direct LOOP 1->1 on input 0
    if (strncmp(_rcProgram, "101", 3) == 0) {
        (void) sprintf(_rcResult,"%s(%c)\t\t %s\n","LOOP:START", *_rcProgram, _rcProgram);
        return eBP_Omega;
    }


    for (   uPState= 1, cPState = '1', pc = _rcProgram;
            uPState <= uPStates;
            uPState++,  cPState++, pc += 2*uPCells) {

        // -------------------------- UNREACHABLE ----------------------------//
        // Reject SPLIT graph ("Forrest" w. state x disconnected => n-1 Beaver!)
        for ( iFound = 0,  pc1 = _rcProgram+2;
            pc1 < pcEnd; pc1 += uPCells) {
            iFound = iFound || *pc1 == cPState;
        }
        if (! iFound) {
            (void) sprintf(_rcResult,"%s(%c)\t\t %s\n","CONN:SPLIT", cPState, _rcProgram);
            return eBP_Split;
        }

        // -------------------------- TRAP -----------------------------------//
        // Reject BLIND state (direct recursion state x->x => n-1 Beaver!)
        if (*pc == *(pc + 2) && *(pc+uPCells) == *(pc+uPCells+2)) {
            (void) sprintf(_rcResult,"%s(%c)\t\t %s\n","CONN:TRAP", cPState, _rcProgram);
            return eBP_Blind;
        }
    }

    return eBP_Ok;
    
} // END function vBPChkStruc



//+2 MODULE : BBTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vBPChkTrans                                   //
//-- SYNOPSIS ----------------------------------------------------------------//
enum eBPType
eBPChkTrans
(   char    *_rcResult )
//  DESCRIPTION
//  Detect "dynamic anomalies" during simulation of the BTM
//-2
{
     char       *pcT    = 0;        // Trace
     char       *pcTI = 0;          // Initial Trace
     unsigned    U      = 0;        //
     unsigned    V      = 0;

     pcTI = _rcResult;
     for (U = 0; U < 2; U++, pcTI++) {
          assert((pcTI = strchr(pcTI, '~')) != NULL);
     }
     
     //------------------------------- 2-STATE LOOP --------------------------//
     // Check trace for simple (2-state) loop: { (S1-X)->(S2-Y) } ...etc.
     // ie. check STST~STST, 8 byte (1K trace ~ 127*8 ~ 254 cycles)
     //------------------------------- 3-STATE LOOP --------------------------//
     // Check trace for 3-state DaisyChain: { (S1-X)->(S2-Y)->(S3-Z) } ...etc.
     // ie. check STSTSI~STSTSI, 12 byte (1K trace ~ 84*12 ~ 168 cycles)
     //------------------------------- 4-STATE LOOP --------------------------//
     // Check trace for 4-state DaisyChain: { (S1-X)->(S2-Y)->(S3-Z)->(S4-Q) } ...etc.
     // ie. check STSTSISI~STSTSISI, 16 byte (1K trace ~ 63*16 = 126 cycles)
      
     for (unsigned I=8u,J=4u; I <= 8u; I+=4u,J+=2u) {
     //for (unsigned I=8u,J=4u; I <= 16u; I+=4u,J+=2u) {    // OOPS: BB(5)!
                                
        for (U = 0, V = 960u/I, pcT = pcTI; 
             U <= V && (strncmp(pcT, pcT+J, J) == 0);
             U++, pcT += I) { 
                D(DLev4, std::cout << U << "/" << V << "<->" << pcT);
        }
        if (U > V ) {
            *_rcResult = '*';   // mark
            return eBP_Omega;
        }             
     }        

     return eBP_Ok;

} // End Function vBPChkTrans



//+2 MODULE : BBTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vBPRun                                        //
//-- SYNOPSIS ----------------------------------------------------------------//
void                                        // Return
vBPRun                                      // Function
(   Beaver     &_BTM,                       // Param 1: [in]  The mother beaver
    char       *_rcProgram,                 //       2: [in]  The program
    char       *_rcResult,                  //       3: [out] The result
    unsigned    _uStep )                    //       4: [in]  Trace ON/OFF
{                                           //--------------------------------//
//  DESCRIPTION
//  Run the beaver
//-2
    unsigned    U       = 0u;               // Scratch
    int         iRes    = 0;                //
    int         iPHalt  = 1;                //
                                            //--------------------------------//
    
    memset(_rcResult, '-', 500);


    // IF VALID BTM program...
    if (eBPChkStruc(_rcProgram, _rcResult) == eBP_Ok) {

        // 1: SETUP BTM
        _BTM.vPReset(_rcProgram);
    
        // 2: RUN BTM: First 2K Trans, - then if OK, up to 1M Trans...
        _BTM.vPRun(_uStep, 2048ul, _rcResult);
        assert(sscanf(_rcResult, "%ud", &U) == 1);
        if (U == 0u && eBPChkTrans(_rcResult) == eBP_Ok) {
            _BTM.vPRun(_uStep, 998000ul, _rcResult);
        }

        // 3: UPDATE history & REPORT result
        std::cout << _rcResult;
        sscanf(_rcResult, "%d", &iRes);
        if (iRes == iPHalt) {
            BSL.vInsert(_rcResult, 1);
            std::cout << BSL;
        }
        
    } else
    // ELSE REPORT result (invalid beaver)
        std::cout << _rcResult;
        
} // End Function vBPRun()        



// END MODULE BBTEST.CPP                                                      //
//============================================================================//
