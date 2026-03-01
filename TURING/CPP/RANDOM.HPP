//+1==========================================================================//
//  MODULE                      RANDOM.HPP                                    //
//============================================================================//
//  FUNCTION    Definition of PMMLCG pseudo random number generator;
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
#if !defined RANDOM_HPP
#define RANDOM_HPP
#include <time.h>


//=+2 MODULE RANDOPM.HPP =====================================================//
//    CLASS                     CRandom                                       //
//=== SYNOPSIS ===============================================================//
//    Prime Modulus Multiplicative Linear Congruential Genarator (PMMLCG)
class CRandom {

private :
    // DATA MEMBESR: "Key numbers" for PMMLCG random number generator --------//
        long        a;              // multiplier : a long [2...m-1]
        long        m;              // modulus....: a large prime
        long        q;              // quotient...: (m div a)
        long        r;              // rest.......: (m mod a)
        unsigned long   ulSeed;     // seed for fRand() function

    // HELPING FUNCTIONS: Auxiliary tasks
        int         chiSquare   (void);

public  :
    // MANAGER FUNCTIONS: Class con/destruct ---------------------------------//
    		CRandom     ( long _a=16807L,  long _m=2147483647L,
                          long _q=127773L, long _r=2836L ) :
                                a(_a), m(_m), q(_q), r(_r)
                          {  
                            vRandomize();
                          };
            ~CRandom()  { /* nothing */ };

    // IMPLEMENTOR FUNCTIONS: Class abstraction ------------------------------//
            void   vRandomize   ( unsigned long _ulSeed=0)
                                { ulSeed = (_ulSeed != 0 ? _ulSeed :
                                           (unsigned long) time(NULL) );
                                }
            float        fRand   ();
          	unsigned     uRand   (unsigned uStart, unsigned uEnd);
          	char         cRand   (char  *pcSelect, unsigned uEnd);

    // ACCESSOR FUNCTIONS: Class member I/O ----------------------------------//
    		friend std::ostream& operator<<(std::ostream&, const CRandom&);

    // TESTING FUNCTIONS: Class Built-In-Test (BIT) --------------------------//
            void        vBITInt (void);
            void        vBITChi (void);
            void        vBITSeq (void);

}; // END CLASS CRandom


#endif // RANDOM_HPP
// END MODULE RANDOM.CPP                                                      //
//============================================================================//
