//+1==========================================================================//
//  MODULE                      RANDOM.CPP                                    //
//============================================================================//
//  FUNCTION    Implementation of a pseudo random generator : PMMLCG
//              Prime Modulus Multiplicative Linear Congruential Genarator.
//  SYSTEM      IBM PC >= i386 cpu
//  SEE ALSO    Module RANDOM.HPP
//  PROGRAMMER  Allan Dystrup
//  VERSION     0.1
//  REFERENCES  coming soon
//  USAGE       "
//  DOC.        Incorporated in source
//  BUGS        ?
//  COPYRIGHT   none!
//-1==========================================================================//


//----------------------------------------------------------------------------//
//                              Includes                                      //
//----------------------------------------------------------------------------//
#include <iostream>				// <iostream.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>              // fputs
#include "random.hpp"
#include "xassert.hpp"


#define uROUND(f)       (unsigned) (fmod(f,1) < 0.5 ? (f) : ((f)+1.0) )


//----------------------------------------------------------------------------//
//                              TestDriver                               Test //
//----------------------------------------------------------------------------//
// UnComment for activation of class test driver (using class BIT)
//#define RANDOM_MAIN
#undef RANDOM_MAIN




//=== MODULE RANDOPM.HPP =====================================================//
//    CLASS  ****************** CRandom ************************************* //
//============================================================================//


//+3 MODULE : RANDOM.CPP ------ CLASS : CRANDOM ------------------------------//
//   FUNCTION public            fRand                                         //
//-- SYNOPSIS ----------------------------------------------------------------//
float                           // Return   Float PseudoRandom# in range [0..1]
CRandom::fRand                  // Function PMMLCG of PseudoRandom Numbers
()                              // Param    None
{                               //--------------------------------------------//
//  DESCRIPTION
//  Prime Modulus Multiplicative Linear Congruential Genarator (PMMLCG)
//  using Lehmer's algorithm implemented by Schrage's method.
//  RETURN  Pseudorandom number in range [0...1]
//-3
    long    lo, hi, yx;         // Scratch variables
    float   fRes;               // Return  value

    // CALCULATE Lehmer's expression : f(X) = a*X mod m
    // using Schrage's method to eliminate potential overflow by a*X, and
    // class member static unsigned long ulSeed as base for the calculation
    lo  =   ulSeed % q;
    hi  =   ulSeed / q;
    yx  =   (a * lo) - (r * hi);

    // Always in range [0...2m-1]
    ulSeed = (yx > 0 ? yx : yx + m);

    // NORMALIZE to range [1/m...1-1/m]
    fRes = (float) ulSeed / (float) m;

    return fRes;

} // END FUNCTION CRandom::fRand()



//+3 MODULE : RANDOM.CPP ------ CLASS : CRANDOM ------------------------------//
//   FUNCTION public            uRand                                         //
//-- SYNOPSIS ----------------------------------------------------------------//
unsigned int                    // Return   Unsigned PRandom# range [uStart..uEnd]
CRandom::uRand                  // Function PMMLCG mapped from float[0..1] to int
(   unsigned    uStart,         // Param 1: [in] Start of range for return value
    unsigned    uEnd )          //       2: [in] End of range for return value
{                               //--------------------------------------------//
//  DESCRIPTION
//  Calls CRandom::fRand() to calculte a float PseudoRandom# in range [0..1];
//  RETURN Float PRandom# mapped map it to unsigned int in range: [uStart..uEnd].
//-3
    float       fRes;           //
    unsigned    uRes;           //

    fRes =  (float) uStart + fRand() * (float)(uEnd - uStart);
    uRes = uROUND(fRes);
    return uRes;

} // END FUNCTION CRandom::uRand()



//+3 MODULE : RANDOM.CPP ------ CLASS : CRANDOM ------------------------------//
//   FUNCTION public            cRand                                         //
//-- SYNOPSIS ----------------------------------------------------------------//
char                            // Return   Random char from string pcSelect
CRandom::cRand                  // Function Pick random char: pcSelect[0..uEnd-1]
(   char     *pcSelect,         // Param 1: [in] String from which to select char
    unsigned  uEnd )            //       2: [in] End of range in pcSelect[0..uEnd-1]
{                               //--------------------------------------------//
//  DESCRIPTION
//  Calls CRandom::uRand() to calc. an unsigned PseudoRandom# in range [0..uEnd-1];
//  RETURN Char, using unsigned PRandom# as index into string pcSelect[0..uEnd-1]
//-3
    char cRes;
    cRes = *(pcSelect + uRand(0,uEnd-1));
    return cRes;

} // END FUNCTION CRandom::cRand()



