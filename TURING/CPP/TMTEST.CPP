//+1==========================================================================//
//  MODULE                      TMTEST.CPP                                    //
//============================================================================//
//  FUNCTION    Test CLASSes TURING and BEAVER (Universal TM and BusyBeaver TM)
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Modules TURING.HPP/CPP for def./implementation of Class Turing 
//              Modules BEAVER.HPP/CPP for def./implementation of Class Beaver
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  https://www.quantamagazine.org/amateur-mathematicians-find-fifth-busy-beaver-turing-machine-20240702/
//  USAGE       
//  DOC.
//  BUGS
//  COPYRIGHT   none!
//-1==========================================================================//

#include <iostream>			// <iostream.h> 
#include <stdio.h>
#include <memory.h> 
#include <ctype.h>

#include "Turing.hpp"  
#include "Beaver.hpp"


void    vTestTM     (unsigned uStep);
void    vTestBBTM   (unsigned uStep);
void    vTestBTM    (unsigned uStep);


 
//+2 MODULE : TMTEST.CPP -----------------------------------------------------//
//   FUNCTION public            Main                            entry point   //
//-- SYNOPSIS ----------------------------------------------------------------//
int 
main()
{   
    char     cHitKey;
    unsigned uStep = 0;

    
    do {
        std::cout << "\t+-------------------------------------------+\n";
        std::cout << "\t|  1: Test TM                               |\n";
        std::cout << "\t|  2: Test BBTM(n)                          |\n";
        std::cout << "\t|  3: Test BTM(program)                     |\n";
        std::cout << "\t+-------------------------------------------+\n";
        std::cout << "\t|  s: Toggle SingleStep mode (" << uStep <<")\n";
        std::cout << "\t|  x: Exit                                  |\n";
        std::cout << "\t+-------------------------------------------+\n";
        
        std::cout << "\tPlease select from the menu [1|2|3|s|x] ->\n"; 
        std::cin  >> cHitKey;
        cHitKey = toupper(cHitKey);
        
        switch (cHitKey) {
            case '1' :  vTestTM(uStep);
                        break;
            case '2' :  vTestBBTM(uStep);
                        break;
            case '3' :  vTestBTM(uStep);
                        break;
            case 'S' :  uStep = ! uStep;
                        break;                      
            case 'X' :  break;
            default:    std::cout << cHitKey <<"???\n";
                        break;
        }           

    } while (cHitKey != 'X');

} // END function main



//+2 MODULE : TMTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vTestTM                             private   //
//-- SYNOPSIS ----------------------------------------------------------------//
void
vTestTM(unsigned uStep)
{
// DESCRIPTION : Test Class Turing
// Using TM for calculating odd(O) or even(E) number of X'es
//-2

    // 0: DEFINE datastructs for the UTM Program and Tape
    char *rcProgram;                    // For transitions
    char rcTape[100];                   // For input string
    char cHitKey;                       //
    enum Turing::ePExit eResult;        //
    char rcResult[5000];                //

    
    // 0:   DEFINE the Odd/Even (O/E) program for the TM
    //      +---------------+------------------------+
    //      |   1       X   |   2       b       2    |
    //      |___1_______b___|___0_______E______(2)___|
    //      |   2       X   |   1       b       2    |
    //      |___2_______b___|___0_______O______(2)___|
    rcProgram = (char*) "1X2b21b0E22X1b22b0O2"; 

    
    // 1: PROMPT for TM tape content
    std::cout << "An OETM counts the number of X'es on the input Tape\n";
    std::cout << "and outputs the result: O for odd, E for even #X'es\n";


    do {
        // 2: GET a string of X'es 
        std::cout << "Type a string of X'es->"; std::cin >> rcTape;  

        // 3: BUILD & RUN the OETM on the X-string
        {
            // 3.1: BUILD an OETM, using the UTM class
            Turing TM(1,2,2,1024, rcProgram, rcTape, 'b');
            std::cout << "Hit a key to run the O/E TM..." << '\n';
            std::cin >> &cHitKey;
               
            // 3.2: RUN the OETM on the input tape
            while ((eResult = TM.ePTrans()) == Turing::ePE_PTrans) 
                if (uStep) {
                    std::cout << TM << "\n";  
                    std::cin  >> &cHitKey;
                }
                 
            // Format output string
            (void) sprintf(rcResult, "%d\t %c\t %lu\t %s\n", eResult, *TM.pcTape, TM.ulTrans, TM.rcProgram);
            std::cout << rcResult << '\n';
        } // TM out of scope ("Hasta la Vista, Baby!!")


        // 4: LOOP once more ?
        std::cout << "OETM Again ? [Y=yes,N=no] >";
        std::cin >> &cHitKey;
        
    } while (cHitKey == 'Y');
    
} // END function vTestTM



//+2 MODULE : TMTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vTestBBTM                           private   //
//-- SYNOPSIS ----------------------------------------------------------------//
void
vTestBBTM(unsigned uStep)
{
// DESCRIPTION : Test Class Beaver
// Using BBTM(n) with n in [1,2,3,4,5,6]
//-2

    // 0: DEFINE datastructs for the UTM Program and Tape
    char        *rcProgram;                 // For transitions
    char        rcResult[2000];             // For result
    unsigned    U;
    char        cHitKey;

    
    // 1: PROMPT for n, the #States for the BBTM
    std::cout << "An n-state BBTM :" << '\n';               
    std::cout << "   - takes as input an unlimited Tape filled with 0's\n";
    std::cout << "   - writes at least as many 1's as any other n-state TM\n";
    std::cout << "   - eventually halts!\n";
  
    do {
        // 2: GET #states (n) for the BBTM & setup the BBTM(n) program :
        std::cout << "Please input n for the BBTM [1,2,3,4,5] ->\n";
        std::cin  >> U;
        switch ( U) {
            		// ============================================================
					//  BusyBeaver TM : Program/Trans.Sequence
					//  +---------------+------------------------+	*) 0=Halt
					//  |    INPUT      |           OUTPUT       |	   1=Left
					//  |State  Symbol  |   State   Symbol  Move*|	   2=Right
					//  +---------------+------------------------+

            case 1: //					#1s/#Trans/#TMs			    
					//  __BB-1STATE_________1/1/64________________ 
                    //  |   1       0   |   0       1       (2)  |         
                    //  |___1_______1___|___0_______1_______(2)__|       
                    //  RESULT: 	1x1    	1 Step  
                        rcProgram = (char*) "1001211012";
                        break;
            case 2:     						
            		//  __BB-2STATE_________4/6/20.736____________ 
					// |    1       0   |   2       1       2    |        
					// |____1_______1___|___2_______1_______1____| 
					// |    2       0   |   1       1       1    |
					// |____2_______1___|___0_______1______(2)___| 
					// RESULT:		4x1  	6 Steps 
						rcProgram = (char*) "10212112112011121012";
                        break;              
            case 3: 
					//  __BB-3STATE_________6/13/268.435.456______ 
                    //  |   1       0   |   2       1       2    |
                    //  |___1_______1___|___3_______1_______1____|
                    //  |   2       0   |   1       1       1    |
                    //  |___2_______1___|___2_______1_______2____|
                    //  |   3       0   |   2       1       1    |
                    //  |___3_______1___|___0_______1______(2)___|
                    //  RESULT: 	6x1   	13 Steps    		 
                        rcProgram = (char*) "102121131120111212123021131012";   
                        break;
            case 4: 
					//  __BB-4STATE________12/107/25.600.000.000__ 
                    //  |   1       0   |   2       1       2    |
                    //  |___1_______1___|___2_______1_______1____|
                    //  |   2       0   |   1       1       1    |
                    //  |___2_______1___|___3_______0______(1)___|
                    //  |   3       0   |   0       1       2    |
                    //  |___3_______1___|___4_______1_______1____|
                    //  |   4       0   |   4       1       2    |
                    //  |___4_______1___|___1_______0______(2)___|
                    //   RESULT:	13x1  	107 Steps 
                        rcProgram = (char*) "1021211211201112130130012314114041241102";
                        break;
            case 5: 
            		//  _BB-5STATE Candidate______________________
					//  |   1       0   |   2       1       1    |
					//  |___1_______1___|___1_______1_______1____|
					//  |   2       0   |   3       1       2    |
					//  |___2_______1___|___2_______1_______2____|
					//  |   3       0   |   1       1       1    |
					//  |___3_______1___|___4_______1_______2____|
					//  |   4       0   |   1       1       1    |
					//  |___4_______1___|___5_______1_______2____|
					//  |   5       0   |   0       1      (2)   |
					//  |___5_______1___|___3_______0_______2____| 
                    //   RESULT:	13x1  	107 Steps 					
					//	rcProgram = (char*) "10211111112031221212301113141240111415125001251302";
					//	break;
					
					//  _BB-5STATE Candidate______________________
                    //  |   1       0   |   2       1       2    |
                    //  |___1_______1___|___3_______0_______1____|
                    //  |   2       0   |   3       1       2    |
                    //  |___2_______1___|___4_______1_______2____|
                    //  |   3       0   |   1       1       1    |
                    //  |___3_______1___|___2_______0_______2____|
                    //  |   4       0   |   5       0       2    |
                    //  |___4_______1___|___0_______1______(2)___|
                    //  |   5       0   |   3       1       1    |
                    //  |___5_______1___|___1_______1_______2____|
					//	RESULT: 	501x1	134.467 Steps
                    //	rcProgram = (char*) "10212113012031221412301113120240502410125031151112";
                    //	break;

					//  The BBn for n=5 was discovered 1989 and proven in 2026.
					//  Most of the 17 trillion 5-state BTMs were eliminated
					//  through state machine analysis, and of the remaining
					//	handful of candidates, this one was proven to be BB5
					//  _BB-5STATE________________________________
                    //  |   1       0   |   2       1       2    |
                    //  |___1_______1___|___3_______1_______1____|
                    //  |   2       0   |   3       1       2    |
                    //  |___2_______1___|___2_______1_______2____|
                    //  |   3       0   |   4       1       2    |
                    //  |___3_______1___|___5_______0_______1____|
                    //  |   4       0   |   1       1       1    |
                    //  |___4_______1___|___4_______1_______1____|
                    //  |   5       0   |   0       1       2    |
                    //  |___5_______1___|___1_______0_______1____|
					//	RESULT: 	3848x1 	 4.940.398 Steps <-OBS: Resource limited...
					//     BB5:     4098x1  47,176,870 Steps
						rcProgram = (char*) "10212113112031221212304123150140111414115001251101";
						break;
						
        	case 6:
            		//  The next will be BB6, but there are 60 quadrillion BTMs,
					//  so analyzing these will be close to impossible (NP time).
					//  _BB-5STATE________________________________
                    //  |   1       0   |   2       1       2    |
                    //  |___1_______1___|___1_______1_______2____|
                    //  |   2       0   |   3       1       2    |
                    //  |___2_______1___|___0_______1_______2____|
                    //  |   3       0   |   4       1       1    |
                    //  |___3_______1___|___6_______0_______2____|
                    //  |   4       0   |   1       1       2    |
                    //  |___4_______1___|___5_______0_______1____|
                    //  |   5       0   |   4       0       1    |
                    //  |___5_______1___|___3_______1_______2____|
                    //  |   6       0   |   1       1       2    |
                    //  |___6_______1___|___5_______0_______2____|
					//	RESULT: 	3977x1 	10.039.866 Steps   <-OBS resource limited
					// 	  
					rcProgram = (char*) "102121111220312210123041131602401124150150401513126011261502";
						break;
 										
            default:    rcProgram = NULL;
                        break;
        }                   


        // 3: BUILD & RUN the BBTM(n), if defined                   
        if (rcProgram != NULL) {
            Beaver BTM(U, rcProgram);
            BTM.vPRun(uStep, 1000000ul, rcResult);
        } // BTM: "Hasta la Vista, Baby!"
        std::cout << "RESULT: " << rcResult << "\n";
            

        // 4: LOOP once more ? 
        std::cout << "BBTM Again ? [Y=yes,N=no] ->\n";
        std::cin  >> &cHitKey;
        
    } while (cHitKey == 'Y');

} // END function vTestBBTM