//+2 MODULE : RANDOM.CPP ------ CLASS : CRANDOM ------------------------------//
//   FUNCTION public            operator<<                     friend CRandom //
//-- SYNOPSIS ----------------------------------------------------------------//
std::ostream&                   // Return   C++ OutStream object
operator<<                 		// Function Stream CRandom object to cout
(   std::ostream&   os,         // Param 1: [in|out] C++ OutStream object
    const CRandom&  CR )        //       2: [in]     CRandom object
{                               //-------------------------------------------//
// DESCRIPTION
//
//-3
     std::cout << "a=" << CR.a << " m=" << CR.m << " q=" << CR.q << " r=" << CR.r;
     std::cout << "ulSeed=" << CR.ulSeed;
     std::cout << std::endl;
     return os;

} // END FUNCTION friend CRandom operator<<



//+2 MODULE : RANDOM.CPP ------ CLASS : CRANDOM ------------------------------//
//   FUNCTION public            vBITInt                                   BIT //
//-- SYNOPSIS ----------------------------------------------------------------//
void                            // Return   Nothing
CRandom::vBITInt                // Function BuiltInTest for internal validation
(void)                          // Param    None
{                               //--------------------------------------------//
//  DESCRIPTION
//  Run internal test based on: ulSeed(1)=1ul => ulSeed(10001)=1043618065ul
//-3
    static char m0[] = "\n=====TEST DRIVER FOR RANDOM=====\n";
    static char m1[] = "\nInternal test of PMMLCG \n";
    static char m2[] = "\n...PMMLCG passed internal test !\n";
    static char m3[] = "\n...PMMLCG failed internal test ! \a\n";

    fputs(m0, stdout);
    fputs(m1, stdout);

    vRandomize(1ul);
    for (int i = 1; i <= 10000; i++) {
        if (i % 1000 == 0) putchar('>');
        (void) fRand();
    }
    fputs((ulSeed==1043618065ul ? m2 : m3), stdout);

} // END FUNCTION CRandom::vBITInt() 



//+2 MODULE : RANDOM.CPP ------ CLASS : CRANDOM ------------------------------//
//   FUNCTION public            vBITChi                                   BIT //
//-- SYNOPSIS ----------------------------------------------------------------//
void                            // Return   Nothing
CRandom::vBITChi                // Function BuiltInTest for ChiSquare validation
(void)                          // Param    None
{                               //--------------------------------------------//
//  DESCRIPTION
//  Run chi square test
//-3
    static char m0[] = "\n=====TEST DRIVER FOR RANDOM=====\n";
    static char m1[] = "\nChiSquare test of PMMLCG \n";
    static char m2[] = "\n...PMMLCG passed chiSqr test !\n";
    static char m3[] = "\n...PMMLCG failed chiSqr test ! \a\n";
    char    inp[80];

    fputs(m0, stdout);
    fputs(m1, stdout);

    for (*inp = 'x'; *inp != 'q'; fgets(inp, 80, stdin))
       fputs((chiSquare() ? m2 : m3), stdout);

} // END FUNCTION CRandom::vBITChi()



//+2 MODULE : RANDOM.CPP ------ CLASS : CRANDOM ------------------------------//
//   FUNCTION public            vBITSeq                                   BIT //
//-- SYNOPSIS ----------------------------------------------------------------//
void                            // Return   Nothing
CRandom::vBITSeq                // Function BuiltInTest for sequence validation
(void)                          // Param    None
{                               //--------------------------------------------//
//  DESCRIPTION
//  Run simple sequence test
//-3
    static char m0[] = "\n=====TEST DRIVER FOR RANDOM=====\n";
    static char m1[] = "\nTest of random sequence = f( seed )\n";
    static char m2[] = "\n(To break test loop : enter q !)\n";
    char        inp[80];
    unsigned    u;

    fputs(m0, stdout);
    fputs(m1, stdout);
    fputs(m2, stdout);

    vRandomize();
    for (*inp = 'x'; *inp != 'q'; fgets(inp, 80, stdin)) {
        for (int i = 1; i <= 5; i++) {
            u = uRand(1,9);
            printf( "%lf  %u %c\n", (double)fRand(), u, cRand((char*)"ABCDEFGHIJ", u));
        }
        putchar('\n');
    }

} // END FUNCTION CRandom::vBITSeq()



//+4 MODULE : RANDOM.CPP -----------------------------------------------------//
//   FUNCTION private           chiSquare                                     //
//-- SYNOPSIS ----------------------------------------------------------------//
// Select random func for test 
#define CRand                   // Test Class CRandom::fRand() - prim. purpose
//#define BC4                   // Test Borland C++ v4.02 rand()  - for compare
//#define MSC                   // Test Microsoft C++ v1.5 rand() - for compare
                                //--------------------------------------------//