//+2 MODULE : TMTEST.CPP -----------------------------------------------------//
//   FUNCTION public            vTestBTM                            private   //
//-- SYNOPSIS ----------------------------------------------------------------//
void
vTestBTM(unsigned uStep)
{
// DESCRIPTION : Test Class Beaver
// Using BTM(Program), with Program provided as input string
//-2

    // 0: DEFINE datastructs for the UTM Program and Tape
    char        rcProgram[101];             // For transition definitions
    char        rcResult[2000];             // For result
    unsigned    U       = 0;                //
    char        cHitKey = 'N';              //

    
    // 1: PROMPT for the BTM program
    std::cout << "A Beaver Turing Machine (BTM) :" << '\n';               
    std::cout << "   - takes as input an unlimited Tape filled with 0's\n";
    std::cout << "   - writes 1's on the tape\n";
    std::cout << "   - until [Halt|Tape Under- or Overflow| Loop Omega]\n";

    // 2: CREATE (but don't run) the "mother beaver", with room for 10 states!
    memset(rcProgram, '~', 100);
    rcProgram[100] = '\0';
    Beaver BTM(U, rcProgram);
    
    do {
        // 2: GET program for the BTM
        std::cout << "Please input program string for the BTM (states: n<=10) \n->";
        rcProgram[0] = '\0';
        std::cin  >> rcProgram;


        // 3: RESET & RUN the BTM(program), if program defined
        if (*rcProgram != '\0') { 
            BTM.vPReset(rcProgram); 
            BTM.vPRun(uStep, 50000000ul, rcResult);
        }
        std::cout << "RESULT: " << rcResult << "\n";


        // 4: LOOP once more ? 
        std::cout << "BTM Again ? [Y=yes,N=no] ->\n";
        std::cin  >> &cHitKey;
        
    } while (cHitKey == 'Y');
  
  // BTM: "Hasta la Vista, Baby!"
} // END function vTestBBTM

                                                


// END MODULE TMTEST.CPP                                                      //    
//============================================================================//
    