int	                            // Return   Nothing
CRandom::chiSquare              // Function ChiSquare test of CRandom::fRand()
(void)                          // Param    None
{                               //--------------------------------------------//
// DESCRIPTION
// Generate sample of PseudoRandom# using CRandom::fRand(), -
// Count frequencies and perform ChiSquare test for "goodness-of-fit"
// of observed freq.distrib vs. expected random (Poisson) freq.distrib :
// Hypothesis   null H0 : freq[0] == freq[1] == ... == freq[sampleRange]
//              alt. H1 : fraq[0] != freq[1] != ... != freq[sampleRange]
// Statistics   one-sample test comparing observed vs. expected frequencies
//              in discrete categories :
//              ChiSquare = SUM((observed-expected)**2 / expected)
// Sampling     sample size N=1000, discrete categories k=101, #samples = 10
// Rejection    H0 (Random) will be rejected, if the probability of the
//              observed chiSqr-value under H0 for degrees of freedom df=k-1
//              is < tabulated value for significance level a, where a = .050
//              Otherwise H0 is accepted at 95% probability level (P > 0.05)
//-4
    int     i, j, k;                // scratch counters
    int     freq[101];              // count of 1000 random# in range [0..100]

    int     sampleRuns  =   10;     // number of samples = 10
    int     sampleRange =   101;    // discrete categories k=101; df=(k-1)=100
    int     sampleSize  =   1000;   // sample size N=1000 R# in [0..sampleRange]
    double  expMean     =   0.0;    // sampleSize / sampleRange
    double  obsMean     =   0.0;    // observed mean, using fRand()
    double  devMean     =   0.0;    // obsMean - expMean
    double  sqrDevMean  =   0.0;    // devMean**2
    double  sumSqrDevMean=  0.0;    // SUM(sqrDevMean)
    double  chiSqr      =   0.0;    // sumSqrDevMean / expMean
    float   sigLevel    =   0.050f; // reject if P < 0.05 for chiSquare

    // %points for ChiSquare distribution, with (n-1)=100
    static float chiSqrTable[14][2] =
    {
        {  67.3276f, 0.995f }, {  70.0649f, 0.990f }, {  74.2219f, 0.975f },
        {  77.9295f, 0.950f }, {  82.3581f, 0.900f }, {  90.1332f, 0.750f },
        {  99.3341f, 0.500f }, { 109.141f , 0.250f }, { 118.498f , 0.100f },
        { 124.342f , 0.050f }, { 129.561f , 0.025f }, { 135.807f , 0.010f },
        { 140.169f , 0.005f }, { 149.449f , 0.001f }
    };
    #if defined(CRand)                  // CRandom
        vRandomize(0);                  
    #elif defined(BC4)                  // BC4
        randomize();                    
    #elif defined(MSC)                  // MSC
        srand((unsigned) time(NULL));   
    #endif

    // Run chi-square test "sampleRuns" times
    for (k = 1; k <= sampleRuns; k++) {

        // Generate sample of random # and count frequencies
        for (i = 0; i < sampleRange; i++)
            freq[i] = 0;                		// Clear
        for (i = 0; i <= sampleSize; i++) {
            #if defined(CRand)                  // CRandom
                j = (int)((float)(sampleRange-1) * fRand());    
            #elif defined(BC4) || defined(MSC)  // BC4 || MSC
                j =  rand() % sampleRange;                      
            #endif
            freq[j] += 1;               		// Count
        }
       assert( forall( (int i = 0; i < sampleRange; i++), 
       		
       		0 <= freq[i] && freq[i] <= sampleSize)); // TEST ASSERT
            
        // Calc. expexted mean for random distribution
        expMean = (double) sampleSize / (double) sampleRange;   // 1000/101~10
        // Calc. observed ChiSquare
        sumSqrDevMean = 0;              // Clear
        for (i = 0; i < sampleRange; i++) {
            obsMean = freq[i];
            devMean = obsMean - expMean;
            sqrDevMean = devMean * devMean;
            sumSqrDevMean += sqrDevMean;// Count
        }
 
        // Evaluate chrSqr for df == sampleRange-1
        chiSqr = sumSqrDevMean / expMean;
        for (i = 0; i <= 12 && chiSqr > chiSqrTable[i][0]; i++)
            /*lookup*/;
        printf( "chiSqr=%03.3lf < chiSqrTable=%03.3lf :\t (P~%1.3lf a=%1.3lf)\n",
                chiSqr, chiSqrTable[i][0], chiSqrTable[i][1], sigLevel);
        if (chiSqrTable[i][1] < sigLevel)
            break;              // OOps : H0 rejected!
    }

    // Return the verdict
    return(k < sampleRuns ? 0 : 1);

} // END FUNCTION CRandom::chiSquare()



#if defined RANDOM_MAIN
//+3 MODULE : RANDOM.CPP =====================================================//
//   FUNCTION ***************** main ***************************** TestDriver //
//== SYNOPSIS ================================================================//
int                            // Return   Nothing
main                            // Function Exercise the BIT func's of CRandom
(void)                          // Param    None
{                               //--------------------------------------------//

    CRandom Rnd;                // Instantiate a CRandom object

    Rnd.vBITInt();              // Exercise the Internal  BIT
    Rnd.vBITChi();              //    -"-       ChiSquare BIT
    Rnd.vBITSeq();              //    -"-       Sequense  BIT
    
    return 1;
    
} // END FUNCTION main()
#endif // RANDOM_MAIN



// END MODULE RANDOM.CPP                                                      //
//============================================================================//
